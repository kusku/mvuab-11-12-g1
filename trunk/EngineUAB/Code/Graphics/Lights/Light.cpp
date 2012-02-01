#include "Light.h"
#include "Effects\Effect.h"
#include "RenderManager.h"
#include "Textures\Texture.h"

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
		//TODO: Falta terminar el código
	}
}