#pragma once

#ifndef _VIEWERPROCESS_H
#define _VIEWERPROCESS_H

class CCamera;
class CThPSCamera;
class CStaticMeshManager;

#include "Player.h"
#include "Process.h"
#include "Math\Vector2.h"

class CViewerProcess : public CProcess
{
public:
	CViewerProcess();
	~CViewerProcess();

	void Init();
	void Update(float elapsedTime);
	void Render(CRenderManager *RM);

	CCamera* GetCamera () const { return m_Camera; }

protected:
	void UpdateInputs( float elapsedTime );

	Vect2i pos;
	Vect2i screen;

	float yaw;
	CPlayer m_Player;

	CThPSCamera *m_pThPSCamera;
	CStaticMeshManager *m_SMManager;
};

#endif
