#include "OmniLight.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

COmniLight::COmniLight()
{
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