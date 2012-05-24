//----------------------------------------------------------------------------------
// CGameProcess class
// Author: Marc Cuscullola
//
// Description:
// Proceso que se encarga de gestionar el juego
//----------------------------------------------------------------------------------
#pragma once

#ifndef _GAME_PROCESS_H_
#define _GAME_PROCESS_H_

#include "EngineProcess.h"
#include "Cameras\StaticCamera.h"
#include "Characters\CharacterManager.h"
#include <Windows.h>

class CRenderManager;
class CCHaractersManager;
class CThPSCamera;

class CGameProcess : public CEngineProcess
{
public:
	CGameProcess( HWND hWnd );
	~CGameProcess();

	bool Init	();
	void Update ( float elapsedTime );
	void Render (  CRenderManager &RM );

	static CGameProcess*	GetGameProcess ();
	CCharactersManager*	GetCharacterManager () const		{ return m_pCharacterManager; }

private:
	void RegisterMethods ();

private:
	CStaticCamera		m_StaticCamera;
	CThPSCamera			*m_pThPSCamera;

	CCharactersManager	*m_pCharacterManager;

	HWND	m_hWnd;
};

#endif

