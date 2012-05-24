//----------------------------------------------------------------------------------
// CGameProcess class
// Author: Marc Cuscullola
//
// Description:
// Proceso que se encarga de gestionar el juego
//----------------------------------------------------------------------------------
#pragma once

#ifndef __GAME_PROCESS_TEST_BEHAVIOUR_H__
#define __GAME_PROCESS_TEST_BEHAVIOUR_H__

#include <Windows.h>
#include "EngineProcess.h"
#include "Cameras\StaticCamera.h"
#include "Cameras\FreeCamera.h"

// --- Foward Declaractions ---
class CRenderManager;
class CThPSCamera;
class CScene;
class CPlayer; 
class CCharactersManager;
class CPropertiesManager;
// ----------------------------

class CGameProcess : public CEngineProcess
{
public:
	//--- Init and End protocols-----------------------------------------------------
								CGameProcess	( HWND hWnd );
								~CGameProcess	( void );

	//----Main Methods----------------------------------------------------------------
	bool						Init			( void );
	void						Update			( float elapsedTime );
	void						Render			(  CRenderManager &RM );

	//----Methods --------------------------------------------------------------------
private:
	void						RegisterMethods ( void );
	void						UpdateInputs	( float _ElapsedTime );

	//----Members --------------------------------------------------------------------
private:
	HWND						m_hWnd;
	Vect2i						pos;
	Vect2i						screen;

	CCharactersManager			*m_pCharactersManager;
	CScene						*m_pScene;
	CThPSCamera					*m_pThPSCamera;
	CThPSCamera					*m_pThPSFreeCamera;
	CStaticCamera				m_StaticCamera;

	CPlayer						*m_Player;
	CFreeCamera					m_FreeCamera;
	CCamera						*m_pFreeCamera;
	
	bool						m_AudioIsFade;
	uint32						m_uAudioID;
};

#endif __GAME_PROCESS_TEST_BEHAVIOUR_H__