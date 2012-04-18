#pragma once

#include "EngineProcess.h"
class CGUIManager;
class CRenderManager;

class CGUIEditorProcess : public CEngineProcess
{
public:
	CGUIEditorProcess();
	~CGUIEditorProcess();

	void Init();
	void Update (float elapsedTime);
	void Render(CRenderManager &RM);

	void		SetHWNDOutput		( HWND hWnd )		{ m_hWndOutput = hWnd; }
	CGUIManager*	GetGUIManager	() const		{ return m_pGUIManager; }
protected:
	HWND		m_hWndOutput;

	CGUIManager		*m_pGUIManager;
};

