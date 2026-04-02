/**
*	@file		Music.h
*	@author		Nahum Budin
*	@date		31-Mar-2026
*	@version	1.0
*					Initial version
*
*	@brief		Music scales handling.
*
*	History:\n
*		version 1.0	6-Aug-2024 Initial
*
*	Based on my Android TilTune Player Java Project 
*
*/

#pragma once

#define _SCALE_MAJOR 0
#define _SCALE_MINOR_NATURAL 1
#define _SCALE_MINOR_HARMONIC 2
#define _SCALE_MINOR_MELODIC 3
#define _SCALE_ALL_NOTES 4

const int scale_intervals[5][28] = {

	{	0, 2, 4, 5,	7, 9, 11, // Major scale 4 intervals
        12, 14, 16, 17, 19, 21, 23,
		24, 26, 28, 29, 31, 33, 35,
		36, 38, 40, 41, 43, 45, 47},

	{	0, 2, 3, 5, 7, 8, 10, // Natural Minor scale
        12, 14, 15, 17, 19, 20, 22,
		24, 26, 27, 29, 31, 32, 34,
		36, 38, 39, 41, 43, 44, 46},

	{	0, 2, 3, 5, 7, 8, 11, // Harmonic Minor scale
        12, 14, 15, 17, 19, 20, 23,
		24, 26, 27, 29, 31, 32, 35,
		36, 38, 39, 41, 43, 44, 47},

	{	0, 2, 3, 5, 7, 9, 11, // Melodic Minor scale
        12, 14, 15, 17, 19, 21, 23,
		24, 26, 27, 29, 31, 33, 35,
		36, 38, 39, 41, 43, 45, 47},

	{	0, 1, 2, 3, 4, 5, 6, // No scale
        7, 8, 9, 10, 11, 12, 13, 14, 
		15, 16, 17, 18, 19, 20, 21, 
		22, 23, 24, 25, 26, 27}
};

const int scales_templates[5][24] = {
	{	1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1}, // Major scale
	{	1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0,
		1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0}, // Natural Minor scale
	{	1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1,
		1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1}, // Harmonic Minor scale
	{	1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // Melodic Minor scale
	{	1, 1, 1, 1,	1, 1 ,1, 1,	1 ,1 ,1 ,1,
		1, 1, 1, 1,	1, 1 ,1, 1,	1 ,1 ,1 ,1}	 // No scale - all notes
};
	
