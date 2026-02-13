/**
*	@file		adjSynthSettingsCallbacksReverb.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.2
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle Distortion Settings
*	settings
*
*	History:\n
*	
*	version 1.1		30_Nov-2019
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int set_reverb3m_preset_cb(int pset, int prog)
{
	sf_presetreverb(&AdjSynth::get_instance()->audio_reverb->rv3m, 
		AdjSynth::get_instance()->get_sample_rate(), 
		(sf_reverb_preset)pset);
	return 0;
}

int set_reverb_room_size_cb(int rs, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_room_size(rs);
	return 0;
}

int set_reverb_damp_cb(int dmp, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_damp(dmp);
	return 0;
}

int set_reverb_wet_cb(int wt, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_wet(wt);
	return 0;
}

int set_reverb_dry_cb(int dry, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_dry(dry);
	return 0;
}

int set_reverb_width_cb(int wd, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_width(wd);
	return 0;
}

int set_reverb_mode_cb(int md, int prog)
{
	AdjSynth::get_instance()->audio_reverb->reverb->set_mode(md);
	return 0;
}

int set_reverb3m_enable_state_cb(bool en, int prog)
{
	if (en)
	{
		AdjSynth::get_instance()->audio_reverb->rev3m_enable();
	}
	else
	{
		AdjSynth::get_instance()->audio_reverb->rev3m_disable();
	}
	return 0;
}

int set_reverb_enable_state_cb(bool en, int prog)
{
	if (en)
	{
		AdjSynth::get_instance()->audio_reverb->rev_enable();
	}
	else
	{
		AdjSynth::get_instance()->audio_reverb->rev_disable();
	}
	return 0;
}
