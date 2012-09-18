
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
	
	//Extract Common Info
	ExtractCommonLightInfo(XMLNode);
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
	RM->DrawSphere( 0.4f, 6, colYELLOW );
}

void COmniLight::SetShadowMap()
{
	return;
}

void COmniLight::RenderShadows(CRenderManager *RM)
{
	return;
}

void COmniLight::CreateShadowTextures(bool staticMap, CTexture::TFormatType formatStatic, uint32 staticWidth, uint32 staticHeight, bool dynamicMap, CTexture::TFormatType formatDynamic, uint32 dynamicWidth, uint32 dynamicHeight)
{

}
