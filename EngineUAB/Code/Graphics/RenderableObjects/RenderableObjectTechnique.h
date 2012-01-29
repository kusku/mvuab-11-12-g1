#pragma once

#ifndef _RENDERABLE_OBJECT_TECHNIQUE_H
#define _RENDERABLE_OBJECT_TECHNIQUE_H

#include "Utils\Named.h"
#include <string>

class CEffectTechnique;

class CRenderableObjectTechnique : public CNamed
{
public:
	CRenderableObjectTechnique();
	CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique);
	virtual ~CRenderableObjectTechnique();

	void				SetEffectTechnique		( CEffectTechnique *EffectTechnique )	{ m_pEffectTechnique = EffectTechnique; }
	CEffectTechnique*	GetEffectTechnique		() const								{ return m_pEffectTechnique; }	

private:
	CEffectTechnique		*m_pEffectTechnique;
};

#endif
