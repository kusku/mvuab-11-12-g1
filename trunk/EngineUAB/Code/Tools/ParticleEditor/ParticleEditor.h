
// ParticleEditor.h: archivo de encabezado principal para la aplicaci�n ParticleEditor
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // S�mbolos principales


// CParticleEditorApp:
// Consulte la secci�n ParticleEditor.cpp para obtener informaci�n sobre la implementaci�n de esta clase
//

class CParticleEditorApp : public CWinAppEx
{
public:
	CParticleEditorApp();


// Reemplazos
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementaci�n
	UINT	m_nAppLook;
	BOOL	m_bHiColorIcons;
	bool	m_bStarting;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnNewSystem();
	DECLARE_MESSAGE_MAP()
};

extern CParticleEditorApp theApp;
