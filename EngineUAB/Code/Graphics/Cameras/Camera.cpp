#include <d3d9.h>

#include "Camera.h"
#include "Assert.h"
#include "Math\MathUtils.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CCamera::CCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, ETypeCamera typeCamera)
: m_pObject3D(object3D)
,	m_fFOV(fov)
,	m_fAspectRatio(aspect)
,	m_fZNear(zn)				
,	m_fZFar(zf)		
, m_fView_d(2.f)
, m_eTypeCamera(typeCamera)
{
	assert(m_pObject3D);
}

CCamera::CCamera()
: m_pObject3D(NULL)
,	m_fFOV( mathUtils::Deg2Rad(60.f) )
,	m_fAspectRatio(4.f/3.f)
,	m_fZNear(0.1f)				
,	m_fZFar(100.f)	
, m_fView_d(2.f)
, m_eTypeCamera(TC_THPS)
{}

void CCamera::UpdateMatrices()
{	
    Vect3f eye = this->GetEye();
    D3DXVECTOR3 l_Eye = D3DXVECTOR3(eye.x, eye.y, eye.z);

    Vect3f lookat = this->GetLookAt();
    D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);

    Vect3f vup = this->GetVecUp();
    D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);

    //Setup Matrix view
	D3DXMatrixLookAtLH( &m_View, &l_Eye, &l_LookAt, &l_VUP);

    //Setup Matrix projection
    D3DXMatrixPerspectiveFovLH( &m_Projection, this->GetFov(), this->GetAspectRatio(), this->GetZn(), this->GetZf());
}

