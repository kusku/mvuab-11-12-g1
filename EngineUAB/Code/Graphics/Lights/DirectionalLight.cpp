#include "DirectionalLight.h"
#include "RenderManager.h"

CDirectionalLight::CDirectionalLight()
	: m_Direction(Vect3f(0.f, 0.f, 0.f))
{
}

CDirectionalLight::~CDirectionalLight()
{
}

void CDirectionalLight::Render(CRenderManager *RM)
{
}
