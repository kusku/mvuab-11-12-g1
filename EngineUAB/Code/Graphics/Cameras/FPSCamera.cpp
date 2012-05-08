#include "FPSCamera.h"
#include <assert.h>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CFPSCamera::CFPSCamera( float _Zn, float _Zf, float _Fov, float _Aspect, CObject3D* _pObject3D )
	: CCamera( _Zn, _Zf, _Fov, _Aspect, _pObject3D, TC_FPS)
{}

CFPSCamera::CFPSCamera( void )
	: CCamera()
{}


// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------

Vect3f CFPSCamera::GetDirection ( void ) const
{
	assert( m_pObject3D );

	return (GetLookAt() - GetEye());
}

Vect3f CFPSCamera::GetLookAt ( void ) const
{
	assert(m_pObject3D);

	float yaw	= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();
	Vect3f pos	= m_pObject3D->GetPosition();

	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	Vect3f vEyePt( cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch) );

	return (pos + vEyePt);
}

Vect3f CFPSCamera::GetEye ( void ) const
{
	assert(m_pObject3D);

	return m_pObject3D->GetPosition();
}

Vect3f CFPSCamera::GetVecUp ( void ) const
{
	assert(m_pObject3D);

	float yaw		= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();

	Vect3f vUpVec(	-cos(yaw) * sin(pitch), 
		cos(pitch), 
		-sin(yaw) * sin(pitch) );

	return vUpVec;
}