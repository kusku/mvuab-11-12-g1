
#include <assert.h>

#include "OmniLight.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Math\Matrix44.h"
#include "XML\XMLTreeNode.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

COmniLight::COmniLight()
{
	m_Type = OMNI;
}

COmniLight::COmniLight(CXMLTreeNode &XMLNode)
{
	m_Type = OMNI;
	m_Name = XMLNode.GetPszProperty("name", "");
	m_Position = XMLNode.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f));
	m_StartRangeAttenuation = XMLNode.GetFloatProperty("att_start_range", 0.0f);
	m_EndRangeAttenuation = XMLNode.GetFloatProperty("att_end_range", 0.0f);
	m_RenderShadows = XMLNode.GetBoolProperty("render_shadows", false);
	Vect3f l_Color = XMLNode.GetVect3fProperty("color", Vect3f(0.0f, 0.0f, 0.0f));
	
	m_Color = CColor( l_Color.x, l_Color.y, l_Color.z );

	//Shadow Map Params
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

COmniLight::~COmniLight()
{
}

void COmniLight::Render(CRenderManager *RM)
{
	Mat44f mat;
	mat.SetIdentity();
	mat.Translate( GetPosition() );

	RM->SetTransform(mat);
	RM->DrawSphere( 0.3f, 5, colYELLOW );
}

void COmniLight::SetShadowMap(CRenderManager *RM)
{
	assert(!"Not implemented");
}