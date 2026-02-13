

/**
* @file		utils.h
*	@author		Nahum Budin
*	@date		30-Oct-2024
*	@version	1.0
*	
*	@brief		Provides the GUI API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

/**
	*   @brief  Returns a logaritmic scaled value. (Used for logarithmic based parameters).
	*   @param  float  min  minimum value of logarithmic range.
	*   @param  float  max  maximum value of logarithmic range.
	*   @param  float  base logarithmic base.
	*   @param  int    input value
	*   @return float  logVal = (float)(min + (max - min)*pow((double)base, (double)in / 50.0) / 100.0).
	*/
float mod_synth_log_scale_100_float(float min, float max, float base, int val);

/**
*   @brief  Returns a logaritmic scaled value. (Used for logarithmic based parameters).
*   @param  int  min  minimum value of logarithmic range.
*   @param  int  max  maximum value of logarithmic range.
*   @param  int  base logarithmic base.
*   @param  int  input value
*   @return int  logVal = (float)(min + (max - min)*pow((double)base, (double)in / 50.0) / 100.0).
*/
int mod_synth_log_scale_100_int(int min, int max, int base, int val);
