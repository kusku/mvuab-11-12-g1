#pragma once

#ifndef _VIEWERPROCESS_H
#define _VIEWERPROCESS_H

class CCamera;
class CPlayer;

#include "Process.h"
#include "Math\Vector2.h"

class CViewerProcess : public CProcess
{
protected:
	Vect2i pos;
	Vect2i screen;

	float m_RotTerra;
	float m_RotTMateixa;
	float m_RotLluna;


	float yaw;
	CPlayer *m_Player;

public:
	CViewerProcess();
	~CViewerProcess();

	void Init();
	void Update(float elapsedTime);
	void Render(CRenderManager *RM);

	CCamera* GetCamera () const { return m_Camera; }
};

#endif
