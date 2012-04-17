#pragma once

#ifndef _TESTPROCESS_H
#define _TESTPROCESS_H

class CCamera;
class CThPSCamera;
class CActionToInput;

#include "Player.h"
#include "EngineProcess.h"
#include "Math\Vector2.h"
#include <vector>

class CGUIManager;

class CTestGUIProcess : public CEngineProcess
{
public:
	CTestGUIProcess();
	virtual ~CTestGUIProcess();

	void Init ();
	void Update (float elapsedTime);
	void Render (CRenderManager &RM);

protected:
	void UpdateInputs( float elapsedTime );
	void UpdateDebugInputs( float elapsedTime, CActionToInput &action2Input );

	Vect2i pos;
	Vect2i screen;

	CPlayer m_Player;
	CThPSCamera *m_pThPSCamera;

	CGUIManager *m_pGUIManager;
};

#endif
