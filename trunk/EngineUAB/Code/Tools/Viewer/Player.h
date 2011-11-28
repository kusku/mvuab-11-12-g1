#pragma once

#ifndef _PLAYER_H
#define _PLAYER_H

class CRenderManager;
class CCamera;

#include "Object3D.h"
#include "Math\Vector3.h"

class CPlayer : public CObject3D
{
public:
	CPlayer();
	~CPlayer();

	void Update(float elapsedTime, CCamera* camera);
	void Render(CRenderManager *RM);

private:
	void UpdateInputActions(float elapsedTime, CCamera* camera);

	Vect3f	m_Dir;
};

#endif
