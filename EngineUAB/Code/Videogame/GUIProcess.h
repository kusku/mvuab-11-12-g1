//----------------------------------------------------------------------------------
// CGUIProcess class
// Author: Marc Cuscullola
//
// Description:
// Proceso que se encarga de gestionar la GUI del juego
//----------------------------------------------------------------------------------
#pragma once

#ifndef _GUI_PROCESS_H_
#define _GUI_PROCESS_H_

#include "EngineProcess.h"
#include "Cameras\StaticCamera.h"
#include <Windows.h>

class CRenderManager;
class CThPSCamera;

class CGUIProcess : public CEngineProcess
{
public:
	CGUIProcess( HWND hWnd );
	~CGUIProcess();
	
	bool			Init ();
	void			Update ( float elapsedTime );
	void			Render ( CRenderManager &RM );

	//--Métodos para exponer en LUA-------
	void			ChangeProcess		();
	void			LoadGameData		();
	void			ExitGame			();
	void			SetCameraToCore		();
	void			SetCameraToAudio	();

	static void		RegisterMethods ();

private:
	void			Release			();

private:
	CStaticCamera		m_StaticCamera;
	CThPSCamera			*m_pThPSCamera;

	HWND	m_hWnd;
};

#endif
