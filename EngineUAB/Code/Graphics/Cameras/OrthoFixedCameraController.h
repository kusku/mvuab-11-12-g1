
#pragma once

#ifndef _ORTHO_FIXED_CAMERA_CONTROLLER_H
#define _ORTHO_FIXED_CAMERA_CONTROLLER_H

#include "Camera.h"
#include "Math\Vector3.h"

class COrthoFixedCameraController : public CCamera
{
public:
	COrthoFixedCameraController(Vect3f lightVector, Vect3f position, float witdh, float heigth, float near, float far);
	virtual ~COrthoFixedCameraController();

	//Get
	Vect3f					GetDirection			() const;
	Vect3f					GetLookAt				() const;
	Vect3f					GetEye					() const;
	Vect3f					GetVecUp				() const;
};

#endif