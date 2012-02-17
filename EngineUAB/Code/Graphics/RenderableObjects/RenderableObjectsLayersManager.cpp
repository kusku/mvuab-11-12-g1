#include "RenderableObjectsLayersManager.h"
#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include "Base.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager()
	: m_pDefaultRenderableObjectManager(NULL)
{
}

CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{
	Destroy();
}

void CRenderableObjectsLayersManager::Destroy()
{
	CTemplatedVectorMapManager::Destroy();
	m_pDefaultRenderableObjectManager = NULL;
}

void CRenderableObjectsLayersManager::Load(const std::string &FileName)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CRenderableObjectsLayersManager::Load->Cargando los Renderable Objects y las layers.");

	m_FileName = FileName;
	LoadFile();
}

void CRenderableObjectsLayersManager::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CRenderableObjectsLayersManager::Reload->Reload de los Renderable Objects Layers Manager.");
	Destroy();
	LoadFile();
}

void CRenderableObjectsLayersManager::LoadFile()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CRenderableObjectsLayersManager::LoadFile->Error al intentar leer el archivo de renderable objects: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return;
	}

	CXMLTreeNode l_RObjects = newFile["renderable_objects"];
	if( l_RObjects.Exists() )
	{
		uint16 l_Count = l_RObjects.GetNumChildren();
		for( uint16 i=0; i < l_Count; ++i)
		{
			std::string l_Type = l_RObjects(i).GetName();
			if( l_Type == "layer" )
			{
				std::string l_Name = l_RObjects(i).GetPszProperty("name", "");
				bool l_Default = l_RObjects(i).GetBoolProperty("default", false, false);

				if( GetResource(l_Name) == NULL )
				{
					CRenderableObjectsManager *l_ROManager = new CRenderableObjectsManager();
					AddResource(l_Name, l_ROManager);

					if( l_Default )
					{
						m_pDefaultRenderableObjectManager = l_ROManager;
					}
				}	
			}
			else if( l_Type == "mesh_instance" )
			{
				std::string l_Layer = l_RObjects(i).GetPszProperty("layer", "", false);
				CRenderableObjectsManager *l_ROManager = NULL;
				if( l_Layer == "" )
				{
					l_ROManager = m_pDefaultRenderableObjectManager;
				}
				else
				{
					l_ROManager = GetResource(l_Layer);
				}

				if( l_ROManager != NULL )
				{
					l_ROManager->AddMeshInstance(l_RObjects(i));
				}
				else
				{
					std::string l_Name = l_RObjects(i).GetPszProperty("name", "");
					std::string err = "CRenderableObjectsLayersManager::LoadFile->No se ha podido encontrar la layer: " + l_Layer +
						" del mesh instance: " + l_Name;
					LOGGER->AddNewLog( ELL_WARNING, err.c_str() );
				}
			}
			else if( l_Type == "animated_model_instance" )
			{
				std::string l_Layer = l_RObjects(i).GetPszProperty("layer", "");
				CRenderableObjectsManager *l_ROManager = NULL;
				if( l_Layer == "" )
				{
					l_ROManager = m_pDefaultRenderableObjectManager;
				}
				else
				{
					l_ROManager = GetResource(l_Layer);
				}

				if( l_ROManager != NULL )
				{
					l_ROManager->AddAnimatedMeshInstance(l_RObjects(i));
				}
				else
				{
					std::string l_Name = l_RObjects(i).GetPszProperty("name", "");
					std::string err = "CRenderableObjectsLayersManager::LoadFile->No se ha podido encontrar la layer: " + l_Layer +
						" del animated model instance: " + l_Name;
					LOGGER->AddNewLog( ELL_WARNING, err.c_str() );
				}
				
			}
		}
	}
}

void CRenderableObjectsLayersManager::Update(float ElapsedTime)
{
	TVectorResources l_VectorResources = GetResourcesVector();

	TVectorResources::iterator l_It = l_VectorResources.begin();
	TVectorResources::iterator l_End = l_VectorResources.end();
	for(; l_It != l_End; ++l_It)
	{
		(*l_It)->Update(ElapsedTime);
	}
}

void CRenderableObjectsLayersManager::Render(CRenderManager *RM)
{
	TVectorResources l_VectorResources = GetResourcesVector();

	TVectorResources::iterator l_It = l_VectorResources.begin();
	TVectorResources::iterator l_End = l_VectorResources.end();
	for(; l_It != l_End; ++l_It)
	{
		(*l_It)->Render(RM);
	}
}

void CRenderableObjectsLayersManager::Render(CRenderManager *RM, const std::string &LayerName)
{
	CRenderableObjectsManager *l_ROManager = GetResource(LayerName);
	if( l_ROManager != NULL )
	{
		l_ROManager->Render(RM);
	}
}

CRenderableObjectsManager* CRenderableObjectsLayersManager::GetRenderableObjectManager(CXMLTreeNode &Node)
{
	return NULL;
}
