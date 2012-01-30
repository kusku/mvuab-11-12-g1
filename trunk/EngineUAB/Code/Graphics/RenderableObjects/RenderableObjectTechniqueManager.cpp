#include "RenderableObjectTechniqueManager.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
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
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CRenderableObjectTechniqueManager::Load->Error al intentar leer el archivo de renderable objects techniques: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	CXMLTreeNode l_ROT = newFile["renderable_objects_techniques"];
	if( l_ROT.Exists() )
	{
		uint16 l_Count = l_ROT.GetNumChildren();
		for( uint16 i=0; i < l_Count; ++i)
		{
			//TODO: Acabar el lector.
		}
	}
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
