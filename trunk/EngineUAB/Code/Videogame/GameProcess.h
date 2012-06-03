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
#include "Cameras\FreeCamera.h"
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

	bool		Init				();
	void		Update				( float elapsedTime );
	void		Render				(  CRenderManager &RM );

	void	CreatePlayerCamera	( float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name );

	static CGameProcess*	GetGameProcess		();
	CCharactersManager*		GetCharacterManager () const		{ return m_pCharacterManager; }
	CThPSCamera*			GetPlayerCamera		() const		{ return m_pThPSCamera; }

private:
	void		RegisterMethods			();
	void		LoadGameObjects			();
	bool		LoadMainScript			();

private:
	bool				m_IsOK;

	CFreeCamera			m_FreeCamera;
	CStaticCamera		m_StaticCamera;
	CThPSCamera			*m_pThPSCamera;
	CThPSCamera			*m_pThPSFreeCamera;
	CCamera				*m_pFreeCamera;

	CCharactersManager	*m_pCharacterManager;

	HWND	m_hWnd;
};

#endif

