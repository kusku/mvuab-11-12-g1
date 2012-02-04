#include "Light.h"
#include "Effects\Effect.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "XML\XMLTreeNode.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CLight::CLight()
	: m_Color(colWHITE)
	, m_Type(OMNI)
	, m_StartRangeAttenuation(0.0f)
	, m_EndRangeAttenuation(0.0f)
	, m_GenerateDynamicShadowMap(false)
	, m_GenerateStaticShadowMap(false)
	, m_MustUpdateStaticShadowMap(false)
	, m_pStaticShadowMap(NULL)
	, m_pDynamicShadowMap(NULL)
	, m_pShadowMaskTexture(NULL)
{
}

CLight::~CLight()
{
}

void CLight::BeginRenderEffectManagerShadowMap(CEffect *Effect)
{
}

void CLight::GenerateShadowMap(CRenderManager *RM)
{
	SetShadowMap(RM);

	if( m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap )
	{
		m_pStaticShadowMap->SetAsRenderTarget(0);
		RM->BeginRendering();
		for(size_t i=0; i<m_StaticShadowMapRenderableObjectsManagers.size(); ++i)
		{
			m_StaticShadowMapRenderableObjectsManagers[i]->Render(RM);
		}

		m_MustUpdateStaticShadowMap = false;
		RM->EndRendering();
		m_pStaticShadowMap->UnsetAsRenderTarget(0);
	}

	if( m_DynamicShadowMapRenderableObjectsManagers.size() > 0)
	{
		m_pDynamicShadowMap->SetAsRenderTarget(0);
		RM->BeginRendering();
		for(size_t i=0; i<m_DynamicShadowMapRenderableObjectsManagers.size(); ++i)
		{
			m_DynamicShadowMapRenderableObjectsManagers[i]->Render(RM);
		}

		RM->EndRendering();
		m_pDynamicShadowMap->UnsetAsRenderTarget(0);
	}
}

void CLight::ExtractCommonLightInfo(CXMLTreeNode &XMLNode)
{
	//Common Info por all Lights

	//Name and Position
	m_Name = XMLNode.GetPszProperty("name", "");
	m_Position = XMLNode.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f));

	//Attenuation
	m_StartRangeAttenuation = XMLNode.GetFloatProperty("att_start_range", 0.0f);
	m_EndRangeAttenuation = XMLNode.GetFloatProperty("att_end_range", 0.0f);

	//Color
	Vect3f l_Color = XMLNode.GetVect3fProperty("color", Vect3f(0.0f, 0.0f, 0.0f));
	m_Color = CColor( l_Color.x, l_Color.y, l_Color.z );

	//Shadows
	m_GenerateDynamicShadowMap = XMLNode.GetBoolProperty("generate_shadow_map", false, false);
	m_GenerateStaticShadowMap = XMLNode.GetBoolProperty("generate_static_shadow_map", false, false);

	if( m_GenerateDynamicShadowMap )
	{
		std::string l_DynamicType = XMLNode.GetPszProperty("shadow_map_format_type", "");
		uint32 l_WidthDynamicShadowMap = XMLNode.GetIntProperty("shadow_map_width", 512);
		uint32 l_HeightDynamicShadowMap = XMLNode.GetIntProperty("shadow_map_height", 512);

		m_pDynamicShadowMap = new CTexture();
		CTexture::TFormatType l_Format = m_pDynamicShadowMap->GetFormatTypeFromString(l_DynamicType);
		m_pDynamicShadowMap->Create(m_Name, l_WidthDynamicShadowMap, l_HeightDynamicShadowMap, 3, 
			CTexture::DYNAMIC, CTexture::DEFAULT, l_Format);
	}

	if( m_GenerateStaticShadowMap )
	{
		std::string l_StaticType = XMLNode.GetPszProperty("static_shadow_map_format_type", "");
		uint32 l_WidthStaticShadowMap = XMLNode.GetIntProperty("static_shadow_map_width", 512);
		uint32 l_HeightStaticShadowMap = XMLNode.GetIntProperty("static_shadow_map_height", 512);

		m_pStaticShadowMap = new CTexture();
		CTexture::TFormatType l_Format = m_pStaticShadowMap->GetFormatTypeFromString(l_StaticType);
		m_pStaticShadowMap->Create(m_Name, l_WidthStaticShadowMap, l_HeightStaticShadowMap, 3, 
			CTexture::DYNAMIC, CTexture::DEFAULT, l_Format);
	}
}