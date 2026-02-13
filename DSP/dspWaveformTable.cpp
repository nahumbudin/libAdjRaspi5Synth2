/**
*	@file		dspWaveformTable.cpp
*	@author		Nahum Budin
*	@date		21_sep-2024
*	@version	1.2 
*					1. Code refactoring and notaion. 
*					
*	@History	23_Jan-2021 1.1 Code refactoring and notaion. 
*				31-Oct-2019	1.0 (dspWaveformLUT) (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		A Table used for generating sampled floating point waveforsm.
*	
*/

#include <stdlib.h> // NULL

#include "dspWaveformTable.h"

DSP_WaveformTab::DSP_WaveformTab(int len)
{
	waveform_tab = NULL;
	
	set_wtab_length(len);
	init_waveform_table();
}

DSP_WaveformTab::~DSP_WaveformTab()
{
	if (waveform_tab != NULL)
	{
		delete[] waveform_tab;
	}
}

/**
*	@brief	Initialize the waveform table buffer
*	@param	none
*	@return wavform table length
*/
int DSP_WaveformTab::init_waveform_table()
{
	if (waveform_tab != NULL) // == ?
	{
		delete[] waveform_tab;
	}
	
	waveform_tab = new float[get_wtab_length()];
	
	return wtab_length;
}

/**
*	@brief	Set WTAB length
*	@param len WTAB length - must be a power of 2 between _WTAB_MIN_LEN and _WTAB_MAX_LEN
*	@return WTAB length
*/
int DSP_WaveformTab::set_wtab_length(int len)
{
	wtab_length = len;
	
	// Length must be a power of 2
	if (!(wtab_length & (wtab_length - 1)))
	{
		// not a power of 2 - next power of 2 
		wtab_length = (int)pow(2, ceil(log(wtab_length) / log(2)));
	}

	if (wtab_length < _WTAB_MIN_LEN)
	{
		wtab_length = _WTAB_MIN_LEN;
	}
	else if (wtab_length > _WTAB_MAX_LEN)
	{
		wtab_length = _WTAB_MAX_LEN;
	}
	
	return wtab_length;
}

/**
*	@brief	Return WTAB length
*	@param none
*	@return WTAB length
*/
int DSP_WaveformTab::get_wtab_length() 
{ 
	return wtab_length; 
}

/**
*	@brief	Set a WTAB value
*	@param index value location in the WTAB
*	@param val value
*	@return 0 if OK; -1 if index is out of range or wavetable is not initiated
*/
int DSP_WaveformTab::set_wtab_val(int index, float val)
{
	if ((index < 0) || (index >= wtab_length) || (waveform_tab == NULL))
	{
		return -1;
	}
	else
	{
		waveform_tab[index] = val;
		return 0;
	}
}

/**
*	@brief	Return a WTAB value
*	@param index value location in the LUT
*	@return value if OK -99999999.f if  index is out of range or wavetable is not initiated
*/
float DSP_WaveformTab::get_wtab_val(int index)
{	
	float value = -99999999.f;
	
	if ((index >= 0) && (index < wtab_length) && (waveform_tab != NULL))
	{
		value = waveform_tab[index];
	}

	return value;
}

/**
*	@brief	Return a pointer to the WTAB 1st value
*	@param  none
*	@return a pointer to the WTAB 1st value
*/
float *DSP_WaveformTab::get_wtab_ptr() 
{ 
	return &waveform_tab[0]; 
}
