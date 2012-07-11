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

	//Guardamos la posici�n anterior
	m_PrevEye = m_Eye;

	//Obtenemos los datos del object3D asignado
	float l_fYaw = m_pObject3D->GetYaw();
	float l_fPitch = m_pObject3D->GetPitch();
	Vect3f l_Pos = m_pObject3D->GetPosition();

	//----Calculamos la posici�n de la c�mara------
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

	//----Calculamos la direcci�n------
	m_Direction = l_Pos - m_Eye;

	//----Calculamos el look at------
	m_LookAt = l_Pos;
	m_LookAt.y += m_fHeightLookAt;

	//----Situamos la esfera de colisi�n a su posici�n-----
	//m_pActor->SetGlobalPosition(m_Eye + l_Dir + v3fNEGY);

	//----Miramos las colisiones con el escenario-------
	//--------------------------------------------------
	SCollisionInfo l_CollisionInfo;
	CPhysicUserData *l_pUserData = NULL;
	CPhysicUserData *l_pUserDataBack = NULL;

	//Vectores de direcci�n de rayos
	Vect3f l_Dir = m_Eye - m_LookAt;
	l_Dir.Normalize();

	Vect3f l_DirXZ = l_Dir;
	l_DirXZ.y = 0.f;

	//M�scara de colisi�n
	int l_iMask = 1 << ECG_PERSONATGE;
	l_iMask |= 1 << ECG_OBJECTES_DINAMICS;
	l_iMask |= 1 << ECG_ESCENARI;
	l_iMask |= 1 << ECG_ENEMICS;	

	//Miramos si hay un objeto por delante de la c�mara
	l_pUserData = CORE->GetPhysicsManager()->RaycastClosestActor(m_LookAt, l_Dir, l_iMask, l_CollisionInfo);
	if( l_pUserData != NULL )
	{
		float l_fDistancePlayerToCamera = l_Pos.SqDistance(m_Eye);
		float l_fDistancePlayerToCollision = l_Pos.SqDistance(l_CollisionInfo.m_CollisionPoint);

		if( l_fDistancePlayerToCollision < l_fDistancePlayerToCamera )	//La c�mara est� por detr�s de un objeto
		{
			m_Eye = l_CollisionInfo.m_CollisionPoint - l_Dir;
		}
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
	Vect3f l_EyeRayPosition = m_Eye + v3fNEGY;
	Vect3f l_DownPosition = l_EyeRayPosition + v3fNEGY;
	Vect3f l_Dir = m_Eye - m_LookAt;
	l_Dir.Normalize();	
	Vect3f l_DirXZ = l_Dir;
	l_DirXZ.y = 0.f;

	Mat44f mat;
	mat.SetIdentity();
	_RM->SetTransform(mat);
	_RM->DrawLine( m_Eye, m_LookAt);

	mat.SetIdentity();
	mat.Translate(m_Eye);
	_RM->SetTransform(mat);
	_RM->DrawSphere(1.f, 20);

	mat.SetIdentity();
	_RM->SetTransform(mat);
	_RM->DrawLine(m_Eye + l_Dir + v3fNEGY, m_Eye + l_Dir + v3fNEGY*2, colRED);

	mat.SetIdentity();
	_RM->SetTransform(mat);
	_RM->DrawLine(m_Eye + l_DirXZ, m_Eye + l_DirXZ * 2, colRED);
}