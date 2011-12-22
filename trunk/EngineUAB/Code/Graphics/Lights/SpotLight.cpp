#include "SpotLight.h"
#include "RenderManager.h"

CSpotLight::CSpotLight()
	: m_Angle(0.0f)
	, m_FallOff(0.0f)
{
}

CSpotLight::~CSpotLight()
{
}

void CSpotLight::Render(CRenderManager *RM)
{
}
