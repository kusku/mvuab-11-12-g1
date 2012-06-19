
#include "AnimalManager.h"
#include "Movement\WayPoint.h"
#include "Movement\WayPointManager.h"
#include "Core.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\StaticMesh\InstanceMeshHW.h"

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

				m_AnimalToWPGMap[layerName] = AnimalLayerPathSettings(layerName, wpGroup);
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

			CObject3D* obj = (CObject3D*)ro;
			Update(elapsedTime, it->second, obj);
		}

		
		std::map<std::string, CInstanceMeshHW*>& hwMap = rom->GetResourceHWMap();

		std::map<std::string, CInstanceMeshHW*>::iterator itHW, itHWEnd;
		itHW = hwMap.begin();
		itHWEnd = hwMap.end();

		for (; itHW != itHWEnd; ++itHW)
		{
			CInstanceMeshHW* instanceHW = itHW->second;

			std::map<std::string, CObject3D*>& objMap = instanceHW->GetObjectMap();

			std::map<std::string, CObject3D*>::iterator itObj, itObjEnd;
			itObj = objMap.begin();
			itObjEnd = objMap.end();

			for (; itObj != itObjEnd; ++itObj)
			{
				Update(elapsedTime, it->second, itObj->second);
			}
		}
	}
}

void CAnimalManager::Update(float elapsedTime, const AnimalLayerPathSettings& settings, CObject3D* obj)
{
	std::string name = obj->GetName();

	if(m_AnimalToWPMap.find(name) == m_AnimalToWPMap.end())
	{
		m_AnimalToWPMap[name] = AnimalPath();
	}

	CWayPointManager* wpm = CORE->GetWayPointManager();

	if(m_AnimalToWPMap[name].m_Current == m_AnimalToWPMap[name].m_Path.size())
	{
		CWayPoint* closestWP = wpm->GetClosestWayPoint(settings.m_WayPointGroup, obj->GetPosition());
		CWayPoint* destPoint = wpm->GetRandomWayPoint(settings.m_WayPointGroup);

		m_AnimalToWPMap[name].m_Current = 0;
		m_AnimalToWPMap[name].m_Path = wpm->GetPath(settings.m_WayPointGroup, closestWP->GetPosition(), destPoint->GetPosition());

		if(m_AnimalToWPMap[name].m_Path.size() == 0)
		{
			return;
		}
	}

	uint32 index = m_AnimalToWPMap[name].m_Current;

	Vect3f pointA = m_AnimalToWPMap[name].m_Path[index];
	Vect3f pointB = obj->GetPosition();

	if(pointA.SqDistance(pointB) < 0.3f)
	{
		m_AnimalToWPMap[name].m_Current++;
		return;
	}

	Vect3f v = (m_AnimalToWPMap[name].m_Path[index] - obj->GetPosition());
	
	float yaw = obj->GetYaw();

	Vect3f frontV(0, 0, 0);
	frontV.GetXZFromAngle(yaw);

	float back = v.Dot(frontV);
	
	if(back < 0)
	{		
		yaw += (mathUtils::Deg2Rad(90.0f) * elapsedTime);
	}
	else
	{
		yaw += (-mathUtils::Deg2Rad(90.0f) * elapsedTime);
	}

	obj->SetYaw(yaw);

	Vect3f pointA2 = m_AnimalToWPMap[name].m_Path[index];
	Vect3f pointB2 = obj->GetPosition();
	Vect3f l_Position = Vect3f(0.0f, 0.0f, 0.0f);
	Vect3f l_Dir = (pointA2 - pointB2).Normalize();

	l_Position += l_Dir * 3.0f * elapsedTime;

	obj->SetPosition(obj->GetPosition() + l_Position);
}