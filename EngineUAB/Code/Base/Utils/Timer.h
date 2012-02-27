//----------------------------------------------------------------------------------
// CTimer class
// Author: Enric Vergara
//
// Description:
// Contiene toda la informacion relacionada con el tiempo, es decir:
//	- FPS: los frames por segundo a que va la aplicacion
//	- ElapsedTime: el tiempor que ha pasado dsd la ultima llamad a Update
//----------------------------------------------------------------------------------
#pragma once 
#ifndef INC_TIMERMANAGER_H_
#define INC_TIMERMANAGER_H_

#include "Types.h"

class CTimer
{

public:

	CTimer(uint32 avgSamples);
	~CTimer();

	void			Update			();
	float			GetElapsedTime	() const { return m_fElpasedTime; }
	float			GetFPS			() const { return m_fFPS; }
	float			GetTotalTime	() const { return m_fTotalTime; }

private:

	float		m_fElpasedTime;		// ElapsedTime
	float		m_fFPS;				// FPS
	float		m_fTotalTime;
	float		m_fLastFps;
	float		m_fFPSTime;
	double		m_dLastTime;
	uint32		m_uFPSCount;
	float*		m_Deltas;			// array of instant delta times
	uint32		m_uSamples;			// deltas arrays length
	uint32		m_uIndex;			// current array position
};

#endif //INC_TIMERMANAGER_H_