#pragma once

#ifndef _PROCESS_H
#define _PROCESS_H

#include "RenderManager.h"

class CProcess
{
protected:
public:
	CProcess();

	virtual ~CProcess();

	virtual void Init();
	virtual void Update(float ElapsedTime);
	virtual void Render();
};

#endif