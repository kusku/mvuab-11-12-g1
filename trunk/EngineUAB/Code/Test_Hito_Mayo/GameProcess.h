#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_

#include "EngineProcess.h"
#include "Characters\Player\Player.h"
#include "Cameras\FreeCamera.h"

class CScene;
class CThPSCamera;
class CCamera;
class CCharactersManager;

class CGameProcess : public CEngineProcess
{
public:
	// ------------- Constructors i Destructors --------------------------
					CGameProcess(HWND hWnd);
					~CGameProcess(void);

	// ------------- Mètodes Principals -----------------------------------
	bool			Init				( void );
	void			Update				( float _ElapsedTime );
	void			Render				( CRenderManager &_RM );

	//----Properties ( get & Set )---------------------------------------
	CCamera*		GetCamera			( void ) const			{ return m_pCamera; }

private:
	void			UpdateInputs		( float _ElapsedTime );

private:
	CCharactersManager			*m_pCharactersManager;
	CScene						*m_pScene;
	CThPSCamera					*m_pThPSCamera;
	CThPSCamera					*m_pThPSFreeCamera;

	CPlayer						*m_Player;
	CFreeCamera					m_FreeCamera;

	CCamera						*m_pFreeCamera;

	Vect2i						pos;
	Vect2i						screen;

	bool						m_AudioIsFade;
	uint32						m_uAudioID;
};

#endif
