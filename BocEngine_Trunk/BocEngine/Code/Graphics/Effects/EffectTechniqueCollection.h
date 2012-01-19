
#pragma once

#ifndef EFFECT_TECHNIQUE_COLLECTION_H
#define EFFECT_TECHNIQUE_COLLECTION_H

#include <string>

#include "Utils\MapManager.h"

class CEffectTechnique;

class CEffectTechniqueCollection : public CMapManager<CEffectTechnique>
{
public:
	CEffectTechniqueCollection();
	virtual ~CEffectTechniqueCollection();
};

#endif