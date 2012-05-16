#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_

#include "EngineProcess.h"
#include "Player.h"
#include "Camera\FreeCamera.h"

class CLogic;
class CScene;
class CThPSCamera;
class CCamera;

class CMain : public CEngineProcess
{
public:
	CMain(void);
	~CMain(void);

	bool			Init				( void );
	void			Update				( float _ElapsedTime );
	void			Render				( CRenderManager &RM );

	CCamera* GetCamera () const { return m_pCamera; }

private:
	void			UpdateInputs		( float _ElapsedTime );

private:
	CLogic		*m_pLogic; 
	CScene		*m_pScene;
	CThPSCamera *m_pThPSCamera;
	CThPSCamera *m_pThPSFreeCamera;

	CPlayer		m_Player;
	CFreeCamera m_FreeCamera;

	CCamera		*m_pFreeCamera;

	Vect2i pos;
	Vect2i screen;
};

#endif
