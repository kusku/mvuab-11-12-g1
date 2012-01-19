
#pragma once

#ifndef EFFECT_PARAMETER_COLLECTION_H
#define EFFECT_PARAMETER_COLLECTION_H

#include <string>

#include "Utils\MapManager.h"

class CEffectParameter;

class CEffectParameterCollection : public CMapManager<CEffectParameter>
{
public:
	CEffectParameterCollection();
	virtual ~CEffectParameterCollection();

	/**Gets**/
	CEffectParameter* GetParameterBySemantic(const std::string& parameterSemantic);
};

#endif