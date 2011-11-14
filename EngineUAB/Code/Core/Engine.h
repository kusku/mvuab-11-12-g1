#pragma once

#ifndef _ENGINE_H
#define _ENGINE_H

#include "Core.h"
#include "Process.h"

class CEngine
{
protected:
	CCore *m_pCore;
	CProcess *m_pProcess;

public:
	CEngine();
	virtual ~CEngine();

	void Init(HWND hWnd);
	void Update(float ElapsedTime);
	void Render();
	void RenderScene(CRenderManager* renderManager);

	void SetProcess(CProcess *process);
};

#endif