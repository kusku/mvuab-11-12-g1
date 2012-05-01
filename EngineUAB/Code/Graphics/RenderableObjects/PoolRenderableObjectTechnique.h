#pragma once

#ifndef _POOL_RENDERABLE_OBJECT_TECHNIQUE_H
#define _POOL_RENDERABLE_OBJECT_TECHNIQUE_H

#include "RenderableObjectTechnique.h"
#include "Utils\Named.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include <vector>

class CEffectTechnique;

class CPoolRenderableObjectTechnique : public CNamed
{
private:
	class CPoolRenderableObjectTechniqueElement
	{
	public:
		CRenderableObjectTechnique		m_RenderableObjectTechnique;
		CRenderableObjectTechnique		*m_pOnRenderableObjectTechniqueManager;

		CPoolRenderableObjectTechniqueElement();
		CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique,
			CRenderableObjectTechnique *OnRenderableObjectTechniqueManager);

		virtual ~CPoolRenderableObjectTechniqueElement();
	};

	//----Membres --------------------------------------------------------
	typedef std::vector<CPoolRenderableObjectTechniqueElement*>		TPoolROTechniqueElement;
	TPoolROTechniqueElement											m_RenderableObjectTechniqueElementsVector;

public:
	//--- Init and End protocols------------------------------------------
			CPoolRenderableObjectTechnique	( CXMLTreeNode &_Node );
	virtual ~CPoolRenderableObjectTechnique ( void );

	//----Funcions principals---------------------------------------------
	void	AddElement						( const std::string &_Name, const std::string &_TechniqueName, CRenderableObjectTechnique *_ROTOnRenderableObjectTechniqueManager );
	void	Apply							( void );
	void	Destroy							( void );
};

#endif
