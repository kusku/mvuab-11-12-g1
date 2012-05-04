#pragma once

#ifndef _VIEWERPROCESS_H
#define _VIEWERPROCESS_H

class CCamera;
class CThPSCamera;
class CActionToInput;

#include "Player.h"
#include "EngineProcess.h"
#include "Math\Vector2.h"
#include "Modifiers\ModifierManager.h"

class CViewerProcess : public CEngineProcess
{
public:
	CViewerProcess();
	~CViewerProcess();

	bool Init();
	void Update(float elapsedTime);
	void Render(CRenderManager &RM);

	CCamera* GetCamera () const { return m_pCamera; }

protected:
	void UpdateInputs( float elapsedTime );
	void UpdateDebugInputs( float elapsedTime, CActionToInput &action2Input );

	Vect2i pos;
	Vect2i screen;

	float yaw;
	CPlayer m_Player;

	CThPSCamera *m_pThPSCamera;
};

#endif
