#pragma once

#ifndef _CORE_H
#define _CORE_H

#include "RenderManager.h"
#include "FontManager.h"
#include "Location\LanguageManager.h"
#include "Utils\Singleton.h"
#include "definesEngine.h"

class CCore : public CSingleton<CCore>
{
private:
	CRenderManager		*m_pRenderManager;
	CFontManager		*m_pFontManager;
	CLanguageManager	*m_pLanguageManager;

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
};

#endif