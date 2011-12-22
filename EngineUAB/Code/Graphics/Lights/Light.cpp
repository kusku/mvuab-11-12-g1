#include "Light.h"


CLight::CLight()
	: m_Color(colWHITE)
	, m_Type(OMNI)
	, m_RenderShadows(false)
	, m_StartRangeAttenuation(0.0f)
	, m_EndRangeAttenuation(0.0f)
{
}

CLight::~CLight()
{
}

bool CLight::RenderShadows() const
{
	return true;
}