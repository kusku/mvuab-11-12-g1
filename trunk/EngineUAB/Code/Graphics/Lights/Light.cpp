#include "Light.h"
#include "Effects\Effect.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "RenderableObjects\RenderableObjectsManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CLight::CLight()
	: m_Color(colWHITE)
	, m_Type(OMNI)
	, m_RenderShadows(false)
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