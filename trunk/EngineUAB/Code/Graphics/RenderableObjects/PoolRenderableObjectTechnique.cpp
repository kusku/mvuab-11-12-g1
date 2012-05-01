#include "PoolRenderableObjectTechnique.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// *****************************************
//			CONSTRUCTOR/DESTRUCTOR
// *****************************************
CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &_Node)
{
	SetName ( _Node.GetPszProperty ( "name" ) );
}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique ( void )
{
	Destroy();
}

//CPoolRenderableObjectTechnique::CPoolRenderableObjectTechniqueElement::CPoolRenderableObjectTechniqueElement()
//{
//}

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

// *****************************************
//				MÈTODES 
// *****************************************
void CPoolRenderableObjectTechnique::AddElement ( const std::string &_Name, const std::string &_TechniqueName, CRenderableObjectTechnique *_ROTOnRenderableObjectTechniqueManager )
{
	CEffectTechnique *l_EffectTechnique = CORE->GetEffectManager()->GetEffectTechnique( _TechniqueName );

	CPoolRenderableObjectTechniqueElement *l_PoolROTE = 
		new CPoolRenderableObjectTechniqueElement( _Name, l_EffectTechnique, _ROTOnRenderableObjectTechniqueManager );

	m_RenderableObjectTechniqueElementsVector.push_back( l_PoolROTE );

	l_PoolROTE = NULL;
}

void CPoolRenderableObjectTechnique::Apply()
{
	CEffectTechnique *l_EffectTechnique = NULL;

	TPoolROTechniqueElement::iterator l_It	= m_RenderableObjectTechniqueElementsVector.begin ();
	TPoolROTechniqueElement::iterator l_End = m_RenderableObjectTechniqueElementsVector.end ();
	for(; l_It != l_End; ++l_It)
	{
		// assignem cada CEffectTechnique del ROTechnique al onROTechniqueManager	
		(*l_It)->m_pOnRenderableObjectTechniqueManager->SetEffectTechnique( (*l_It)->m_RenderableObjectTechnique.GetEffectTechnique() );
	}
}



void CPoolRenderableObjectTechnique::Destroy()
{
	TPoolROTechniqueElement::iterator l_It = m_RenderableObjectTechniqueElementsVector.begin();
	TPoolROTechniqueElement::iterator l_End = m_RenderableObjectTechniqueElementsVector.end();
	for(; l_It != l_End; ++l_It)
	{
		CHECKED_DELETE(*l_It);
	}
	m_RenderableObjectTechniqueElementsVector.clear();
}
