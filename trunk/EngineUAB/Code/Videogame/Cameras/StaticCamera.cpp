#include "StaticCamera.h"
#include "Cameras\Camera.h"
#include "RenderManager.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CStaticCamera::CStaticCamera()
{
}

CStaticCamera::~CStaticCamera()
{
}

void CStaticCamera::Update(float _ElapsedTime, CCamera *camera)
{
}

void CStaticCamera::Render(CRenderManager *RM)
{
}
