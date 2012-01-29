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

	typedef std::vector<CPoolRenderableObjectTechniqueElement*>		TPoolROTechinqueElement;
	TPoolROTechinqueElement											m_RenderableObjectTechniqueElementsVector;

public:
	CPoolRenderableObjectTechnique(CXMLTreeNode &Node);
	virtual ~CPoolRenderableObjectTechnique();

	void		Destroy			();
	void		AddElement		(const std::string &Name, const std::string &TechniqueName, 
								CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager);
	void		Apply			();
};

#endif
