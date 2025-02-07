#include <d3d9.h>

#include "Camera.h"
#include "Assert.h"
#include "Math\MathUtils.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CCamera::CCamera( float _Zn, float _Zf, float _Fov, float _Aspect, CObject3D* _pObject3D, ETypeCamera _TypeCamera )
	: m_pObject3D		( _pObject3D )
	, m_fFOV			( _Fov )
	, m_fAspectRatio	( _Aspect )
	, m_fZNear			( _Zn )				
	, m_fZFar			( _Zf )		
	, m_fView_d			( 2.f )
	, m_eTypeCamera		( _TypeCamera )
{
	m_Name = "";
	assert(m_pObject3D);
}

CCamera::CCamera( void )
	: m_pObject3D		( NULL)
	, m_fFOV			( mathUtils::Deg2Rad(60.f) )
	, m_fAspectRatio	( 4.f/3.f )
	, m_fZNear			( 0.1f )				
	, m_fZFar			( 100.f )	
	, m_fView_d			( 2.f )
	, m_eTypeCamera		( TC_THPS )
{
	m_Name = "";
}

void CCamera::UpdateMatrices()
{	
	CreateView();
	Createprojection();
}

void CCamera::CreateView()
{
    Vect3f eye = this->GetEye();
    D3DXVECTOR3 l_Eye = D3DXVECTOR3(eye.x, eye.y, eye.z);

    Vect3f lookat = this->GetLookAt();
    D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);

    Vect3f vup = this->GetVecUp();
    D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);

    //Setup Matrix view
	D3DXMatrixLookAtLH( &m_View, &l_Eye, &l_LookAt, &l_VUP);
}

void CCamera::Createprojection()
{
    //Setup Matrix projection
    D3DXMatrixPerspectiveFovLH( &m_Projection, this->GetFov(), this->GetAspectRatio(), this->GetZn(), this->GetZf());
}

