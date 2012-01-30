#include "RenderableObjectTechniqueManager.h"
#include "RenderableObjectTechnique.h"
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

	CXMLTreeNode l_ROTs = newFile["renderable_objects_techniques"];
	if( l_ROTs.Exists() )
	{
		uint16 l_Count = l_ROTs.GetNumChildren();
		for( uint16 i=0; i < l_Count; ++i)
		{
			std::string l_Type = l_ROTs(i).GetName();
			if( l_Type == "pool_renderable_object_technique" )
			{
				std::string l_PROTName = l_ROTs(i).GetPszProperty("name", "");
				CPoolRenderableObjectTechnique *l_PoolROT = new CPoolRenderableObjectTechnique(l_ROTs(i));
				m_PoolRenderableObjectTechniques.AddResource(l_PROTName, l_PoolROT);

				uint16 l_Child = l_ROTs(i).GetNumChildren();
				for(uint16 j=0; j < l_Child; ++j)
				{
					l_Type = l_ROTs(i)(j).GetName();
					if( l_Type == "default_technique" )
					{
						int l_VertexType = l_ROTs(i)(j).GetIntProperty("vertex_type", 0);
						std::string l_Technique = l_ROTs(i)(j).GetPszProperty("technique", "");

						std::string l_Name = GetRenderableObjectTechniqueNameByVertexType( static_cast<uint32>(l_VertexType) );

						InsertRenderableObjectTechnique(l_Name, l_Technique);
						
						l_PoolROT->AddElement(l_Name, l_Technique, GetResource(l_Name) );
					}
					else if( l_Type == "technique" )
					{
						std::string l_Name = l_ROTs(i)(j).GetPszProperty("name", "");
						std::string l_Technique = l_ROTs(i)(j).GetPszProperty("technique", "");

						InsertRenderableObjectTechnique(l_Name, l_Technique);

						l_PoolROT->AddElement(l_Name, l_Technique, GetResource(l_Name) );
					}
				}
				
			}
		}
	}
}

std::string CRenderableObjectTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(uint32 VertexType)
{
	std::string l_Name = "DefaultROTTechnique_" + VertexType;
	return l_Name;
}

void CRenderableObjectTechniqueManager::InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName)
{
	CEffectTechnique *l_EffectTechnique = CORE->GetEffectManager()->GetEffectTechnique(TechniqueName);
	CRenderableObjectTechnique *l_ROT = new CRenderableObjectTechnique(ROTName, l_EffectTechnique);

	AddResource(ROTName, l_ROT);

	l_ROT = NULL;
}
