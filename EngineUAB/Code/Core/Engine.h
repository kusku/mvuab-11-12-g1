#pragma once

#ifndef _ENGINE_H
#define _ENGINE_H

#include "Core.h"
#include "Process.h"
#include "Math\Color.h"
#include "Utils\Timer.h"
#include "CoreDefs.h"

class CEngine
{
protected:
	CCore		*m_pCore;
	CProcess	*m_pProcess;
	CTimer		m_pTimer;

	SConfig		m_Config;

	const CColor string2Color( const std::string &color );

public:
	CEngine();
	virtual ~CEngine();

	void Init( HWND hWnd );
	void Update();
	void Render();
	void RenderScene( CRenderManager* renderManager );
	uint32 RenderFPS	();

	void LoadConfigXML	( const std::string &configFile );
	void Reload			();

	void SetProcess( CProcess *process );

	Vect2i		GetResolution	() const { return m_Config.resolution; }
	Vect2i		GetPosition		() const { return m_Config.position; }
};

#endif