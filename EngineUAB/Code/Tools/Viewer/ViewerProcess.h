#pragma once

#ifndef _VIEWERPROCESS_H
#define _VIEWERPROCESS_H

#include "Process.h"

class CViewerProcess : public CProcess
{
protected:
	float time;
public:
	CViewerProcess();
	~CViewerProcess();

	void Init();
	void Update(float elapsedTime);
	void Render();
};

#endif
