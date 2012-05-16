//----------------------------------------------------------------------------------
// CDebugRender class
// Author: Marc Cuscullola
//
// Description:
// Clase encargada de pintar por pantalla la informacion de debug
//----------------------------------------------------------------------------------
#pragma once

#ifndef _DEBUGRENDER_H
#define _DEBUGRENDER_H

#include "Math\Color.h"
#include "Math\Vector2.h"

//---Forward Declarations---
class CRenderManager;
class CFontManager;
class CTimer;
//--------------------------

class CDebugRender
{
public:
	CDebugRender();
	~CDebugRender();

	void					Init						();
	void					Render						( CRenderManager *rm, CFontManager *fm, CTimer *timer, CColor color = colWHITE );

	//Set & Get Methods-------------------
	void				SetVisible					( bool flag )		{ m_bIsVisible = flag; }
	bool				GetVisible					( void ) const			{ return m_bIsVisible; }

	bool				GetFPSVisible				( void ) const			{ return m_bFPSVisible; }
	bool				GetDeltaTimeVisible			( void ) const			{ return m_bDeltaTimeVisible; }
	bool				GetGamePadVisible			( void ) const			{ return m_bGamePadVisible; }
	bool				GetCameraVisible			( void ) const			{ return m_bCameraVisible; }

	void				SetFPSVisible				( bool visible )	{ m_bFPSVisible = visible; AnalizeSizeInfo(); }
	void				SetDeltaTimeVisible			( bool visible )	{ m_bDeltaTimeVisible = visible; AnalizeSizeInfo(); }
	void				SetGamePadVisible			( bool visible )	{ m_bGamePadVisible = visible; AnalizeSizeInfo(); }
	void				SetCameraVisible			( bool visible )	{ m_bCameraVisible = visible; AnalizeSizeInfo(); }
	
	//Toggle Methods----------------------
	void				ToggleFPS					()					{ m_bFPSVisible = !m_bFPSVisible; AnalizeSizeInfo(); }
	void				ToggleDeltaTime				()					{ m_bDeltaTimeVisible = !m_bDeltaTimeVisible; AnalizeSizeInfo(); }
	void				ToggleGamePad				()					{ m_bGamePadVisible = !m_bGamePadVisible; AnalizeSizeInfo(); }
	void				ToggleCamera				()					{ m_bCameraVisible = !m_bCameraVisible; AnalizeSizeInfo(); }

private:
	void AnalizeSizeInfo ();

	bool					m_bIsVisible;
	Vect2i					m_SizeRectangle;

	bool					m_bFPSVisible;
	bool					m_bDeltaTimeVisible;
	bool					m_bGamePadVisible;
	bool					m_bCameraVisible;
};

#endif

