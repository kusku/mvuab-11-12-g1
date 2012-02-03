#include "OrthoFixedCameraController.h"

COrthoFixedCameraController::COrthoFixedCameraController(Vect3f lightVector, Vect3f position, float witdh, float heigth, float near, float far)
{
}

COrthoFixedCameraController::~COrthoFixedCameraController()
{
}

Vect3f	COrthoFixedCameraController::GetDirection() const
{
	return Vect3f(0.0f, 0.0f, 0.0f);
}

Vect3f	COrthoFixedCameraController::GetLookAt() const
{
	return Vect3f(0.0f, 0.0f, 0.0f);
}

Vect3f	COrthoFixedCameraController::GetEye() const
{
	return Vect3f(0.0f, 0.0f, 0.0f);
}

Vect3f	COrthoFixedCameraController::GetVecUp() const
{
	return Vect3f(0.0f, 0.0f, 0.0f);
}