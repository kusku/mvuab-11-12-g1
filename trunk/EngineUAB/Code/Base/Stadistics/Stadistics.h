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
	void		AddVerticesInFrustum		( uint32 vertices )			{ m_uNumOfVerticesInFrustum += vertices; }
	void		AddDebugLine				()							{ ++m_uNumOfDebugLines; }
	void		AddTriangles				( uint32 triangles )		{ m_uNumOfTrianglesInFrustum += triangles; }

	//--- Reset Methods ---
	void		ResetAll					();
	void		ResetDrawCalls				()							{ m_uNumOfDrawCalls = 0; }
	void		ResetVerticesInFrustum		()							{ m_uNumOfVerticesInFrustum = 0; }
	void		ResetDebugLines				()							{ m_uNumOfDebugLines = 0; }
	void		ResetTrianglesInFrustum		()							{ m_uNumOfTrianglesInFrustum = 0; }

	//--- Get Methods ---
	inline uint32		GetNumOfDrawCalls			() const					{ return m_uNumOfDrawCalls; }		
	inline uint32		GetNumOfVerticesInFrustum	() const					{ return m_uNumOfVerticesInFrustum; }
	inline uint32		GetNumOfDebugLines			() const					{ return m_uNumOfDebugLines; }
	inline uint32		GetNumOfTrianglesInFrustum	() const					{ return m_uNumOfTrianglesInFrustum; }

private:
	uint32		m_uNumOfDrawCalls;
	uint32		m_uNumOfVerticesInFrustum;
	uint32		m_uNumOfDebugLines;
	uint32		m_uNumOfTrianglesInFrustum;
};

#endif

