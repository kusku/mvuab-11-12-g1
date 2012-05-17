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

class CMain : public CEngineProcess
{
public:
	// ------------- Constructors i Destructors --------------------------
					CMain(void);
					~CMain(void);

	// ------------- Mètodes Principals -----------------------------------
	bool			Init				( void );
	void			Update				( float _ElapsedTime );
	void			Render				( CRenderManager &_RM );

private:
	void			UpdateInputs		( float _ElapsedTime );

	//----Properties ( get & Set )----------------------------------------
public:
	CCamera*		GetCamera			( void ) const			{ return m_pCamera; }

	//----Members --------------------------------------------------------
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
};

#endif
