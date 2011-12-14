#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "../RenderManager.h"
#include "InstanceMesh.h"
#include "XML\XMLTreeNode.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderableObjectsManager::CRenderableObjectsManager()
	: m_FileName("")
{
}


CRenderableObjectsManager::~CRenderableObjectsManager()
{
	CleanUp();
}

void CRenderableObjectsManager::Update(float elapsedTime)
{
	std::vector<CRenderableObject*>::iterator l_It = m_RenderableObjects.begin();
	std::vector<CRenderableObject*>::iterator l_End = m_RenderableObjects.end();

	for(; l_It != l_End; ++l_It)
	{
		(*l_It)->Update(elapsedTime);
	}
}

void CRenderableObjectsManager::Render(CRenderManager *RM)
{
	std::vector<CRenderableObject*>::iterator l_It = m_RenderableObjects.begin();
	std::vector<CRenderableObject*>::iterator l_End = m_RenderableObjects.end();

	for(; l_It != l_End; ++l_It)
	{
		if( (*l_It)->GetVisible() )
		{
			(*l_It)->Render(RM);
		}
	}
}

CRenderableObject* CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position)
{
	CInstanceMesh *l_InstanceMesh = new CInstanceMesh(InstanceName, CoreMeshName);
	l_InstanceMesh->SetPosition( Position );

	AddResource( InstanceName, static_cast<CRenderableObject*>(l_InstanceMesh) );

	return static_cast<CRenderableObject*>(l_InstanceMesh);
}

bool CRenderableObjectsManager::AddResource(const std::string &Name, CRenderableObject *RenderableObject)
{
	if( CMapManager<CRenderableObject>::AddResource(Name, RenderableObject) )
	{
		m_RenderableObjects.push_back(RenderableObject);
		return true;
	}
	return false;
}

CRenderableObject* CRenderableObjectsManager::GetInstance(const std::string &Name)
{
	return GetResource(Name);
}

void CRenderableObjectsManager::CleanUp()
{
	Destroy();
	m_RenderableObjects.clear();
}

void CRenderableObjectsManager::Load(const std::string &FileName)
{
	m_FileName = FileName;
	LoadFile();
}

void CRenderableObjectsManager::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CRenderableObjectsManager::Reload -> Reload de los Renderable Objects.");
	CleanUp();
	LoadFile();
}

void CRenderableObjectsManager::LoadFile()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CRenderableObjectsManager::Load->Error al intentar leer el archivo de renderable objects: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		//throw CException(__FILE__, __LINE__, msg_error);
	}

	CXMLTreeNode l_RObjects = newFile["renderable_objects"];
	if( l_RObjects.Exists() )
	{
		uint16 l_Count = l_RObjects.GetNumChildren();
		for( uint16 i=0; i < l_Count; ++i)
		{
			std::string l_Type = l_RObjects(i).GetName();
			if( l_Type == "mesh_instance" )
			{
				std::string l_Name = l_RObjects(i).GetPszProperty("name", "");
				std::string l_Core = l_RObjects(i).GetPszProperty("core", "");
				Vect3f l_Position = l_RObjects(i).GetVect3fProperty("position", Vect3f(0.0, 0.0, 0.0));
				float l_Yaw = l_RObjects(i).GetFloatProperty("yaw", 0.0f);
				float l_Pitch = l_RObjects(i).GetFloatProperty("pitch", 0.0f);
				float l_Roll = l_RObjects(i).GetFloatProperty("roll", 0.0f);

				CInstanceMesh *l_InstanceMesh = new CInstanceMesh(l_Name, l_Core);
				l_InstanceMesh->SetPosition(l_Position);
				l_InstanceMesh->SetYaw(l_Yaw);
				l_InstanceMesh->SetPitch(l_Pitch);
				l_InstanceMesh->SetRoll(l_Roll);

				AddResource( l_Name, static_cast<CRenderableObject*>(l_InstanceMesh) );
			}
		}
	}
}