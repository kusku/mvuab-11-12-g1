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
class CFPSCamera;
class CThPSCamera;
class CWeaponManager;
class CAnimationCallbackManager;
class CHud;

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
	void						DebugRender					(  CRenderManager &RM );

	//----Methods --------------------------------------------------------------------
	CThPSCharacterCamera*		CreatePlayerCamera	( float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name );
	void						CreateFreeCamera	( float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name );
	void						CreateRailCamera	( float _near, float _far );

	void			ChangeToGUIProcess		();

	float	GetTimeBetweenClicks	() const				{ return m_fTimeBetweenClicks; }

	static void					RegisterMethods				();

	//----Properties  --------------------------------------------------------------------
	static CGameProcess*		GetGameProcess				();
	CCharactersManager*			GetCharactersManager		() const		{ return m_pCharactersManager; }
	CThPSCharacterCamera*		GetPlayerCamera				() const		{ return m_pThPSCamera; }
	CAnimationCallbackManager*	GetAnimationCallbackManager	() const		{ return m_pAnimationCallbackManager; }
	CCharacter*					GetPlayer					() const		{ return GetCharactersManager()->GetPlayer(); }
	inline CHud*				GetHUD						() const		{ return m_pHUD; }

	//-----Presentation  -----------------------------------------------------------------
	void		SetIntroFinish		( bool _finish )		{ m_bIntroFinished = _finish; }
	bool		IsIntroFInished		() const				{ return m_bIntroFinished; }
	CObject3D*	GetRailObject		() const				{ return m_pObjectRail; }
	void		AddRailCounter		()						{ ++m_uRailCounter; }
	uint8		GetRailCounter		() const				{ return m_uRailCounter; }

private:
	void		RegisterToLuaGameProcess	( lua_State* _pLua );
	void		UpdateInputs				( float _ElapsedTime );
	void		ReloadGameObjects			();
	void		LoadGameObjects				();
	bool		LoadMainScript				();

	//----Members --------------------------------------------------------------------
private:
	HWND						m_hWnd;
	bool						m_IsOK;
	float						m_fTimeBetweenClicks;

	CFreeCamera					m_FreeCamera;
	CStaticCamera				m_StaticCamera;
	CThPSCharacterCamera		*m_pThPSCamera;
	CThPSCamera					*m_pThPSFreeCamera;
	CFPSCamera					*m_pFPSRailCamera;
	CCamera						*m_pFreeCamera;
	CCamera						*m_pRailCamera;
	CObject3D					*m_pObjectRail;

	// Intro
	bool						m_bIntroFinished;
	uint8						m_uRailCounter;

	bool						m_bStartRails;
	bool						m_bStablishFramerateState;

	float						m_fElapseds[30];
	uint16						m_uElapsedIndex;
	bool						m_bStartAnalise;

	CHud						*m_pHUD;
	CCharactersManager			*m_pCharactersManager;
	CWeaponManager				*m_pWeaponManager;
	CAnimationCallbackManager	*m_pAnimationCallbackManager;
};

#endif

