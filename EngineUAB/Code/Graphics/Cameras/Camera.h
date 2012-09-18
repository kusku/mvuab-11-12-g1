//----------------------------------------------------------------------------------
// CCamera class
// Author: Enric Vergara
//
// Description:
// Clase base que contiene la informacion generica de una camara la cual esta atachada a un 
// puntero de la clase base CObject3D
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include "Object3D.h"
#include "Utils\Named.h"
#include "Math\Matrix44.h"
#include <Windows.h>

class CCamera : public CNamed
{
public:
	typedef enum ETypeCamera { TC_FPS = 0, TC_THPS };

public:

	CCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, ETypeCamera typeCamera);
	CCamera();
	virtual ~CCamera(){ m_pObject3D = NULL; }

	//--- GET FUNCTIONS ---
	virtual Vect3f					GetDirection			() const = 0;
	virtual Vect3f					GetLookAt				() const = 0;
	virtual Vect3f					GetEye					() const = 0;
	virtual Vect3f					GetVecUp				() const = 0;
	CObject3D*						GetObject3D				() {return m_pObject3D;}
	float							GetZf					() const { return m_fZFar;}
	float							GetZn					() const { return m_fZNear;}
	float							GetFov					() const { return m_fFOV;}
	float							GetViewD				() const { return m_fView_d;}
	float							GetAspectRatio			() const { return m_fAspectRatio;}
	ETypeCamera						GetTypeCamera			() const { return m_eTypeCamera;}
	Mat44f							GetViewMatrix			() const { return Mat44f(m_View); }
	Mat44f							GetProjectionMatrix		() const { return Mat44f(m_Projection); }
	D3DXMATRIX						GetViewMatrixDX			() const { return m_View; }
	D3DXMATRIX						GetProjectionMatrixDX	() const { return m_Projection; }
	Vect3f							GetPosition				() const { return m_pObject3D->GetPosition(); }

	//--- SET FUNCTIONS ---
	void					SetZn					( float amount )			{ m_fZNear = amount; }
	void					SetZf					( float amount )			{ m_fZFar = amount; }
	void					SetObject3D				( CObject3D* object3D )		{ m_pObject3D = object3D;}

	//--- ADD FUNCTIONS ---
	void					AddZf					( float amount )			{ m_fZFar += amount; }
	void					AddZn					( float amount )			{ m_fZNear += amount; }
	void					AddFov					( float delta_fov )			{ m_fFOV += delta_fov; }
	void					AddViewD				( float amount )			{ if( m_fView_d + amount > 1) m_fView_d += amount; }

	//Other
	virtual void			UpdateMatrices			();

protected:

	virtual void			CreateView				();
	virtual void			Createprojection		();

	CObject3D*				m_pObject3D;					// variable que determina sobre qué objeto hacemos el siguimiento
	float					m_fView_d;						// variable de debug utilizada para pintar el objeto de la camara.	
	float					m_fFOV;
	float					m_fAspectRatio;
	float					m_fZNear;						// valor del z near (a partir de que vemos)
	float					m_fZFar;						// valor del z far (hasta donde podemos ver)
	ETypeCamera				m_eTypeCamera;

public:

	D3DXMATRIX				m_View;
	D3DXMATRIX				m_Projection;
};

#endif // INC_CAMERA_H_