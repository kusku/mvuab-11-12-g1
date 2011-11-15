#pragma once

#ifndef _ENGINE_H
#define _ENGINE_H

#include "Core.h"
#include "Process.h"
#include "Math\Color.h"

class CEngine
{
protected:
	CCore *m_pCore;
	CProcess *m_pProcess;

	const CColor string2Color( const std::string &color );

public:
	CEngine();
	virtual ~CEngine();

	void Init( HWND hWnd, const SConfig &Config );
	void Update( float ElapsedTime );
	void Render();
	void RenderScene( CRenderManager* renderManager );

	void LoadConfigXML( const std::string &configFile, SConfig &ConfigStruct_ );

	void SetProcess( CProcess *process );
};

#endif