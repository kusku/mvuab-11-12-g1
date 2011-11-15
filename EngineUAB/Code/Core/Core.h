#pragma once

#ifndef _CORE_H
#define _CORE_H

#include "RenderManager.h"
#include "FontManager.h"
#include "Location\LanguageManager.h"
#include "Utils\Singleton.h"

struct SConfig
{
	bool fullscreen;
	Vect2i position;
	Vect2i resolution;
	CColor color_debug;
	CColor color_release;

	std::string default_language;
	std::string fonts_path;
	std::vector<std::string> languages_path;
};

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