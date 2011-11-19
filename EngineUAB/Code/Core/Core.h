#pragma once

#ifndef _CORE_H
#define _CORE_H

class CInputManager;
class CFontManager;
class CLanguageManager;
class CActionToInput;
class CRenderManager;

#include <Windows.h>
#include "Utils\Singleton.h"
#include "CoreDefs.h"

class CCore : public CSingleton<CCore>
{
private:
	CRenderManager		*m_pRenderManager;
	CFontManager		*m_pFontManager;
	CLanguageManager	*m_pLanguageManager;
	CInputManager		*m_pInputManager;
	CActionToInput		*m_pActionToInput;

public:
	CCore();
	virtual ~CCore();

	void Init( HWND hWnd, const SConfig &config );
	void Release();
	void Update( float ElapsedTime );
	void Render();

	CRenderManager*		GetRenderManager() const	{ return m_pRenderManager; }
	CFontManager*		GetFontManager() const		{ return m_pFontManager; }
	CLanguageManager*	GetLanguageManager() const	{ return m_pLanguageManager; }
	CInputManager*		GetInputManager() const		{ return m_pInputManager; }
	CActionToInput*		GetActionToInput() const	{ return m_pActionToInput; }
};

#endif