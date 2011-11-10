#pragma once

#ifndef _VIEWERPROCESS_H
#define _VIEWERPROCESS_H

#include "Process.h"

class CViewerProcess : public CProcess
{
public:
	CViewerProcess();
	~CViewerProcess();

	void Init();
	void Update(float ElapsedTime);
	void Render();
};

#endif
