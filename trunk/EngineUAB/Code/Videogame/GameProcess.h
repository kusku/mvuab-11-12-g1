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
class CCharactersManager;
class CCharacter;
class CThPSCharacterCamera;
class CThPSCamera;
class CWeaponManager;
class CAnimationCallbackManager;

class CGameProcess : public CEngineProcess
{
public:
	//--- Init and End protocols-----------------------------------------------------
								CGameProcess				( HWND hWnd );
								~CGameProcess				();

	//----Main Methods----------------------------------------------------------------
	bool						Init						();
	void						CleanUp						();
	void						Update						( float elapsedTime );
	void						Render						(  CRenderManager &RM );

	//----Methods --------------------------------------------------------------------
	void	CreatePlayerCamera	( float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name );
	void	CreateFreeCamera	( float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name );

	void			ChangeToGUIProcess		();

	float	GetTimeBetweenClicks	() const				{ return m_fTimeBetweenClicks; }

	static void					RegisterMethods				();

private:
	void						RegisterToLuaGameProcess	( lua_State* _pLua );
	void						UpdateInputs				( float _ElapsedTime );
	void						ReloadGameObjects			();
	void						LoadGameObjects				();
	bool						LoadMainScript				();

	//----Properties  --------------------------------------------------------------------
public:
	
	static CGameProcess*		GetGameProcess				();
	CCharactersManager*			GetCharactersManager		() const		{ return m_pCharactersManager; }
	CThPSCharacterCamera*		GetPlayerCamera				() const		{ return m_pThPSCamera; }
	CAnimationCallbackManager*	GetAnimationCallbackManager	() const		{ return m_pAnimationCallbackManager; }
	CCharacter*					GetPlayer					() const		{ return GetCharactersManager()->GetPlayer(); }

	//----Members --------------------------------------------------------------------
private:
	HWND						m_hWnd;
	bool						m_IsOK;
	float						m_fTimeBetweenClicks;

	CFreeCamera					m_FreeCamera;
	CStaticCamera				m_StaticCamera;
	CThPSCharacterCamera		*m_pThPSCamera;
	CThPSCamera					*m_pThPSFreeCamera;
	CCamera						*m_pFreeCamera;

	CCharactersManager			*m_pCharactersManager;
	CWeaponManager				*m_pWeaponManager;
	CAnimationCallbackManager	*m_pAnimationCallbackManager;
};

#endif

