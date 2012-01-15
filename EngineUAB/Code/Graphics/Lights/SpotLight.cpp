#include "SpotLight.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"
#include "Math\Vector2.h"
#include "XML\XMLTreeNode.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSpotLight::CSpotLight(CXMLTreeNode *XMLNode)
{
	m_Type = SPOT;
	m_Name = XMLNode->GetPszProperty("name", "");
	m_Position = XMLNode->GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f));
	m_Direction = XMLNode->GetVect3fProperty("dir", Vect3f(0.0f, 0.0f, 0.0f));
	m_Angle = XMLNode->GetFloatProperty("angle", 0.0f);
	m_FallOff = XMLNode->GetFloatProperty("fall_off", 0.0f);
	m_StartRangeAttenuation = XMLNode->GetFloatProperty("att_start_range", 0.0f);
	m_EndRangeAttenuation = XMLNode->GetFloatProperty("att_end_range", 0.0f);
	m_RenderShadows = XMLNode->GetBoolProperty("render_shadows", false);
	Vect3f l_Color = XMLNode->GetVect3fProperty("color", Vect3f(0.0f, 0.0f, 0.0f));

	m_Color = CColor( l_Color.x, l_Color.y, l_Color.z );
}

CSpotLight::CSpotLight()
	: m_Angle(0.0f)
	, m_FallOff(0.0f)
{
	m_Type = SPOT;
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
