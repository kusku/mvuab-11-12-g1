#include "StaticMeshManager.h"
#include "xml\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "StaticMesh.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CStaticMeshManager::CStaticMeshManager()
	: m_FileName("")
{
}


CStaticMeshManager::~CStaticMeshManager()
{
	Destroy();
}

bool CStaticMeshManager::Load(const std::string &FileName)
{
	m_FileName = FileName;
	return LoadXML();
}

bool CStaticMeshManager::LoadXML()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CStaticMeshManager::LoadXML->Error al intentar leer el archivo de static meshes: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		//throw CException(__FILE__, __LINE__, msg_error);
		
		return false;
	}

	CXMLTreeNode l_StaticMeshes = newFile["static_meshes"];
	if( !l_StaticMeshes.Exists() )
	{
		return false;
	}
	else
	{
		uint16 l_Count = l_StaticMeshes.GetNumChildren();
		for( uint16 i = 0; i < l_Count; ++i )
		{
			std::string l_Name = l_StaticMeshes(i).GetName();
			if( l_Name == "static_mesh" )
			{
				l_Name = l_StaticMeshes(i).GetPszProperty("name", "");
				std::string l_Path = l_StaticMeshes(i).GetPszProperty("filename", "");

				CStaticMesh* l_StaticMesh = new CStaticMesh();
				bool state = l_StaticMesh->Load(l_Path);
				if( !state )
				{
					std::string msg_error = "CStaticMeshManager::LoadXML->No se ha podido hacer el Load de un Static Mesh con nombre: " + l_Name 
											+ " y path: " + l_Path;
					LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
					CHECKED_DELETE(l_StaticMesh);
				}
				else
				{
					AddResource(l_Name, l_StaticMesh);
				}
				l_StaticMesh = NULL;
			}
		}
	}

	return true;
}

bool CStaticMeshManager::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CStaticMeshManager: Reload de las Static Meshes.");
	Destroy();
	return LoadXML();
}
