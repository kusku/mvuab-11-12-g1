#include "Light.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

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