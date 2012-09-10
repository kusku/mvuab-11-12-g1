#include "SpotLight.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Math\Matrix44.h"
#include "Math\Vector2.h"
#include "XML\XMLTreeNode.h"
#include "Effects\EffectManager.h"
#include "Core.h"
#include "Base.h"
#include "Cameras\Camera.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSpotLight::CSpotLight(CXMLTreeNode &XMLNode)
{
	m_Type = SPOT;
	m_Direction = XMLNode.GetVect3fProperty("dir", Vect3f(0.0f, 0.0f, 0.0f));
	m_Angle = XMLNode.GetFloatProperty("angle", 0.0f);
	m_FallOff = XMLNode.GetFloatProperty("fall_off", 0.0f);
	
	//Extract Common Info
	ExtractCommonLightInfo(XMLNode);
}

CSpotLight::CSpotLight()
	: m_Angle(0.0f)
	, m_FallOff(0.0f)
{
	m_Direction = Vect3f(0.f,0.f,0.f);
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
	RM->DrawCone(0.6f, 10, colYELLOW);

	mat.SetIdentity();
	RM->SetTransform(mat);
	Vect3f l_Dir = l_Pos + (m_Direction.GetNormalized() * 10.0f);
	RM->DrawLine( l_Pos , l_Dir );
	
	RM->SetTransform(m44fIDENTITY);

	if(m_LightFrustum != NULL)
	{
		m_LightFrustum->DrawFrustum();
	}
}

void CSpotLight::SetShadowMap()
{
	D3DXMATRIX l_View;
	D3DXMATRIX l_Projection;

	CEffectManager* l_EffectManager = CORE->GetEffectManager();

    D3DXVECTOR3 l_Eye = D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z);

    Vect3f lookat = m_Direction;
    D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);
	
    Vect3f vup(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);

    //Setup Matrix view
	D3DXMatrixLookAtLH( &l_View, &l_Eye, &l_LookAt, &l_VUP);
	
    //Setup Matrix projection
	D3DXMatrixPerspectiveFovLH(&l_Projection, D3DX_PI * .25f, 1.0f, 1.0f, m_EndRangeAttenuation);

	m_ViewShadowMap = Mat44f(l_View);
	m_ProjectionShadowMap= Mat44f(l_Projection);

	// Compute near and far
	float Near = CORE->GetCamera()->GetZn();
	float Far  = m_EndRangeAttenuation;

	// Compute linear (distance to light) near and min/max for this perspective matrix
	float CosLightFOV = mathUtils::Cos<float>(0.5f * D3DX_PI * .25f);
	m_LightLinNearFar.x = Near;
	m_LightLinNearFar.y  = Far / (CosLightFOV * CosLightFOV);

	m_LightFrustum->Update((m_ProjectionShadowMap * m_ViewShadowMap).GetD3DXMatrix());
	l_EffectManager->SetShadowCameraEye(m_Position);
	l_EffectManager->SetShadowProjectionMatrix(m_ProjectionShadowMap);
	l_EffectManager->SetShadowViewMatrix(m_ViewShadowMap);
}
