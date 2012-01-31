#include "PoolRenderableObjectTechnique.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &Node)
{
}


CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
	Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
	TPoolROTechinqueElement::iterator l_It = m_RenderableObjectTechniqueElementsVector.begin();
	TPoolROTechinqueElement::iterator l_End = m_RenderableObjectTechniqueElementsVector.end();
	for(; l_It != l_End; ++l_It)
	{
		CHECKED_DELETE(*l_It);
	}
	m_RenderableObjectTechniqueElementsVector.clear();
}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, 
												CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
	CEffectTechnique *l_EffectTechnique = CORE->GetEffectManager()->GetEffectTechnique(TechniqueName);

	CPoolRenderableObjectTechniqueElement *l_PoolROTE = 
		new CPoolRenderableObjectTechniqueElement(Name, l_EffectTechnique, ROTOnRenderableObjectTechniqueManager);

	m_RenderableObjectTechniqueElementsVector.push_back( l_PoolROTE );

	l_PoolROTE = NULL;
}

void CPoolRenderableObjectTechnique::Apply()
{
	CEffectTechnique *l_EffectTechnique = NULL;

	TPoolROTechinqueElement::iterator l_It = m_RenderableObjectTechniqueElementsVector.begin();
	TPoolROTechinqueElement::iterator l_End = m_RenderableObjectTechniqueElementsVector.end();
	for(; l_It != l_End; ++l_It)
	{
		(*l_It)->m_pOnRenderableObjectTechniqueManager->SetEffectTechnique( (*l_It)->m_RenderableObjectTechnique.GetEffectTechnique() );
	}
}

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement()
{
}

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement(const std::string &Name,
	CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager)
	: m_pOnRenderableObjectTechniqueManager(OnRenderableObjectTechniqueManager)
	, m_RenderableObjectTechnique(Name, EffectTechnique)
{
}

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::~CPoolRenderableObjectTechniqueElement()
{
	m_pOnRenderableObjectTechniqueManager = NULL;
}