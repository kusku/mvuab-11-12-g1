#pragma once

#ifndef _FREE_CAMERA_H_
#define _FREE_CAMERA_H_

#include "Object3D.h"
#include "Math\Vector3.h"

#define VELOCITY_FREE_CAMERA 30.0

class CRenderManager;
class CCamera;

class CFreeCamera : public CObject3D
{
public :
	CFreeCamera();
	~CFreeCamera();

	void Update ( float _ElapsedTime, CCamera *camera );
	void Render	( CRenderManager *_RM );

private:
	Vect3f m_Dir;
};

#endif
