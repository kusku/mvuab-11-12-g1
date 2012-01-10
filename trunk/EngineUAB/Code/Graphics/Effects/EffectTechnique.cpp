#include "EffectTechnique.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffectTechnique::CEffectTechnique()
	: m_UseCameraPosition(false)
	, m_UseInverseProjMatrix(false)
	, m_UseInverseViewMatrix(false)
	, m_UseInverseWorldMatrix(false)
	, m_UseLights(false)
	, m_NumOfLights(0)
	, m_UseLightAmbientColor(false)
	, m_UseProjMatrix(false)
	, m_UseViewMatrix(false)
	, m_UseWorldMatrix(false)
	, m_UseWorldViewMatrix(false)
	, m_UseWorldViewProjectionMatrix(false)
	, m_UseViewProjectionMatrix(false)
	, m_UseViewToLightProjectionMatrix(false)
	, m_UseTime(false)
	, m_Effect(NULL)
	, m_D3DTechnique(NULL)
	, m_TechniqueName("")
{
}


CEffectTechnique::~CEffectTechnique()
{
}

bool CEffectTechnique::BeginRender()
{
	return true;
}

bool CEffectTechnique::Refresh()
{
	return true;
}