
#include "AnimalManager.h"
#include "Movement\WayPoint.h"
#include "Movement\WayPointManager.h"
#include "Core.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimalManager::CAnimalManager()
{

}

CAnimalManager::~CAnimalManager()
{

}

bool CAnimalManager::Load(const std::string& filename)
{
	m_Filename = filename;

	return Reload();
}

bool CAnimalManager::Reload()
{
	m_AnimalToWPGMap.clear();
	m_AnimalToWPMap.clear();

	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "CAnimalManager::Load->Error al intentar leer el archivo xml: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}	

	CXMLTreeNode l_wps = newFile["animal_movement"];
	if( l_wps.Exists() )
	{
		uint16 l_Count = l_wps.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_wps(i).GetName();

			if( l_Type == "animal" )
			{
				CXMLTreeNode groupChild = l_wps(i); 

				std::string layerName = groupChild.GetPszProperty("layer", "", true);
				std::string wpGroup = groupChild.GetPszProperty("way_point_group", "", true);

				if(m_AnimalToWPGMap.find(layerName) != m_AnimalToWPGMap.end())
				{
					assert(!"Animal Layer already in Way Group");
				}

				m_AnimalToWPGMap[layerName] = wpGroup;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

void CAnimalManager::CleanUp()
{
	m_AnimalToWPGMap.clear();
	m_AnimalToWPMap.clear();
}

void CAnimalManager::Update(float elapsedTime)
{
	AnimalToWPGIt it = m_AnimalToWPGMap.begin();
	AnimalToWPGIt itEnd = m_AnimalToWPGMap.end();

	CWayPointManager* wpm = CORE->GetWayPointManager();

	for(; it != itEnd; ++it)
	{
		CRenderableObjectsManager* rom = CORE->GetRenderableObjectsLayersManager()->GetResource(it->first);

		if(rom == NULL)
		{
			continue;
		}
		
		std::map<std::string, CRenderableObject*>& roMap = rom->GetResourceMap();

		std::map<std::string, CRenderableObject*>::iterator itRO, itROEnd;

		itRO = roMap.begin();
		itROEnd = roMap.end();

		for(; itRO != itROEnd; ++itRO)
		{
			CRenderableObject* ro = itRO->second;

			if(!ro->GetVisible())
			{
				continue;
			}

			if(m_AnimalToWPMap.find(ro->GetName()) == m_AnimalToWPMap.end())
			{
				m_AnimalToWPMap[ro->GetName()] = AnimalPath();
			}

			if(m_AnimalToWPMap[ro->GetName()].m_Current == m_AnimalToWPMap[ro->GetName()].m_Path.size())
			{
				CWayPoint* closestWP = wpm->GetClosestWayPoint(it->second, ro->GetPosition());
				CWayPoint* destPoint = wpm->GetRandomWayPoint(it->second);

				m_AnimalToWPMap[ro->GetName()].m_Current = 0;
				m_AnimalToWPMap[ro->GetName()].m_Path = wpm->GetPath(it->second, closestWP->GetPosition(), destPoint->GetPosition());
			}
		}		
	}
}

void CAnimalManager::Update(float elapsedTime, const std::string& name)
{

}