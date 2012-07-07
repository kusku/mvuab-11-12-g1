#include "ThPSCharacterCamera.h"
#include <assert.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

//----------------------------------------------
CThPSCharacterCamera::CThPSCharacterCamera(const std::string &_Name)
{
	m_Name = _Name;
}

//----------------------------------------------
CThPSCharacterCamera::CThPSCharacterCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, float zoom, float heightLookAt, float heightEye, const std::string &name)
	: CCamera( zn, zf, fov, aspect, object3D, TC_THPS)
	, m_fZoom( zoom )
	, m_fHeightLookAt( heightLookAt )
	, m_fHeightEye( heightEye )
{
	m_Name = name;
}

//----------------------------------------------
CThPSCharacterCamera::~CThPSCharacterCamera()
{
}

//----------------------------------------------
Vect3f CThPSCharacterCamera::GetDirection() const
{
	assert(m_pObject3D);

	return (m_pObject3D->GetPosition() - GetEye());
}

//----------------------------------------------
Vect3f CThPSCharacterCamera::GetEye() const
{
	assert(m_pObject3D);

	float yaw = m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();
	Vect3f pos = m_pObject3D->GetPosition();
	pos.y += m_fHeightEye;

	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	Vect3f vEyePt(	m_fZoom * cos(yaw) * cos(pitch), 
		m_fZoom * sin(pitch),
		m_fZoom * sin(yaw) * cos(pitch) );

	return (pos - vEyePt); 
}

//----------------------------------------------
Vect3f CThPSCharacterCamera::GetLookAt() const
{
	assert(m_pObject3D);

	Vect3f pos = m_pObject3D->GetPosition();
	pos.y += m_fHeightLookAt;

	return pos;
}

//----------------------------------------------
Vect3f CThPSCharacterCamera::GetVecUp() const
{
	assert(m_pObject3D);

	float yaw		= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();

	Vect3f vUpVec(	-cos(yaw) * sin(pitch), 
		cos(pitch), 
		-sin(yaw) * sin(pitch) );

	return vUpVec;
}

//----------------------------------------------
void CThPSCharacterCamera::SetZoom (float zoom)
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
void CThPSCharacterCamera::AddZoom (float zoom)
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