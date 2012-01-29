#include "RenderableObjectTechniqueManager.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderableObjectTechniqueManager::CRenderableObjectTechniqueManager()
{
}


CRenderableObjectTechniqueManager::~CRenderableObjectTechniqueManager()
{
	Destroy();
}

void CRenderableObjectTechniqueManager::Destroy()
{
	CTemplatedVectorMapManager<CRenderableObjectTechnique>::Destroy();
	m_PoolRenderableObjectTechniques.Destroy();
}

void CRenderableObjectTechniqueManager::Load(const std::string &FileName)
{
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(uint32 VertexType)
{
	//TODO: Mirar que tiene que devolver. ¿Qualquier string constuido por nosotros?
	return "";
}

void CRenderableObjectTechniqueManager::InsertRendrableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName)
{
	CEffectTechnique *l_EffectTechnique = CORE->GetEffectManager()->GetEffectTechnique(TechniqueName);
	CRenderableObjectTechnique *l_ROT = new CRenderableObjectTechnique(ROTName, l_EffectTechnique);

	AddResource(ROTName, l_ROT);

	l_ROT = NULL;
}
