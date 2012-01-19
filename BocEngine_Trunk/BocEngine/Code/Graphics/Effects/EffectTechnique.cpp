#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Effect.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffectTechnique::CEffectTechnique(CEffect*	m_ParentEffect, D3DXHANDLE effectTechnique, const std::string techniqueName)
{
	m_ParentEffect = m_ParentEffect;
	m_D3DTechnique = effectTechnique;
	m_TechniqueName = techniqueName;
}

CEffectTechnique::~CEffectTechnique()
{
}