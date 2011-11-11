#pragma once

#ifndef _CORE_H
#define _CORE_H

#include "RenderManager.h"

class CCore
{
private:
	CRenderManager *m_RenderManager;
public:
	CCore();
	virtual ~CCore();

	void Init(HWND hWnd);
	void Update(float ElapsedTime);
	void Render();

	CRenderManager* GetRenderManager() const;
};

#endif