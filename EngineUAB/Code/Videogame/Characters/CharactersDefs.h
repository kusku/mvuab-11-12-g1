#pragma once

#ifndef _CHARACHTERS_DEFS_H_
#define _CHARACHTERS_DEFS_H_

const float VELOCIDAD_MINIMA	= 1.0f;
const float VELOCIDAD_MAXIMA	= 3.0f;
const float FUERZA_MAXIMA		= 0.15f;
const float ACELERACION_MINIMA	= 1.f;
const float ACELERACION_MAXIMA	= 2.f;
const float ROTACION_MAXIMA		= 360.f;

enum eCharacterTypes
{
	WOLF = 1,
	RABBIT = 2,
	DEER = 3
};

#endif //_CHARACHTERS_DEFS_H_