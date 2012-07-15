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

//---Forward Decalarations-------
class CPhysicActor;
class CRenderManager;
//-------------------------------

class CThPSCharacterCamera : public CCamera
{
public:
	//----Constructor & Destructor--------------
	CThPSCharacterCamera( const std::string &_Name = "" );
	CThPSCharacterCamera( float zn, float zf, float fov, float aspect, CObject3D* object3D, float zoom, float heightLookAt = 0.f, float heightEye = 0.0f, const std::string &name = "" );
	~CThPSCharacterCamera();

	//---Update Methods-------------------------
	void	Update	( float _ElapsedTime );
	void	Render	( CRenderManager *_RM );

	//----Get Methods---------------------------
	inline virtual Vect3f	GetDirection	() const		{ return m_Direction; }
	inline virtual Vect3f	GetLookAt		() const		{ return m_LookAt; }
	inline virtual Vect3f	GetEye			() const		{ return m_Eye; }
	inline virtual Vect3f	GetVecUp		() const		{ return m_VecUp; }

	//---Interfaz de CThPSCamera
	void			SetZoom			(float zoom);
	void			AddZoom			(float zoom);
	float			GetZoom			() const		{ return m_fZoom; }

private:
	void		CreateCollision		();

private:
	Vect3f		m_Direction;
	Vect3f		m_LookAt;
	Vect3f		m_Eye;
	Vect3f		m_VecUp;
	Vect3f		m_PrevEye;

	float		m_fZoom;
	float		m_fMinimumZoom;
	float		m_fHeightLookAt;
	float		m_fHeightEye;

	CPhysicActor	*m_pActor;

	Vect3f		m_CollisionPoint;
	bool		m_bCollision;

	float		m_fCollisionCorrection;
	float		m_fMinimumDistanceToGround;
};

#endif //_THPS_CHARACTER_CAMERA_H_