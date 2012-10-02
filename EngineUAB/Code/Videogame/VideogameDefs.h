#pragma once

#ifndef _VIDEOGAME_DEFS_H_
#define _VIDEOGAME_DEFS_H_

#define INIT_GUI			false

#define WM_GAME_PROCESS		( WM_USER + 1 )
#define WM_GUI_PROCESS		( WM_USER + 2 )
#define WM_EXIT_GAME		( WM_USER + 3 )

#define OPTIONS		COptions::GetSingletonPtr()

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

#endif