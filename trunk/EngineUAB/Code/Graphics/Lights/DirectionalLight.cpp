#include "DirectionalLight.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDirectionalLight::CDirectionalLight()
	: m_Direction(Vect3f(0.f, 0.f, 0.f))
{
}

CDirectionalLight::~CDirectionalLight()
{
}

void CDirectionalLight::Render(CRenderManager *RM)
{
	Mat44f mat, matRotYaw, matRotPitch;
	Vect3f l_Pos = GetPosition();

	mat.SetIdentity();
	matRotYaw.SetIdentity();
	matRotPitch.SetIdentity();

	mat.Translate( l_Pos );
	matRotYaw.RotByAngleZ( GetYaw() );
	matRotPitch.RotByAngleX( GetPitch() );

	mat = mat * matRotYaw * matRotPitch;

	RM->SetTransform( mat );
	RM->DrawTower(0.3f, colYELLOW);

	mat.SetIdentity();
	RM->SetTransform(mat);
	Vect3f l_Dir = l_Pos + m_Direction;
	RM->DrawLine( l_Pos , l_Dir );

}

void CDirectionalLight::CalculateOrientationDebugRender()
{
	Vect2f l_XYCam = Vect2f(0.0f, 1.0f);
	Vect2f l_YZCam = Vect2f(1.0f, 0.0f);

	Vect2f l_DirPlane;
	l_DirPlane.x = m_Direction.x;
	l_DirPlane.y = m_Direction.y;
	float l_Yaw = 0.f;
	if( l_DirPlane.Length() != 0.f)
	{
		if( m_Direction.y > 0.f )
		{
			if( m_Direction.x < 0.f )
				l_Yaw = mathUtils::ACos<float>((l_XYCam * l_DirPlane)/(l_XYCam.Length()*l_DirPlane.Length()));
			else
				l_Yaw = -mathUtils::ACos<float>((l_XYCam * l_DirPlane)/(l_XYCam.Length()*l_DirPlane.Length()));
		}
		else
		{
			if( m_Direction.x < 0.f )
				l_Yaw = mathUtils::ACos<float>((l_XYCam * l_DirPlane)/(l_XYCam.Length()*l_DirPlane.Length()));
			else
				l_Yaw =-mathUtils::ACos<float>((l_XYCam * l_DirPlane)/(l_XYCam.Length()*l_DirPlane.Length()));
		}
	}


	l_DirPlane.x = m_Direction.y;
	l_DirPlane.y = m_Direction.z;
	float l_Pitch = 0.f;
	if( l_DirPlane.Length() != 0.f)
	{
		if( m_Direction.y > 0.f )
		{
			if( m_Direction.z > 0.f )
				l_Pitch = mathUtils::ACos<float>((l_YZCam*l_DirPlane)/(l_YZCam.Length()*l_DirPlane.Length()));
			else
				l_Pitch =-mathUtils::ACos<float>((l_YZCam*l_DirPlane)/(l_YZCam.Length()*l_DirPlane.Length()));
		}
		else
		{
			if( m_Direction.z > 0.f )
				l_Pitch = D3DX_PI-mathUtils::ACos<float>((l_YZCam*l_DirPlane)/(l_YZCam.Length()*l_DirPlane.Length()));
			else
				l_Pitch = -D3DX_PI + mathUtils::ACos<float>((l_YZCam*l_DirPlane)/(l_YZCam.Length()*l_DirPlane.Length()));
		}
	}

	SetYaw(l_Yaw);
	SetPitch(l_Pitch);
}