
#include <map>

#include "EffectParameterCollection.h"
#include "EffectParameter.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffectParameterCollection::CEffectParameterCollection()
{
}

CEffectParameterCollection::~CEffectParameterCollection()
{
	this->Destroy();
}

CEffectParameter* CEffectParameterCollection::GetParameterBySemantic(const std::string& parameterSemantic)
{
    std::map<std::string, CEffectParameter*>::iterator it = m_Resources.begin();
    std::map<std::string, CEffectParameter*>::iterator itEnd = m_Resources.end();

    for(; it != itEnd; ++it)
    { 
		if(it->second->GetSemantic() == parameterSemantic)
		{
			return it->second;
		}
    }

	return NULL;
}