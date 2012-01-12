#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Effect.h"
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
	m_UseViewProjectionMatrix = XMLNode->GetBoolProperty("use_world_view_projection_matrix", false, false);
	m_UseProjMatrix = XMLNode->GetBoolProperty("use_projection_matrix", false, false);
	m_UseWorldViewMatrix = XMLNode->GetBoolProperty("use_world_view_matrix", false, false);
	m_UseViewToLightProjectionMatrix = XMLNode->GetBoolProperty("use_view_to_light_projection_matrix", false, false);
	m_UseCameraPosition = XMLNode->GetBoolProperty("use_camera_position", false, false);
	m_UseLights = XMLNode->GetBoolProperty("use_lights", false, false);
	m_NumOfLights = static_cast<uint32>( XMLNode->GetIntProperty("num_of_lights", 0) );

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
	l_Effect->SetBool( m_Effect->GetWorldMatrix(), m_UseWorldMatrix );
	l_Effect->SetBool( m_Effect->GetViewMatrix(), m_UseViewMatrix );
	l_Effect->SetBool( m_Effect->GetProjectionMatrix(), m_UseProjMatrix );
	l_Effect->SetBool( m_Effect->GetWorldViewMatrix(), m_UseWorldViewMatrix );
	l_Effect->SetBool( m_Effect->GetViewProjectionMatrix(), m_UseViewProjectionMatrix );
	l_Effect->SetBool( m_Effect->GetWorldViewProjectionMatrix(), m_UseWorldViewProjectionMatrix );
	l_Effect->SetBool( m_Effect->GetViewToLightProjectionMatrix(), m_UseViewToLightProjectionMatrix );
	l_Effect->SetBool( m_Effect->GetCameraPositionMatrix(), m_UseCameraPosition );
	l_Effect->SetBool( m_Effect->GetLightEnabledMatrix(), m_UseLights );
	l_Effect->SetBool( m_Effect->GetLightsColorMatrix(), m_UseLightAmbientColor );
	l_Effect->SetBool( m_Effect->GetTimeMatrix(), m_UseTime );

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