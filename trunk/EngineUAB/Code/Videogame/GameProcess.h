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
	//--- Init and End protocols-----------------------------------------------------
								CGameProcess				( HWND hWnd );
								~CGameProcess				( void );

	//----Main Methods----------------------------------------------------------------
	bool						Init						( void );
	void						CleanUp						( void );
	void						Update						( float elapsedTime );
	void						Render						(  CRenderManager &RM );

	//----Methods --------------------------------------------------------------------
	void	CreatePlayerCamera	( float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name );
	void	CreateFreeCamera	( float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name );

	float	GetTimeBetweenClicks	() const				{ return m_fTimeBetweenClicks; }

private:
	void						RegisterMethods				( void );
	void						RegisterToLuaGameProcess	( lua_State* _pLua );
	void						UpdateInputs				( float _ElapsedTime );
	void						ReloadGameObjects			( void );
	void						LoadGameObjects				( void );
	bool						LoadMainScript				( void );

	//----Properties  --------------------------------------------------------------------
public:
	
	static CGameProcess*		GetGameProcess				( void );
	CCharactersManager*			GetCharactersManager		( void ) const		{ return m_pCharactersManager; }
	CThPSCamera*				GetPlayerCamera				( void ) const		{ return m_pThPSCamera; }

	//----Members --------------------------------------------------------------------
private:
	HWND						m_hWnd;
	bool						m_IsOK;
	float						m_fTimeBetweenClicks;

	CFreeCamera					m_FreeCamera;
	CStaticCamera				m_StaticCamera;
	CThPSCamera					*m_pThPSCamera;
	CThPSCamera					*m_pThPSFreeCamera;
	CCamera						*m_pFreeCamera;

	CCharactersManager			*m_pCharactersManager;
};

#endif

