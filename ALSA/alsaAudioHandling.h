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

#pragma once

#include <alsa/asoundlib.h> 

/**   Transfer method - asynchronous notification */
struct async_private_data 
{
	signed short *samples;
	snd_pcm_channel_area_t *areas;
	double phase;
};

struct transfer_method 
{
	const char *name;
	snd_pcm_access_t access;
	int(*transfer_loop)(snd_pcm_t *handle,
		signed short *samples,
		snd_pcm_channel_area_t *areas);
};

typedef struct input_data 
{
	unsigned int id;
	unsigned int size;
	float *samples_L;
	float *samples_R;
} input_data_t;

class AlsaHandler 
{
public:	
	~AlsaHandler(); 
	
	static AlsaHandler *get_instance();
	
	int set_sample_rate(int samp_rate);
	int set_audio_block_size(int size);
	int set_period_time_us(unsigned long ptu);

	//private: 
	
	int set_hw_params(
		snd_pcm_t *handle,
		snd_pcm_hw_params_t *params,
		snd_pcm_access_t access);	
	int set_sw_params(snd_pcm_t *handle, 
		snd_pcm_sw_params_t *sw_params);
	int xrun_recovery(snd_pcm_t *handle, int err);
	int init();
	
	int prepare_area_data(
		const snd_pcm_channel_area_t *areas, 
		snd_pcm_uframes_t offset,
		float *data,
		unsigned int size);
	
	int prepare_area_data(
		const snd_pcm_channel_area_t *areas, 
		snd_pcm_uframes_t offset,
		float *data_L,
		float *data_R,
		unsigned int instep,
		unsigned int size);
	
	void generate_silence(
		const snd_pcm_channel_area_t *areas, 
		snd_pcm_uframes_t offset,
		int count);
	
	void generate_sine(
		const snd_pcm_channel_area_t *areas, 
		snd_pcm_uframes_t offset,
		int count,
		double *_phase,
		double freq,
		bool onflag);
	
	void gen_sine(
		float *data,
		int size,
		double *_phase,
		double freq,
		bool onflag);
					   
	/** Queue for input samples */
	input_data_t in_queue;
	/** The audio device */
	char *device;
	/** sample format */
	snd_pcm_format_t format;    
	/** Number of bits per PCM sample */
	int format_bits;
	/** stream rate */
	unsigned int rate; 
	/* Audio block length */
	int block_size;
	/* ring buffer length in us */
	unsigned int buffer_time; 
	/* period time in us */
	unsigned int period_time;
	/** Sample max value */
	unsigned int max_val;
	/** count of channels */
	unsigned int channels;                       
	/** bytes per sample */
	int bps;
	/** bits needed to store a PCM sample */
	int phys_bps;
	/** endian info for a PCM sample format */
	int big_endian;
	/** sign info for a PCM sample linear format */
	int to_unsigned;
	/** format is floating point? */
	int is_float;
	/** verbose flag */
	int verbose; 
	/** enable alsa-lib resampling */
	int resample;
	/* produce poll event after each period */
	int period_event;                            
	
	snd_pcm_sframes_t buffer_size;
	snd_pcm_sframes_t period_size;
	
	snd_pcm_channel_area_t *my_areas;
	unsigned char *my_samples;
	
	bool thread_is_running;
	
private:
	AlsaHandler(); 
	
	static AlsaHandler *alsa_handler;
};


void async_callback(snd_async_handler_t *ahandler);

int async_loop(
	snd_pcm_t *handle,
	signed short *samples,
	snd_pcm_channel_area_t *areas);
	
void async_direct_callback(snd_async_handler_t *ahandler);	
	
int async_direct_loop(
	snd_pcm_t *handle,
	signed short *samples ATTRIBUTE_UNUSED,
	snd_pcm_channel_area_t *areas ATTRIBUTE_UNUSED);

