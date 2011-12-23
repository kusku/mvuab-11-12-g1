#include "SpotLight.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"
#include "Math\Vector2.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSpotLight::CSpotLight()
	: m_Angle(0.0f)
	, m_FallOff(0.0f)
{
}

CSpotLight::~CSpotLight()
{
}

void CSpotLight::Render(CRenderManager *RM)
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
	RM->DrawCone(0.5f, 10, colYELLOW);

	mat.SetIdentity();
	RM->SetTransform(mat);
	Vect3f l_Dir = l_Pos + m_Direction;
	RM->DrawLine( l_Pos , l_Dir );
}
