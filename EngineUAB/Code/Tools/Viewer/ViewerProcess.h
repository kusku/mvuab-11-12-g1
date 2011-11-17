#pragma once

#ifndef _VIEWERPROCESS_H
#define _VIEWERPROCESS_H

#include "Process.h"
#include "Math\Vector2.h"

class CViewerProcess : public CProcess
{
protected:
	Vect2i pos;
	Vect2i screen;

public:
	CViewerProcess();
	~CViewerProcess();

	void Init();
	void Update(float elapsedTime);
	void Render();
};

#endif
