/**
*	@file		alsaHandling.h
*	@author		Nahum Budin
*	@date		1-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@version	29-Jan-2021	1.1 
*					1. Code refactoring and notaion.
*					2. Adding AudioManger object to handle audio shared memory output data
*					3. Adding setting sample-rate and block-size
*				11-Nov-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 October 18, 2017)
*
*	@brief		ALSA audio handling - receives float data and sends as PCM 16
*/

#include "alsaAudioHandling.h"

#include <math.h>

#include "../Audio/audioManager.h"
#include "../commonDefs.h"

/* A pointer to the AlsaLibHandle singlton instance object */
AlsaHandler *AlsaHandler::alsa_handler = NULL;

/* The ALSA audio device */
static char dev[] = _DEFAULT_ALSA_AUDIO_DEVICE;
		
struct transfer_method tmethod { "async", SND_PCM_ACCESS_RW_INTERLEAVED, async_loop };

AudioManager *alsa_audio_manager = NULL;

/**
*   @brief  Creates and return a pointer to a singelton AlsaHandler  instance.
*   @param  none
*   @return a pointer to a singelton AlsaHandler audio-block object instance
*/
AlsaHandler *AlsaHandler::get_instance() 
{		
	if (!alsa_handler) 
	{
		alsa_handler = new AlsaHandler;
	}
		
	return alsa_handler;
}

AlsaHandler::AlsaHandler() 
{		
	alsa_handler = this;
	// Initialize static member of class for default values
	// Signed 16 bit little endian 
	format = SND_PCM_FORMAT_S16;
	rate = _DEFAULT_SAMPLE_RATE; 
	device = dev;
	block_size = _DEFAULT_BLOCK_SIZE;
	period_time = _DEFAULT_ALSA_AUD_PERIOD_TIME_USEC;
	buffer_time = _DEFAULT_ALSA_AUD_PERIOD_TIME_USEC * 10; //50000;				
	format_bits = snd_pcm_format_width(format);
	max_val = (1 << (format_bits - 1)) - 1;
	bps = format_bits / 8;
	phys_bps = snd_pcm_format_physical_width(format) / 8;
	channels = 2;                       
		
	big_endian = snd_pcm_format_big_endian(format) == 1;
	to_unsigned = snd_pcm_format_unsigned(format) == 1;
	is_float = true;
		
	verbose = 0;
	resample = 1;
	period_event = 0;

	thread_is_running = false;	
	
	alsa_audio_manager = AudioManager::get_instance();
}

AlsaHandler::~AlsaHandler()
{
	
}

/**
*   @brief  Sets the audio sample-rate and updates buffer time and period time.
*   @param  int	_SAMPLE_RATE_44 (44100KHz/default) or _SAMPLE_RATE_48 (48KHz).
*   @return sample-rate.
*/
int AlsaHandler::set_sample_rate(int samp_rate)
{
	if (is_valid_sample_rate(samp_rate))
	{
		rate = samp_rate;
		set_period_time_us(block_size * 1000000.f / (rate + 0.5f));
	}
	else
	{
		set_sample_rate(_DEFAULT_SAMPLE_RATE);
	}
	
	return rate;
}

/**
*   @brief  Sets the audio block-size and updates buffer time and period time.
*   @param  size _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512 (default), _AUDIO_BLOCK_SIZE_1024
*   @return set bloc-size	_AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024.
*/
int AlsaHandler::set_audio_block_size(int size)
{
	if (is_valid_audio_block_size(size))
	{
		block_size = size;
		set_period_time_us(block_size * 1000000.f / (rate + 0.5f));
		
	}
	else
	{
		set_audio_block_size(_DEFAULT_BLOCK_SIZE);
	}
	
	return block_size;
}

/**
*   @brief  Set period time [uSec].
*   @param  ptu		period time [uSec]
*   @return 0
*/
int AlsaHandler::set_period_time_us(unsigned long ptu) { 
	
	period_time = ptu;
	buffer_time = 10*ptu; // TODO: ???
	
	return 0;
}

/**
*   @brief  Set hardware parameters
*   @param  handle	a pointer to a snd_pcm_t struct
*	@param	params	a pointer to a snd_hw_params_t struct
*	@param	access	a pointer to a snd_pcm_access_t struct
*   @return 0 if setting done OK; non zero otherwise.
*/
int AlsaHandler::set_hw_params(
	snd_pcm_t *handle,
	snd_pcm_hw_params_t *params,
	snd_pcm_access_t access)
{
	unsigned int r_rate;
	snd_pcm_uframes_t size;
	int err, dir;
	// choose all parameters
	err = snd_pcm_hw_params_any(handle, params);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
		return err;
	}
	// set hardware resampling
	err = snd_pcm_hw_params_set_rate_resample(handle, params, resample);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Resampling setup failed for playback: %s\n", snd_strerror(err));
		return err;
	}
	// set the interleaved read/write format
	err = snd_pcm_hw_params_set_access(handle, params, access);
	if (err < 0) {
		fprintf(stderr, "ALSA: Access type not available for playback: %s\n", snd_strerror(err));
		return err;
	}
	// set the sample format 
	err = snd_pcm_hw_params_set_format(handle, params, format);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Sample format not available for playback: %s\n", snd_strerror(err));
		return err;
	}
	// set the count of channels
	err = snd_pcm_hw_params_set_channels(handle, params, channels);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Channels count (%u) not available for playbacks: %s\n", channels, snd_strerror(err));
		return err;
	}
	// set the stream rate 
	r_rate = rate;
	err = snd_pcm_hw_params_set_rate_near(handle, params, &r_rate, 0);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Rate %uHz not available for playback: %s\n", rate, snd_strerror(err));
		return err;
	}
	if (r_rate != rate) 
	{
		fprintf(stderr, "ALSA: Rate doesn't match (requested %uHz, get %iHz)\n", rate, err);
		return -EINVAL;
	}
	// set the buffer time
	err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, &dir);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to set buffer time %u for playback: %s\n", buffer_time, snd_strerror(err));
		return err;
	}
	err = snd_pcm_hw_params_get_buffer_size(params, &size);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to get buffer size for playback: %s\n", snd_strerror(err));
		return err;
	}
	buffer_size = size;
	// set the period time
	err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, &dir);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to set period time %u for playback: %s\n", period_time, snd_strerror(err));
		return err;
	}
	err = snd_pcm_hw_params_get_period_size(params, &size, &dir);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to get period size for playback: %s\n", snd_strerror(err));
		return err;
	}
	period_size = size;
	// write the parameters to device
	err = snd_pcm_hw_params(handle, params);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to set hw params for playback: %s\n", snd_strerror(err));
		return err;
	}
	return 0;
}

/**
*   @brief  Set software parameters
*   @param  handle	a pointer to a snd_pcm_t struct
*	@param	swparams a pointer to a snd_pcm_sw_params_t struct
*   @return 0 if setting done OK; non zero otherwise.
*/
int AlsaHandler::set_sw_params(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams)
{
	int err;
	// get the current swparams
	err = snd_pcm_sw_params_current(handle, swparams);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to determine current swparams for playback: %s\n", snd_strerror(err));
		return err;
	}
	// start the transfer when the buffer is almost full:
	// (buffer_size / avail_min) * avail_min
	err = snd_pcm_sw_params_set_start_threshold(handle, swparams, (buffer_size / period_size) * period_size);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to set start threshold mode for playback: %s\n", snd_strerror(err));
		return err;
	}
	// allow the transfer when at least period_size samples can be processed
	// or disable this mechanism when period event is enabled (aka interrupt like style processing)
	err = snd_pcm_sw_params_set_avail_min(handle, swparams, period_event ? buffer_size : period_size);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to set avail min for playback: %s\n", snd_strerror(err));
		return err;
	}
	// enable period events when requested
	if (period_event) 
	{
		err = snd_pcm_sw_params_set_period_event(handle, swparams, 1);
		if (err < 0) 
		{
			fprintf(stderr, "ALSA: Unable to set period event: %s\n", snd_strerror(err));
			return err;
		}
	}
	// write the parameters to the playback device
	err = snd_pcm_sw_params(handle, swparams);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to set sw params for playback: %s\n", snd_strerror(err));
		return err;
	}
	return 0;
}

/**
*   @brief  Prepare the area data mono
*   @param  areas	a pointer to a snd_pcm_channel_area_t struct
*	@param	offset	a snd_pcm_uframes_t unsigned long offset
*	@param	data	a pointer to the audio data (float)
*	@param	size	data size
*   @return 0 if setting done OK; non zero otherwise.
*/
int AlsaHandler::prepare_area_data(
	const snd_pcm_channel_area_t *areas, 
	snd_pcm_uframes_t offset,
	float *data,
	unsigned int size) 
{	
	
	float *samples;
	int i, steps, res;
									  
	union 
	{
		float f;
		int i;
	} fval;
	
	// verify and prepare the contents of areas 
	if ((areas->first % 8) != 0) 
	{
		fprintf(stderr, "ALSA: areas.first == %u, aborting...\n", areas->first);
		exit(EXIT_FAILURE);
	}
                
	samples = ((float *)areas->addr) + (areas->first / 8);
	if ((areas->step % 16) != 0) 
	{
		fprintf(stderr, "ALSA: areas.step == %u, aborting...\n", areas->step);
		exit(EXIT_FAILURE);
	}
                
	steps = areas->step / 8;
	samples += offset * steps;
	
	// fill the channel areas 
	while (size-- > 0) 
	{		
		fval.f = *data;
		res = fval.i;
		// Copy data in native endian format 
		if (big_endian) 
		{
			for (i = 0; i < bps; i++) 
			{
				*(samples + phys_bps - 1 - i) = (res >> i * 8) & 0xff;
			}
		} 
		else {
			for (i = 0; i < bps; i++)
			{
				*(samples + i) = (res >>  i * 8) & 0xff;
			}
		}
		
		samples += steps;
		data++;
	}

	return 0;
}

/**
*   @brief  Prepare the area data stereo
*   @param  areas	a pointer to a snd_pcm_channel_area_t struct
*	@param	offset	a snd_pcm_uframes_t unsigned long offset
*	@param	dataL	a pointer to the Left channel audio data (float)
*	@param	dataR	a pointer to the Right channel audio data (float)
*	@param	instep	data step for interleaved only
*	@param	size	data size
*   @return 0 if setting done OK; non zero otherwise.
*/
int AlsaHandler::prepare_area_data(
	const snd_pcm_channel_area_t *areas, 
	snd_pcm_uframes_t offset,
	float *data_L,
	float *data_R,
	unsigned int instep,
	unsigned int size) 
{	
	unsigned char  *samples[channels];
	int i, index, res;
	int steps[channels];
	unsigned int chn;
									
	// verify and prepare the contents of areas
	for (chn = 0; chn < channels; chn++) 
	{
		if ((areas[chn].first % 8) != 0) 
		{
			fprintf(stderr, "ALSA: areas[%u].first == %u, aborting...\n", chn, areas[chn].first);
			exit(EXIT_FAILURE);
		}
		
		samples[chn] = ((unsigned char  *)areas[chn].addr) + (areas[chn].first / 8);
		if ((areas[chn].step % 16) != 0) 
		{
			fprintf(stderr, "ALSA: areas[%u].step == %u, aborting...\n", chn, areas->step);
			exit(EXIT_FAILURE);
		}
                
		steps[chn] = areas[chn].step / 8;
		samples[chn] += offset * steps[chn];
	}
	
	// fill the channel areas
	for (index = 0; index < size; index++)
	{
		for (chn = 0; chn < channels; chn++) 
		{
			// Copy data in native endian format 
			if (big_endian) 
			{
				for (i = 0; i < bps; i++) 
				{
					if (chn == 0) 
					{
						res = (int16_t)(data_L[index] * 30000);
						*(samples[chn] + phys_bps - 1 - i) = (res >> i * 8) & 0xff;
					}
					else 
					{
						res = (int16_t)(data_R[index] * 30000);
						*(samples[chn] + phys_bps - 1 - i) = (res >> i * 8) & 0xff;
					}
				}
			}				
			else {
				for (i = 0; i < bps; i++) 
				{ 
					if (chn == 0) 
					{
						res = (int16_t)(data_L[index] * 30000);
						*(samples[chn] + i) = (res >>  i * 8) & 0xff;
					}
					else 
					{
						res = (int16_t)(data_R[index] * 30000);
						*(samples[chn] + i) = (res >>  i * 8) & 0xff;
					}
				}
			}
		
			samples[chn] += steps[chn];			
		}
	}	
	
	return 0;
}

/**
*   @brief  Underrun and suspend recovery
*   @param  handle	a pointer to a snd_pcm_t struct
*	@param	err		error number
*   @return 0 if setting done OK; non zero otherwise.
*/
int AlsaHandler::xrun_recovery(snd_pcm_t *handle, int err)
{
	if (verbose)
	{
		fprintf(stderr, "stream recovery\n");
	}
	if (err == -EPIPE) 
	{	
		// under-run
		err = snd_pcm_prepare(handle);
		if (err < 0)
		{                
			fprintf(stderr, "ALSA: Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
		}
		return 0;
	} 
	else if (err == -ESTRPIPE) 
	{
		while ((err = snd_pcm_resume(handle)) == -EAGAIN)
		{
			// wait until the suspend flag is released
			sleep(1); 
		}
		        
		if (err < 0) 
		{
			err = snd_pcm_prepare(handle);
			if (err < 0)
			{
				fprintf(stderr, "ALSA: Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
			}
		}
		return 0;
	}
	
	return err;
}

/**
*   @brief  Initialize
*   @param  none
*   @return void
*/
int AlsaHandler::init() {
	
	int err;
	snd_pcm_t *handle;
	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_sw_params_t *sw_params;
	signed short *samples;
	unsigned int chn;
	snd_pcm_channel_area_t *areas;
	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_sw_params_alloca(&sw_params);
	
	static snd_output_t *output = NULL;	
	
	err = snd_output_stdio_attach(&output, stdout, 0);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Output failed: %s\n", snd_strerror(err));
		return 0;
	}
	// Open PCM. The last parameter of this function is the mode. 
    // If this is set to 0, the standard mode is used. Possible   
    // other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.        
    // If SND_PCM_NONBLOCK is used, read / write access to the    
    // PCM device will return immediately. If SND_PCM_ASYNC is    
    // specified, SIGIO will be emitted whenever a period has     
    // been completely processed by the soundcard.                
	if ((err = snd_pcm_open(&handle, device, stream, 0)) < 0) 
	{
		fprintf(stderr, "ALSA: Playback open error: %s\n", snd_strerror(err));
		return 0;
	}
	
	if ((err = set_hw_params(handle, hw_params, tmethod.access)) < 0) 
	{
		fprintf(stderr, "ALSA: Setting of hw_params failed: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
	
	if ((err = set_sw_params(handle, sw_params)) < 0) 
	{
		fprintf(stderr, "ALSA: Setting of sw_params failed: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
	if (verbose > 0)
	{	
		snd_pcm_dump(handle, output);
	}
	
	samples = (signed short*)malloc((period_size * channels * snd_pcm_format_physical_width(format)) / 8);
	if (samples == NULL) 
	{
		fprintf(stderr, "ALSA: No enough memory\n");
		exit(EXIT_FAILURE);
	}
        
	areas = (snd_pcm_channel_area_t*)calloc(channels, sizeof(snd_pcm_channel_area_t));
	if (areas == NULL) 
	{
		fprintf(stderr, "ALSA: Not enough memory\n");
		exit(EXIT_FAILURE);
	}
	
	for (chn = 0; chn < channels; chn++) 
	{
		areas[chn].addr = samples;
		areas[chn].first = chn * snd_pcm_format_physical_width(format);
		areas[chn].step = channels * snd_pcm_format_physical_width(format);
	}	
	
	my_areas = (snd_pcm_channel_area_t*)calloc(channels, sizeof(snd_pcm_channel_area_t));		
	my_samples = (unsigned char*)malloc((period_size * channels * snd_pcm_format_physical_width(format)) / 8);
	
	for (chn = 0; chn < channels; chn++) 
	{
		my_areas[chn].first = chn * snd_pcm_format_physical_width(format);
		my_areas[chn].step = areas[chn].step;
		my_areas[chn].addr = my_samples;
	}
		
	err = tmethod.transfer_loop(handle, samples, areas);
	if (err < 0) 
	{	
		fprintf(stderr, "ALSA: Transfer failed: %s\n", snd_strerror(err));
	}
	
	free(areas);
	free(samples);
	snd_pcm_close(handle);	
	
	return 0;
}

/**
*   @brief  Generate a block of silence
*   @param  none
*   @return void
*/
void AlsaHandler::generate_silence(const snd_pcm_channel_area_t *areas, 
	snd_pcm_uframes_t offset,
	int count) {
	
	unsigned char *samples[channels];
	int steps[channels];
	unsigned int chn;
	int format_bits = snd_pcm_format_width(format);
	int bps = format_bits / 8; // bytes per sample
	int i;		
	// verify and prepare the contents of areas
	for (chn = 0; chn < channels; chn++) 
	{
		if ((areas[chn].first % 8) != 0) 
		{
			fprintf(stderr, "ALSA: areas[%u].first == %u, aborting...\n", chn, areas[chn].first);
			exit(EXIT_FAILURE);
		}
		samples[chn] = /*(signed short *)*/(((unsigned char *)areas[chn].addr) + (areas[chn].first / 8));
		if ((areas[chn].step % 16) != 0) 
		{
			fprintf(stderr, "ALSA: areas[%u].step == %u, aborting...\n", chn, areas[chn].step);
			exit(EXIT_FAILURE);
		}
		steps[chn] = areas[chn].step / 8;
		samples[chn] += offset * steps[chn];
	}
	// fill the channel areas
	while (count-- > 0) {
		for (chn = 0; chn < channels; chn++) 
		{
			for (i = 0; i < bps; i++) 
			{
				*(samples[chn] + i) = 0;
			}
		}
	}                                    
}


/**
*   @brief  Generate a block of sine wave
*   @param  areas	a pointer to a snd_pcm_channel_area_t struct
*   @param	offset	snd_pcm_uframes_t uint 32 offset value
*   @param	count	number of samples
*   @param	phase	a pointer to a double variable holding the signal phase
*   @param	freqq	sine wave frequency (Hz)
*   @param	onflag	signal is on when true, off otherwise.
*   @return void
*/
void AlsaHandler::generate_sine(const snd_pcm_channel_area_t *areas, 
	snd_pcm_uframes_t offset,
	int count,
	double *_phase,
	double freq,
	bool onflag)
{
	static double max_phase = 2. * M_PI;
	double phase = *_phase;
	double step = max_phase*freq / (double)rate;
	unsigned char *samples[channels];
	int steps[channels];
	unsigned int chn;
	int format_bits = snd_pcm_format_width(format);
	unsigned int max_val = (1 << (format_bits - 1)) - 1;
	int bps = format_bits / 8; /* bytes per sample */
	int phys_bps = snd_pcm_format_physical_width(format) / 8;
	int big_endian = snd_pcm_format_big_endian(format) == 1;
	int to_unsigned = snd_pcm_format_unsigned(format) == 1;
	int is_float = (format == SND_PCM_FORMAT_FLOAT_LE ||
	                format == SND_PCM_FORMAT_FLOAT_BE);
		
	// verify and prepare the contents of areas
	for (chn = 0; chn < channels; chn++) 
	{
		if ((areas[chn].first % 8) != 0) 
		{
			fprintf(stderr, "ALSA: areas[%u].first == %u, aborting...\n", chn, areas[chn].first);
			exit(EXIT_FAILURE);
		}
		samples[chn] = (((unsigned char *)areas[chn].addr) + (areas[chn].first / 8));
		if ((areas[chn].step % 16) != 0) 
		{
			fprintf(stderr, "ALSA: areas[%u].step == %u, aborting...\n", chn, areas[chn].step);
			exit(EXIT_FAILURE);
		}
		steps[chn] = areas[chn].step / 8;
		samples[chn] += offset * steps[chn];
	}
	// fill the channel areas 
	while (count-- > 0) 
	{
		union 
		{
			float f;
			int i;
		} fval;
		
		int res, i;
		if (is_float) 
		{
			fval.f = sin(phase);
			res = fval.i;
		}
		else
		{	
			res = sin(phase) * max_val;
		}
		
		if (to_unsigned)
		{	
			res ^= 1U << (format_bits - 1);
		}
		if (!onflag)
		{
			res = 0;
		}
	
		for (chn = 0; chn < channels; chn++) 
		{					
			// Generate data in native endian format
			if (big_endian) 
			{
				for (i = 0; i < bps; i++)
				{	
					*(samples[chn] + phys_bps - 1 - i) = (res >> i * 8) & 0xff;
				}
			}
			else 
			{
				for (i = 0; i < bps; i++)
				{	
					*(samples[chn] + i) = (res >>  i * 8) & 0xff;
				}
			}
			
			samples[chn] += steps[chn];
		}
		phase += step;
		if (phase >= max_phase)
		{	
			phase -= max_phase;
		}
	}
	
	*_phase = phase;
}

/**
*   @brief  Transfer callback - asynchronous notification
*   @param  ahandler	a pointer to a snd_async_handler_t struct
*   @return void
*/
void async_callback(snd_async_handler_t *ahandler)
{
	snd_pcm_t *handle = snd_async_handler_get_pcm(ahandler);
	snd_pcm_sframes_t avail;
	int err;
	unsigned int id = 0;
	;
	static unsigned int previd = -1;
	static unsigned int missing = 0;
	
	avail = snd_pcm_avail_update(handle);
	err = errno;
	

	while (avail >= AlsaHandler::get_instance()->period_size) {			
		// Get new data
		AlsaHandler::get_instance()->in_queue.samples_L = 
			(float*)alsa_audio_manager->audio_block_stereo_float_shared_memory_outputs->data[_LEFT];
		AlsaHandler::get_instance()->in_queue.samples_R = 
			(float*)alsa_audio_manager->audio_block_stereo_float_shared_memory_outputs->data[_RIGHT];
		AlsaHandler::get_instance()->in_queue.size = 
			alsa_audio_manager->audio_block_stereo_float_shared_memory_outputs->size;
		
		fprintf(stderr, "async_cb: avial: %i \n", avail);
		
		//		if (AlsaHandler::get_instance()->block_size != alsa_audio_manager->audio_block_stereo_float_shared_memory_outputs->size)
		//		{
		//			fprintf(stderr, "ALSA handler async callback error: block size mismatch\n");
		//			exit(-1);
		//		}
		
		
				// TODO: Update for Multi core !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		AlsaHandler::get_instance()->prepare_area_data(
			AlsaHandler::get_instance()->my_areas,
			0, 
			AlsaHandler::get_instance()->in_queue.samples_L, 
			AlsaHandler::get_instance()->in_queue.samples_R, 
			AlsaHandler::get_instance()->bps,
			/*AlsaHandler::get_instance()->in_queue.size*/
			AlsaHandler::get_instance()->block_size);	
				
		err = snd_pcm_writei(handle, AlsaHandler::get_instance()->my_samples, AlsaHandler::get_instance()->period_size);
		if (err < 0) 
		{
			fprintf(stderr, "ALSA: Write error: %s\n", snd_strerror(err));
			//			exit(EXIT_FAILURE);
		}
		if (err != AlsaHandler::get_instance()->period_size) 
		{
			fprintf(stderr, "ALSA: Write error: written %i expected %li\n", err, AlsaHandler::get_instance()->period_size);
			//			exit(EXIT_FAILURE);
		}
		avail = snd_pcm_avail_update(handle);
		
		previd = id;
	}
}

int async_loop(snd_pcm_t *handle,
	signed short *samples,
	snd_pcm_channel_area_t *areas)
{
	struct async_private_data data;
	snd_async_handler_t *ahandler;
	int err, count;
	data.samples = samples;
	data.areas = areas;
	data.phase = 0;
	err = snd_async_add_pcm_handler(&ahandler, handle, async_callback, &data);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Unable to register async handler\n");
		exit(EXIT_FAILURE);
	}
	// Start with 2 silence  
	for (count = 0; count < 2; count++) {
		AlsaHandler::get_instance()->generate_silence(areas, 0, AlsaHandler::get_instance()->period_size);
		err = snd_pcm_writei(handle, samples, AlsaHandler::get_instance()->period_size);
		if (err < 0) 
		{
			fprintf(stderr, "ALSA: Initial write error: %s\n", snd_strerror(err));
			exit(EXIT_FAILURE);
		}
		if (err != AlsaHandler::get_instance()->period_size) 
		{
			fprintf(stderr, "ALSA: Initial write error: written %i expected %li\n", err, AlsaHandler::get_instance()->period_size);
			exit(EXIT_FAILURE);
		}
	}
	if (snd_pcm_state(handle) == SND_PCM_STATE_PREPARED) 
	{
		err = snd_pcm_start(handle);
		if (err < 0) 
		{
			fprintf(stderr, "ALSA: Start error: %s\n", snd_strerror(err));
			exit(EXIT_FAILURE);
		}
	}
	// because all other work is done in the signal handler, suspend the process
	while (1) 
	{
		sleep(1);
	}
		
	fprintf(stderr, "ALSA: Program terminated @async_loop! Press any key to exit.");
	getchar();
}

/*
 *   Transfer method - asynchronous notification + direct write
 */
void async_direct_callback(snd_async_handler_t *ahandler)
{
	snd_pcm_t *handle = snd_async_handler_get_pcm(ahandler);
	const snd_pcm_channel_area_t *my_areas;
	snd_pcm_uframes_t offset, frames, size;
	snd_pcm_sframes_t avail, commitres;
	snd_pcm_state_t state;
	int first = 0, err;
        
	while (1) 
	{
		state = snd_pcm_state(handle);
		if (state == SND_PCM_STATE_XRUN) 
		{
			err = AlsaHandler::get_instance()->xrun_recovery(handle, -EPIPE);
			if (err < 0) 
			{
				fprintf(stderr, "ALSA: XRUN recovery failed: %s\n", snd_strerror(err));
				exit(EXIT_FAILURE);
			}
			first = 1;
		} 
		else if (state == SND_PCM_STATE_SUSPENDED) 
		{
			err = AlsaHandler::get_instance()->xrun_recovery(handle, -ESTRPIPE);
			if (err < 0) 
			{
				fprintf(stderr, "ALSA: SUSPEND recovery failed: %s\n", snd_strerror(err));
				exit(EXIT_FAILURE);
			}
		}
		avail = snd_pcm_avail_update(handle);
		if (avail < 0) 
		{
			err = AlsaHandler::get_instance()->xrun_recovery(handle, avail);
			if (err < 0) 
			{
				fprintf(stderr, "ALSA: avail update failed: %s\n", snd_strerror(err));
				exit(EXIT_FAILURE);
			}
			first = 1;
			continue;
		}
		if (avail < AlsaHandler::get_instance()->period_size) 
		{
			if (first) 
			{
				first = 0;
				err = snd_pcm_start(handle);
				if (err < 0) 
				{
					fprintf(stderr, "ALSA: Start error: %s\n", snd_strerror(err));
					exit(EXIT_FAILURE);
				}
			} 
			else 
			{
				break;
			}
			continue;
		}
		
		size = AlsaHandler::get_instance()->period_size;
		while (size > 0) 
		{
			frames = size;
			err = snd_pcm_mmap_begin(handle, &my_areas, &offset, &frames);
			if (err < 0) 
			{
				if ((err = AlsaHandler::get_instance()->xrun_recovery(handle, err)) < 0) {
					fprintf(stderr, "ALSA: MMAP begin avail error: %s\n", snd_strerror(err));
					exit(EXIT_FAILURE);
				}
				first = 1;
			}
			// Drive the update process
			commitres = snd_pcm_mmap_commit(handle, offset, frames);
			if (commitres < 0 || (snd_pcm_uframes_t)commitres != frames) 
			{
				if ((err = AlsaHandler::get_instance()->xrun_recovery(handle, commitres >= 0 ? -EPIPE : commitres)) < 0) 
				{
					fprintf(stderr, "ALSA: MMAP commit error: %s\n", snd_strerror(err));
					exit(EXIT_FAILURE);
				}
				first = 1;
			}
			size -= frames;
		}
	}
}

int async_direct_loop(
	snd_pcm_t *handle,
	signed short *samples ATTRIBUTE_UNUSED,
	snd_pcm_channel_area_t *areas ATTRIBUTE_UNUSED)
{
	struct async_private_data data;
	snd_async_handler_t *ahandler;
	const snd_pcm_channel_area_t *my_areas;
	snd_pcm_uframes_t offset, frames, size;
	snd_pcm_sframes_t commitres;
	int err, count;
	data.samples = NULL; /* we do not require the global sample area for direct write */
	data.areas = NULL; /* we do not require the global areas for direct write */
	data.phase = 0;		
	
	err = snd_async_add_pcm_handler(&ahandler, handle, async_direct_callback, &data);
	if (err < 0) {
		fprintf(stderr, "ALSA: Unable to register async handler\n");
		exit(EXIT_FAILURE);
	}

	for (count = 0; count < 2; count++) 
	{
		size = AlsaHandler::get_instance()->period_size;
		while (size > 0) 
		{
			frames = size;
			err = snd_pcm_mmap_begin(handle, &my_areas, &offset, &frames);
			if (err < 0) 
			{
				if ((err = AlsaHandler::get_instance()->xrun_recovery(handle, err)) < 0) 
				{
					fprintf(stderr, "ALSA: MMAP begin avail error: %s\n", snd_strerror(err));
					exit(EXIT_FAILURE);
				}
			}

			AlsaHandler::get_instance()->generate_silence(areas, 0, AlsaHandler::get_instance()->period_size);
			commitres = snd_pcm_mmap_commit(handle, offset, frames);
			if (commitres < 0 || (snd_pcm_uframes_t)commitres != frames) 
			{
				if ((err = AlsaHandler::get_instance()->xrun_recovery(handle, commitres >= 0 ? -EPIPE : commitres)) < 0) 
				{
					fprintf(stderr, "ALSA: MMAP commit error: %s\n", snd_strerror(err));
					exit(EXIT_FAILURE);
				}
			}
	           
			size -= frames;
	           
		}
	}
        
	err = snd_pcm_start(handle);
	if (err < 0) 
	{
		fprintf(stderr, "ALSA: Start error: %s\n", snd_strerror(err));
		exit(EXIT_FAILURE);
	}
	// because all other work is done in the signal handler, suspend the process 
	while (1) 
	{
		sleep(1);
	}
}
