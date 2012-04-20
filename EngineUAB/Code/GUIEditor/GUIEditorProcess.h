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

	CGUIManager*	GetGUIManager	() const		{ return m_pGUIManager; }
protected:

	CGUIManager		*m_pGUIManager;
};

