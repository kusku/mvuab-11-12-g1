#include "RenderableObjectTechnique.h"
#include "Effects\EffectTechnique.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderableObjectTechnique::CRenderableObjectTechnique()
{
}

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique)
	: m_pEffectTechnique(EffectTechnique)
{
	SetName(Name);
}


CRenderableObjectTechnique::~CRenderableObjectTechnique()
{
	m_pEffectTechnique = NULL;
}
