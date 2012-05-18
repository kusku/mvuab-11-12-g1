#pragma once

#ifndef _GUI_PROCESS_H_
#define _GUI_PROCESS_H_

#include "Math\Vector2.h"
#include "EngineProcess.h"
#include "Cameras\StaticCamera.h"
#include <Windows.h>

class CRenderManager;
class CThPSCamera;

class CGUIProcess : public CEngineProcess
{
public:
	CGUIProcess( HWND hWnd );
	//CGUIProcess();
	~CGUIProcess();

		// ------------- Mètodes Principals -----------------------------------
	bool			Init				( void );
	void			Update				( float _ElapsedTime );
	void			Render				( CRenderManager &_RM );

	void				ChangeProcess		();
	void				ExitGame			();
	void				RegisterMethods		();

private:
	Vect2i		pos;
	Vect2i		screen;

	CStaticCamera		m_StaticCamera;
	CThPSCamera			*m_pThPSCamera;

	HWND				m_hWnd;
	bool				m_IsFirstSound;
};

#endif
