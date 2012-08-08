
// ParticleEditor.h: archivo de encabezado principal para la aplicación ParticleEditor
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Símbolos principales


// CParticleEditorApp:
// Consulte la sección ParticleEditor.cpp para obtener información sobre la implementación de esta clase
//

class CParticleEditorApp : public CWinAppEx
{
public:
	CParticleEditorApp();


// Reemplazos
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementación
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
