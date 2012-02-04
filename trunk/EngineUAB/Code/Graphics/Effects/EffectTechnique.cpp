#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Lights\Light.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffectTechnique::CEffectTechnique(	CXMLTreeNode *XMLNode )
	: m_D3DTechnique(NULL)
	, m_UseInverseProjMatrix(false)
	, m_UseInverseViewMatrix(false)
	, m_UseInverseWorldMatrix(false)
	, m_UseLightAmbientColor(false)
	, m_UseViewMatrix(false)
	, m_UseViewProjectionMatrix(false)
	, m_UseTime(false)
{
	m_TechniqueName = XMLNode->GetPszProperty("name", "");
	m_UseWorldMatrix = XMLNode->GetBoolProperty("use_world_matrix", false, false);
	m_UseViewProjectionMatrix = XMLNode->GetBoolProperty("use_view_projection_matrix", false, false);
	m_UseWorldViewProjectionMatrix= XMLNode->GetBoolProperty("use_world_view_projection_matrix", false, false);
	m_UseProjMatrix = XMLNode->GetBoolProperty("use_projection_matrix", false, false);
	m_UseWorldViewMatrix = XMLNode->GetBoolProperty("use_world_view_matrix", false, false);
	m_UseShadowViewProjectionMatrix = XMLNode->GetBoolProperty("use_shadow_view_projection_matrix", false, false);
	m_UseShadowCameraPosition = XMLNode->GetBoolProperty("use_shadow_camera_position", false, false);
	m_UseInverseViewMatrix = XMLNode->GetBoolProperty("use_view_inverse_matrix", false, false);
	m_UseInverseWorldMatrix = XMLNode->GetBoolProperty("use_world_inverse_matrix", false, false);
	m_UseInverseProjMatrix = XMLNode->GetBoolProperty("use_proj_inverse_matrix", false, false);
	m_UseCameraPosition = XMLNode->GetBoolProperty("use_camera_position", false, false);
	m_UseLights = XMLNode->GetBoolProperty("use_lights", false, false);
	m_NumOfLights = static_cast<uint32>( XMLNode->GetIntProperty("num_of_lights", 0, false) );

	std::string l_EffectName = XMLNode->GetPszProperty("effect", "");
	m_Effect = CORE->GetEffectManager()->GetEffect(l_EffectName);

	Refresh();
}

CEffectTechnique::~CEffectTechnique()
{
	m_Effect = NULL;
}

bool CEffectTechnique::BeginRender()
{
	LPD3DXEFFECT l_Effect = m_Effect->GetD3DEffect();
	if( l_Effect == NULL )
	{
		return false;
	}

	CEffectManager *l_EffectManager = CORE->GetEffectManager();
	if( m_UseWorldMatrix )
	{
		l_Effect->SetMatrix( m_Effect->GetWorldMatrix(), &l_EffectManager->GetWorldMatrix().GetD3DXMatrix() );
	}

	if( m_UseViewMatrix )
	{
		l_Effect->SetMatrix( m_Effect->GetViewMatrix(), &l_EffectManager->GetViewMatrix().GetD3DXMatrix() );
	}

	if( m_UseProjMatrix )
	{
		l_Effect->SetMatrix( m_Effect->GetProjectionMatrix(), &l_EffectManager->GetProjectionMatrix().GetD3DXMatrix() );
	}

	if( m_UseWorldViewMatrix )
	{
		Mat44f l_WorldMatrix = l_EffectManager->GetWorldMatrix();
		Mat44f l_ViewMatrix = l_EffectManager->GetViewMatrix();
		l_WorldMatrix = l_WorldMatrix * l_ViewMatrix;

		l_Effect->SetMatrix( m_Effect->GetWorldViewMatrix(), &l_WorldMatrix.GetD3DXMatrix() );

	}

	if( m_UseViewProjectionMatrix )
	{
		l_Effect->SetMatrix( m_Effect->GetViewProjectionMatrix(), &l_EffectManager->GetViewProjectionMatrix().GetD3DXMatrix() );
	}

	if( m_UseWorldViewProjectionMatrix )
	{
		Mat44f l_ViewProjMatrix = l_EffectManager->GetViewProjectionMatrix();
		Mat44f l_WorldMatrix = l_EffectManager->GetWorldMatrix();
		l_WorldMatrix =  l_ViewProjMatrix*l_WorldMatrix;

		l_Effect->SetMatrix( m_Effect->GetWorldViewProjectionMatrix(), &l_WorldMatrix.GetD3DXMatrix() );
	}

	if( m_UseShadowViewProjectionMatrix )
	{
		Mat44f l_LightViewMatrix = l_EffectManager->GetShadowViewMatrix();
		Mat44f l_ProjMatrix = l_EffectManager->GetProjectionMatrix();
		l_LightViewMatrix = l_LightViewMatrix * l_ProjMatrix;

		l_Effect->SetMatrix( m_Effect->GetShadowViewProjectionMatrix(), &l_LightViewMatrix.GetD3DXMatrix() );
	}

	if( m_UseShadowCameraPosition )
	{
		Vect3f l_CameraEye = l_EffectManager->GetShadowCameraEye();
		float l_Camera[3];
		l_Camera[0] = l_CameraEye.x;
		l_Camera[1] = l_CameraEye.y;
		l_Camera[2] = l_CameraEye.z;

		l_Effect->SetFloatArray( m_Effect->GetShadowCameraPositionMatrix(), l_Camera, 3);
	}

	if( m_UseInverseViewMatrix )
	{
		Mat44f l_ViewInverseMatrix = l_EffectManager->GetViewInverseMatrix();

		l_Effect->SetMatrix( m_Effect->GetViewInverseMatrix(), &l_ViewInverseMatrix.GetD3DXMatrix() );
	}

	if( m_UseCameraPosition )
	{
		Vect3f l_CameraEye = l_EffectManager->GetCameraEye();
		float l_Camera[3];
		l_Camera[0] = l_CameraEye.x;
		l_Camera[1] = l_CameraEye.y;
		l_Camera[2] = l_CameraEye.z;

		l_Effect->SetFloatArray( m_Effect->GetCameraPositionMatrix(), l_Camera, 3);
	}

	if( m_UseLights )
	{
		const BOOL *l_Enabled = m_Effect->GetLightEnabled();
		const int *l_Type = m_Effect->GetLightType();
		const float *l_Angle = m_Effect->GetLightAngle();
		const float *l_FallOff = m_Effect->GetLightFallOff();
		const float *l_StartAtt = m_Effect->GetLightStartAtt();
		const float *l_EndAtt = m_Effect->GetLightEndAtt();
		const Vect3f *l_Pos = m_Effect->GetLightPosition();
		const Vect3f *l_Dir = m_Effect->GetLightDirection();
		const Vect3f *l_Color = m_Effect->GetLightColor();

		l_Effect->SetInt( m_Effect->GetNumLights(), m_NumOfLights );

		l_Effect->SetBoolArray( m_Effect->GetLightEnabledMatrix(), l_Enabled, m_NumOfLights);
		l_Effect->SetIntArray( m_Effect->GetLightsTypeMatrix(), l_Type, m_NumOfLights);
		l_Effect->SetFloatArray( m_Effect->GetLightsAngleMatrix(), l_Angle, m_NumOfLights);
		l_Effect->SetFloatArray( m_Effect->GetLightsFallOffMatrix(), l_FallOff, m_NumOfLights);
		l_Effect->SetFloatArray( m_Effect->GetLightsStartRangeMatrix(), l_StartAtt, m_NumOfLights);
		l_Effect->SetFloatArray( m_Effect->GetLightsEndRangeMatrix(), l_EndAtt, m_NumOfLights);
		l_Effect->SetFloatArray( m_Effect->GetLightsPositionMatrix(), &l_Pos[0].x, 3*m_NumOfLights);
		l_Effect->SetFloatArray( m_Effect->GetLightsDirectionMatrix(), &l_Dir[0].x, 3*m_NumOfLights);
		l_Effect->SetFloatArray( m_Effect->GetLightsColorMatrix(), &l_Color[0].x, 3*m_NumOfLights);
	}

	if( m_UseLightAmbientColor )
	{
		//TODO: ¿Que se la pasa aquí?
	}

	if( m_UseTime )
	{
		//TODO: ¿Que se la pasa aquí?
	}

	return true;
}

bool CEffectTechnique::Refresh()
{
	D3DXHANDLE l_Technique = m_Effect->GetTechniqueByName( m_TechniqueName );

	if( l_Technique != NULL )
	{
		m_D3DTechnique = l_Technique;
		return true;
	}

	return false;
}