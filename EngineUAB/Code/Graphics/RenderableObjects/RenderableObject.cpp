#include "RenderableObject.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderableObject::CRenderableObject()
	: m_pROTechnique(NULL)
{
}


CRenderableObject::~CRenderableObject()
{
}

void CRenderableObject::Update(float elapsedTime)
{
}