//----------------------------------------------------
// CStadistics class
// Author: Marc Cuscullola
//
// Description: Make stadistics about differents aspects of the engine
//----------------------------------------------------

#pragma once
#ifndef _STADISTICS_H
#define _STADISTICS_H

#include "Utils\Types.h"

class CStadistics
{
public:
	//--- Init & End Protocols ---
	CStadistics();
	virtual ~CStadistics();

	//--- Add Methods ----
	void		AddDrawCall					()							{ ++m_uNumOfDrawCalls; }
	void		AddVerticesInScene			( uint32 vertices )			{ m_uNumOfVerticesInScene += vertices; }
	void		AddVerticesInFrustum		( uint32 vertices )			{ m_uNumOfVerticesInFrustum += vertices; }
	void		AddDebugLine				()							{ ++m_uNumOfDebugLines; }

	//--- Reset Methods ---
	void		ResetAll					();
	void		ResetDrawCalls				()							{ m_uNumOfDrawCalls = 0; }
	void		ResetVerticesInScene		()							{ m_uNumOfVerticesInScene = 0; }
	void		ResetVerticesInFrustum		()							{ m_uNumOfVerticesInFrustum = 0; }
	void		ResetDebugLines				()							{ m_uNumOfDebugLines = 0; }

	//--- Get Methods ---
	uint32		GetNumOfDrawCalls			();					
	uint32		GetNumOfVerticesInScene		() const					{ return m_uNumOfVerticesInScene; }
	uint32		GetNumOfVerticesInFrusutm	() const					{ return m_uNumOfVerticesInFrustum; }
	uint32		GetNumOfDebugLines			() const					{ return m_uNumOfDebugLines; }

private:
	uint32		m_uNumOfDrawCalls;
	uint32		m_uNumOfVerticesInScene;
	uint32		m_uNumOfVerticesInFrustum;
	uint32		m_uNumOfDebugLines;
};

#endif

