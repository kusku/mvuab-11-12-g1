
#include <map>

#include "EffectTechniqueCollection.h"
#include "EffectTechnique.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffectTechniqueCollection::CEffectTechniqueCollection()
{
}

CEffectTechniqueCollection::~CEffectTechniqueCollection()
{
	this->Destroy();
}