//----------------------------------------------------------------------------------
// CThPSCharacterCamera class
// Author: Marc Cuscullola
//
// Description:
// Cámara en tercera persona con colisión para el jugador
//----------------------------------------------------------------------------------
#pragma once

#ifndef _THPS_CHARACTER_CAMERA_H_
#define _THPS_CHARACTER_CAMERA_H_

#include "Cameras\Camera.h"
#include "Math\Vector3.h"
#include <string>

class CThPSCharacterCamera : public CCamera
{
public:
	//----Constructor & Destructor--------------
	CThPSCharacterCamera( const std::string &_Name = "" );
	CThPSCharacterCamera( float zn, float zf, float fov, float aspect, CObject3D* object3D, float zoom, float heightLookAt = 0.f, float heightEye = 0.0f, const std::string &name = "" );
	~CThPSCharacterCamera();

	//----Get Methods---------------------------
	virtual Vect3f	GetDirection	() const;
	virtual Vect3f	GetLookAt		() const;
	virtual Vect3f	GetEye			() const;
	virtual Vect3f	GetVecUp		() const;

	//---Interfaz de CThPSCamera
	void			SetZoom			(float zoom);
	void			AddZoom			(float zoom);
	float			GetZoom			() const		{ return m_fZoom; }

private:
	float		m_fZoom;
	float		m_fHeightLookAt;
	float		m_fHeightEye;
};

#endif //_THPS_CHARACTER_CAMERA_H_