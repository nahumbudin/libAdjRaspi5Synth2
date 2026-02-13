/**
*	@file		midiAlsaSequrncerEventsHandler.h
*	@author		Nahum Budin
*	@date		12-May-2024
*	@version	1.4
*					1. Code refactoring and notaion.
*					2. Adding Rx Q as a parameter
*					3. Remove commands registration by passing parent instrument as a parameter
*
*	@brief		Recives data from a alsa midi by puling it out of the midi in rx data queue.
*
*	History:
*	version 1.3	???
*	version 1.2 26-Nov-2020
*					1. Add support in Serial Port MIDI input.
*					2. Add ALSA client to support serial port input stream..*
*	version	1.1 13-12-2019(revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 Sep 2018)
*
*/

#include <sys/time.h>

#include "alsaMidiSequencerEventsHandler.h"
#include "alsaMidiSequencerClient.h"
#include "../modSynth.h"
#include "../commonDefs.h"
#include "../Instrument/instrumentMidiMapper.h"

//#define _ALSA_MIDI_HANDLER_PRINTS_ON

AlsaMidiSeqencerEventsHandler::AlsaMidiSeqencerEventsHandler(uint8_t stage, SafeQueue<snd_seq_event_t *> *rxq)
	: MidiStream(0, NULL, stage)
{
	alsa_seq_client_rx_queue = rxq;
	// Enable all midi channels as default.
	active_midi_channels = 0x0;

	// Not connected, so must active here
	this->active = true;
}

void AlsaMidiSeqencerEventsHandler::set_instrument(Instrument *inst)
{
	instrument = inst;
}

void AlsaMidiSeqencerEventsHandler::set_active_midi_channels(uint16_t act_chans)
{
	active_midi_channels = act_chans;
}
uint16_t AlsaMidiSeqencerEventsHandler::get_active_midi_channels()
{
	return active_midi_channels;
}

void AlsaMidiSeqencerEventsHandler::channel_is_active_updater(int chan)
{
	if (InstrumentMidiMapper::midi_channel_active_indication_callback_ptr)
	{
		InstrumentMidiMapper::midi_channel_active_indication_callback_ptr(chan, true);
	}
}

void AlsaMidiSeqencerEventsHandler::update()
{
	snd_seq_event_t* qev = NULL;
	bool timeout, blocking = false;
	uint8_t sysex_message[512];
	int i;

	timeval time_val;

	qev = alsa_seq_client_rx_queue->dequeue(0, &timeout, &blocking); //// Non blocking get from Q
	while (qev != NULL)
	{
		if (CHECK_BIT(active_midi_channels, qev->data.control.channel) == 1)
		{
			// Activate only on active midi channels
			switch (qev->type)
			{
			case SND_SEQ_EVENT_NOTEON:

				instrument->note_on_handler(qev->data.control.channel, qev->data.note.note, qev->data.note.velocity);

				gettimeofday(&time_val, NULL);

				#ifdef _ALSA_MIDI_HANDLER_PRINTS_ON
				
				fprintf(stderr,
						"ALSA: Note On  event on Channel %2d: %5d   %6d.%6d      \n",
						qev->data.control.channel,
						qev->data.note.note,
						time_val.tv_sec,
						time_val.tv_usec);
				#endif
				
				if (update_active_midi_channels_on_channel_change)
				{
					channel_is_active_updater(qev->data.control.channel);
				}

				break;

			case SND_SEQ_EVENT_NOTEOFF:

				instrument->note_off_handler(qev->data.control.channel, qev->data.note.note, qev->data.note.velocity);

				gettimeofday(&time_val, NULL);

				#ifdef _ALSA_MIDI_HANDLER_PRINTS_ON
				fprintf(stderr,
						"ALSA: Note Off event on Channel %2d: %5d   %6d.%6d      \n",
						qev->data.control.channel,
						qev->data.note.note,
						time_val.tv_sec,
						time_val.tv_usec);
				#endif
				
				if (update_active_midi_channels_on_channel_change)
				{
					channel_is_active_updater(qev->data.control.channel);
				}

				break;

			case SND_SEQ_EVENT_PGMCHANGE:

				instrument->change_program_handler(qev->data.control.channel, qev->data.control.value);

				/** TODO: callback_midi_program_change_event(qev->data.control.channel, qev->data.control.value); */
				#ifdef _ALSA_MIDI_HANDLER_PRINTS_ON
				fprintf(stderr,
						"ALSA: Change program event on Channel %2d: %5d      \n",
						qev->data.control.channel,
						qev->data.control.value);
				#endif
				
				if (update_active_midi_channels_on_channel_change)
				{
					channel_is_active_updater(qev->data.control.channel);
				}
				
				break;

			case SND_SEQ_EVENT_CHANPRESS:
				instrument->channel_pressure_handler(qev->data.control.channel, qev->data.control.value);
				#ifdef _ALSA_MIDI_HANDLER_PRINTS_ON
				fprintf(stderr,
						"ALSA: Channel pressure event on Channel %2d: %5d\n",
						qev->data.control.channel,
						qev->data.control.value);
				#endif
				
				if (update_active_midi_channels_on_channel_change)
				{
					channel_is_active_updater(qev->data.control.channel);
				}

				break;

			case SND_SEQ_EVENT_CONTROLLER:

				instrument->controller_event_handler(qev->data.control.channel, qev->data.control.param, qev->data.control.value);

				#ifdef _ALSA_MIDI_HANDLER_PRINTS_ON
				fprintf(stderr,
					"ALSA: Control event on Channel %2d: %5d  %5d\n",
						qev->data.control.channel,
						qev->data.control.param,
						qev->data.control.value);
				#endif
				
				if (update_active_midi_channels_on_channel_change)
				{
					channel_is_active_updater(qev->data.control.channel);
				}

				break;

			case SND_SEQ_EVENT_PITCHBEND:
				instrument->pitch_bend_handler(qev->data.control.channel, qev->data.control.value);

				#ifdef _ALSA_MIDI_HANDLER_PRINTS_ON
				fprintf(stderr,
						"ALSA: Pitchbender event on Channel %2d: %5d   \n",
						qev->data.control.channel,
						qev->data.control.value);
				#endif
				
				if (update_active_midi_channels_on_channel_change)
				{
					channel_is_active_updater(qev->data.control.channel);
				}

				break;

			case SND_SEQ_EVENT_SYSEX:

				if ((qev->data.ext.ptr != NULL) && (qev->data.ext.len >= 7)) // min: f0 + 3xvendor id + command + value1 + f7
				{

					fprintf(stderr, "ALSA: Sysex event: ");

					for (i = 0; i < qev->data.ext.len; i++)
					{
						sysex_message[i] = *((uint8_t *)qev->data.ext.ptr + i);
						fprintf(stderr, "%x ", sysex_message[i]);
					}
					fprintf(stderr, "\n");

					instrument->sysex_handler(&sysex_message[0], qev->data.ext.len);
				}
				
				if (update_active_midi_channels_on_channel_change)
				{
					channel_is_active_updater(qev->data.control.channel);
				}

				break;
			}
		}

		delete qev;
		qev = alsa_seq_client_rx_queue->dequeue(0, &timeout, &blocking);
	}
}

/**
void alsa_seq_sysex(uint8_t* message, int len)
{
	uint8_t val1, val2;

	if ((message[0] == _MIDI_SYSEX_START) && (message[len - 1] == _MIDI_SYSEX_END))
	{
		// Verify once again it is a sysex message (just in case)
		if ((message[1] == _MIDI_TILTUNE_SYSEX_VENDOR_ID_0) &&
			(message[2] == _MIDI_TILTUNE_SYSEX_VENDOR_ID_1) &&
			(message[3] == _MIDI_TILTUNE_SYSEX_VENDOR_ID_2))
		{
			// Verified TilTune sysex message
			val1 = *(message + 5);
			val2 = *(message + 6);

			switch (message[4])
			{
			case _MIDI_SYSEX_RASPI_OFF_MESSAGE_ID_1:
				if ((message[5] == _MIDI_SYSEX_RASPI_OFF_MESSAGE_ID_2) && (message[6] == 0))
				{
					// Turn off raspi
					system("Sysex: sudo poweroff");
				}

				break;

			case _MIDI_SYSEX_AUX_KNOB_1_MESSAGE_ID:
				fprintf(stderr, "Sysex: Knob: %i\n", val1);
				// Updating UI application
				if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id() != -1) &&
					(ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id() != -1))
				{
					if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type() == _CONTROL_TYPE_DIAL) ||
						(ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type() == _CONTROL_TYPE_SLIDER))
					{
						ModSynth::get_instance()->execute_synth_control(ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id(),
							ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id(),
							val1);
					}
				}

				break;


			case _MIDI_SYSEX_AUX_KNOB_2_MESSAGE_ID:

				break;

			case _MIDI_SYSEX_AUX_KNOB_1_SW_MESSAGE_ID:
				fprintf(stderr, "Knob Switch: %i\n", val1);

				if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id() != -1) &&
					(ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id() != -1))
				{
					// TOD: handle control switc for select actions
				}

				break;

			case _MIDI_SYSEX_AUX_KNOB_2_SW_MESSAGE_ID:

				break;

			case _MIDI_SYSEX_AUX_SLIDER_1_MESSAGE_ID:
				fprintf(stderr, "Sysex: Slider: %i\n", (val1 & 0x7f) + ((uint16_t)val2 << 7));

				if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id() != -1) &&
					(ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id() != -1))
				{
					if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type() == _CONTROL_TYPE_DIAL) ||
						(ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type() == _CONTROL_TYPE_SLIDER))
					{
						ModSynth::get_instance()->execute_synth_control(ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id(),
							ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id(),
							((val1 & 0x7f) + ((uint16_t)val2 << 7)) / 10); // 0-1350 -> 0-127
					}
				}

				break;

			case _MIDI_SYSEX_AUX_SLIDER_2_MESSAGE_ID:

				break;
			}

		}
	}
}
*/
