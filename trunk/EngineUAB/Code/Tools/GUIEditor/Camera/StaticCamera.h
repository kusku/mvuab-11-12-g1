#pragma once

#ifndef _STATIC_CAMERA_GUI_EDITOR_H_
#define _STATIC_CAMERA_GUI_EDITOR_H_

#include "Math\Vector3.h"
#include "Object3D.h"

class CCamera;
class CRenderManager;

class CStaticCamera : public CObject3D
{
public:
	CStaticCamera();
	~CStaticCamera();

	void Update(float _ElapsedTime, CCamera* camera);
	void Render(CRenderManager *RM);
};

#endif