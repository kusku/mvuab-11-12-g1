#pragma once

#include "EngineProcess.h"

class CGUIEditorProcess : public CEngineProcess
{
public:
	CGUIEditorProcess();
	~CGUIEditorProcess();

	void Update (float elapsedTime);

	void		SetHWNDOutput		( HWND hWnd )		{ m_hWndOutput = hWnd; }

protected:
	HWND		m_hWndOutput;
};

