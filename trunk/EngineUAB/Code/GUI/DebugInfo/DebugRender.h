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
#include <string>
#include <map>

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
	bool				GetVisible					() const			{ return m_bIsVisible; }

	bool				GetFPSVisible				() const			{ return m_bFPSVisible; }
	bool				GetDeltaTimeVisible			() const			{ return m_bDeltaTimeVisible; }
	bool				GetGamePadVisible			() const			{ return m_bGamePadVisible; }
	bool				GetCameraVisible			() const			{ return m_bCameraVisible; }
	bool				GetStateVisible				() const			{ return m_bStateVisible; }

	void				SetFPSVisible				( bool visible )	{ m_bFPSVisible = visible; AnalizeSizeInfo(); }
	void				SetDeltaTimeVisible			( bool visible )	{ m_bDeltaTimeVisible = visible; AnalizeSizeInfo(); }
	void				SetGamePadVisible			( bool visible )	{ m_bGamePadVisible = visible; AnalizeSizeInfo(); }
	void				SetCameraVisible			( bool visible )	{ m_bCameraVisible = visible; AnalizeSizeInfo(); }
	void				SetStateVisible				( bool visible )	{ m_bStateVisible = visible; AnalizeSizeInfo(); }

	void				SetStateName				( const std::string& _State) { m_StateName = _State; }
	void				AddEnemyStateName			( const std::string& _EnemyName, std::string _State) 
																		{ 
																			m_EnemyStates[_EnemyName.c_str()] = _State; 
																		}

	//Toggle Methods----------------------
	void				ToggleFPS					()					{ m_bFPSVisible = !m_bFPSVisible; AnalizeSizeInfo(); }
	void				ToggleDeltaTime				()					{ m_bDeltaTimeVisible = !m_bDeltaTimeVisible; AnalizeSizeInfo(); }
	void				ToggleGamePad				()					{ m_bGamePadVisible = !m_bGamePadVisible; AnalizeSizeInfo(); }
	void				ToggleCamera				()					{ m_bCameraVisible = !m_bCameraVisible; AnalizeSizeInfo(); }
	void				ToggleState					()					{ m_bStateVisible = !m_bStateVisible; AnalizeSizeInfo(); }

private:
	void AnalizeSizeInfo ();

	bool								m_bIsVisible;
	Vect2i								m_SizeRectangle;

	bool								m_bFPSVisible;
	bool								m_bDeltaTimeVisible;
	bool								m_bGamePadVisible;
	bool								m_bCameraVisible;
	bool								m_bStateVisible;

	std::string							m_StateName;

	std::map<std::string, std::string>	m_EnemyStates;
	std::string							m_EnemyStateName;
};

#endif

