#include "ThPSCharacterCamera.h"
#include "PhysicActor.h"
#include "PhysicUserData.h"
#include "PhysicsManager.h"
#include "Core.h"
#include "Base.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"
#include "Math\Vector2.h"
#include <assert.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

//----------------------------------------------
CThPSCharacterCamera::CThPSCharacterCamera(const std::string &_Name)
	: m_pActor( NULL )
	, m_fZoom(0.f)
	, m_fMinimumZoom(0.f)
	, m_fHeightLookAt(0.f)
	, m_fHeightEye(0.f)
	, m_Direction(v3fTOP)
	, m_VecUp(v3fFRONT)
	, m_Eye(v3fZERO)
	, m_PrevEye(v3fZERO)
	, m_LookAt(v3fBOTTOM)
	, m_bCollision(false)
	, m_CollisionPoint(v3fZERO)
	, m_fCollisionCorrection(0.15f)
	, m_fMinimumDistanceToGround(1.f)
{
	m_Name = _Name;
}

//----------------------------------------------
CThPSCharacterCamera::CThPSCharacterCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, float zoom, float heightLookAt, float heightEye, const std::string &name)
	: CCamera( zn, zf, fov, aspect, object3D, TC_THPS)
	, m_pActor( NULL )
	, m_fZoom( zoom )
	, m_fMinimumZoom( zoom )
	, m_fHeightLookAt( heightLookAt )
	, m_fHeightEye( heightEye )
	, m_Direction(v3fTOP)
	, m_VecUp(v3fFRONT)
	, m_Eye(v3fZERO)
	, m_PrevEye(v3fZERO)
	, m_LookAt(v3fBOTTOM)
	, m_bCollision(false)
	, m_CollisionPoint(v3fZERO)
	, m_fCollisionCorrection(0.05f)
	, m_fMinimumDistanceToGround(1.f)
{
	m_Name = name;
	//CreateCollision();
}

//----------------------------------------------
CThPSCharacterCamera::~CThPSCharacterCamera()
{
	CHECKED_DELETE(m_pActor);
}

//----------------------------------------------
void CThPSCharacterCamera::Update(float _ElapsedTime)
{
	assert(m_pObject3D);

	//Variables locales sobre la colisión
	SCollisionInfo l_CollisionInfo;
	CPhysicUserData *l_pUserData = NULL;

	//Guardamos la posición anterior
	m_PrevEye = m_Eye;

	//Obtenemos los datos del object3D asignado
	float l_fYaw = m_pObject3D->GetYaw();
	float l_fPitch = m_pObject3D->GetPitch();
	Vect3f l_Pos = m_pObject3D->GetPosition();

	//----Calculamos la posición de la cámara------
	Vect3f l_PosEye = l_Pos;
	l_Pos.y += m_fHeightEye;

	Vect3f l_vEyePt(m_fZoom * cos(l_fYaw) * cos(l_fPitch),  //Pasamos de coordenadas esfericas a coordenadas cartesianas
					m_fZoom * sin(l_fPitch),
					m_fZoom * sin(l_fYaw) * cos(l_fPitch) );

	m_Eye = l_Pos - l_vEyePt; 

	//----Calculamos el vector up------
	m_VecUp = Vect3f(	-cos(l_fYaw) * sin(l_fPitch), 
						cos(l_fPitch), 
						-sin(l_fYaw) * sin(l_fPitch) );

	//----Calculamos la dirección------
	m_Direction = l_Pos - m_Eye;

	//----Calculamos el look at------
	m_LookAt = l_Pos;
	m_LookAt.y += m_fHeightLookAt;

	//----Situamos la esfera de colisión a su posición-----
	//m_pActor->SetGlobalPosition(m_Eye + l_Dir + v3fNEGY);

	//----Miramos las colisiones con el escenario-------
	//--------------------------------------------------

	//Vectores de dirección de rayos
	Vect3f l_Dir = m_Eye - m_LookAt;
	l_Dir.Normalize();

	Vect3f l_DirXZ = l_Dir;
	l_DirXZ.y = 0.f;

	Vect3f l_vLeft = l_DirXZ.Cross(v3fNEGY);

	//Máscara de colisión
	int l_iMask = 1 << ECG_ESCENARI;
	l_iMask |= 1 << ECG_OBJECTES_DINAMICS;
	l_iMask |= 1 << ECG_ENEMICS;	

	//Miramos si hay un objeto por delante de la cámara
	l_pUserData = CORE->GetPhysicsManager()->RaycastClosestActor(m_LookAt, l_Dir, l_iMask, l_CollisionInfo);
	if( l_pUserData != NULL )
	{
		float l_fDistancePlayerToCamera = l_Pos.SqDistance(m_Eye);
		float l_fDistancePlayerToCollision = l_Pos.SqDistance(l_CollisionInfo.m_CollisionPoint);

		if( l_fDistancePlayerToCollision < l_fDistancePlayerToCamera )	//La cámara está por detrás de un objeto
		{
			m_Eye = l_CollisionInfo.m_CollisionPoint - l_Dir * 0.5f;
		}
	}

	//Miramos si colisiona con algun sitio y desplazamos la cámara
	l_pUserData = CORE->GetPhysicsManager()->RaycastClosestActor(m_Eye + l_Dir + v3fNEGY, v3fNEGY, l_iMask, l_CollisionInfo);
	if( l_pUserData == NULL )
	{
		m_Eye -= v3fNEGY * 1.5f * _ElapsedTime;
		//m_Eye -= v3fNEGY * 0.5f;
		//m_pObject3D->SetPitch( -ePI<float>() / 6 ); //Bloquea el movimiento del pitch
	}
	else
	{
		if ( l_CollisionInfo.m_fDistance < m_fMinimumDistanceToGround )
		{
			m_Eye -= v3fNEGY * m_fCollisionCorrection * _ElapsedTime;
		}
	}

	l_pUserData = CORE->GetPhysicsManager()->RaycastClosestActor(m_Eye + l_vLeft + v3fNEGY, v3fNEGY, l_iMask, l_CollisionInfo);
	if( l_pUserData == NULL )
	{
		m_Eye -= l_vLeft * 1.5f * _ElapsedTime;
	}
	else
	{
		if ( l_CollisionInfo.m_fDistance < m_fMinimumDistanceToGround )
		{
			m_Eye -= v3fNEGY * m_fCollisionCorrection * _ElapsedTime;
		}
	}

	l_pUserData = CORE->GetPhysicsManager()->RaycastClosestActor(m_Eye - l_vLeft + v3fNEGY, v3fNEGY, l_iMask, l_CollisionInfo);
	if( l_pUserData == NULL )
	{
		//m_Eye += l_vLeft * 0.5f;
		m_Eye += l_vLeft * 1.5f * _ElapsedTime;
	}
	else
	{
		if ( l_CollisionInfo.m_fDistance < m_fMinimumDistanceToGround )
		{
			m_Eye -= v3fNEGY * m_fCollisionCorrection * _ElapsedTime;
		}
	}

	//Cálculo de la diferencia de ángulo para modificar el Pitch
	Vect3f l_NewDirection = l_Pos - m_Eye;

	Vect2f l_DirXY = m_Direction.GetProjXY();
	Vect2f l_NewDirXY = l_NewDirection.GetProjXY();

	l_DirXY.Normalize();
	l_NewDirXY.Normalize();

	float angle = l_DirXY.Dot(l_NewDirXY);
	if( angle < 0.99999999f )
	{
		angle = mathUtils::ACos(angle);

		angle = l_fPitch - angle;
		m_pObject3D->SetPitch(angle);
	}
}

//----------------------------------------------
void CThPSCharacterCamera::SetZoom(float zoom)
{
	m_fZoom = zoom;

	if( m_fZoom > m_fZFar*0.8f )
	{
		m_fZoom = m_fZFar*0.8f;
	}
	else if( m_fZoom < m_fZNear*2.f) 
	{
		m_fZoom = m_fZNear*2.f;
	}
}

//----------------------------------------------
void CThPSCharacterCamera::AddZoom(float zoom)
{
	m_fZoom += zoom;

	if( m_fZoom > m_fZFar*0.8f ) 
	{
		m_fZoom = m_fZFar*0.8f;
	}
	else if( m_fZoom < m_fZNear*2.f)
	{
		m_fZoom = m_fZNear*2.f;
	}
}

void CThPSCharacterCamera::CreateCollision()
{
	assert(m_pObject3D);

	CHECKED_DELETE(m_pActor);

	CPhysicUserData *l_pUserData = new CPhysicUserData("camera");
	l_pUserData->SetPaint(true);
	l_pUserData->SetColor(colBLUE);
	
	m_pActor = new CPhysicActor(l_pUserData);
	m_pActor->AddSphereShape(0.1f, m_pObject3D->GetPosition(), v3fZERO, 0, ECG_ESCENARI);
	m_pActor->CreateBody(0.1f);

	CORE->GetPhysicsManager()->AddPhysicActor(m_pActor);
}

void CThPSCharacterCamera::Render(CRenderManager *_RM)
{
	Mat44f mat;

	Vect3f l_EyeRayPosition = m_Eye + v3fNEGY;
	Vect3f l_DownPosition = l_EyeRayPosition + v3fNEGY;
	Vect3f l_Dir = m_Eye - m_LookAt;
	l_Dir.Normalize();	

	Vect3f l_DirXZ = l_Dir;
	l_DirXZ.y = 0.f;

	Vect3f l_vLeft = l_DirXZ.Cross(v3fNEGY);

	//Esfera de la cámara
	mat.SetIdentity();
	mat.Translate(m_Eye);
	_RM->SetTransform(mat);
	_RM->DrawSphere(1.f, 15);

	//Rayo desde la cámara hacia el punto de Look At
	mat.SetIdentity();
	_RM->SetTransform(mat);
	_RM->DrawLine( m_Eye, m_LookAt);

	//Suelo de la cámara
	mat.SetIdentity();
	_RM->SetTransform(mat);
	_RM->DrawLine(m_Eye + l_Dir + v3fNEGY, m_Eye + l_Dir + v3fNEGY*2, colRED);

	//Espalda de la cámara
	mat.SetIdentity();
	_RM->SetTransform(mat);
	_RM->DrawLine(m_Eye + l_DirXZ, m_Eye + l_DirXZ * 2, colRED);

	//Izquierda de la cámara
	mat.SetIdentity();
	_RM->SetTransform(mat);
	_RM->DrawLine(m_Eye + l_vLeft + v3fNEGY, m_Eye + l_vLeft + v3fNEGY * 2, colRED);

	//Derecha de la cámara
	mat.SetIdentity();
	_RM->SetTransform(mat);
	_RM->DrawLine(m_Eye - l_vLeft + v3fNEGY, m_Eye - l_vLeft + v3fNEGY * 2, colRED);

}