
#include "WayPointManager.h"
#include "WayPoint.h"
#include "XML/XMLTreeNode.h"
#include "Base.h"
#include "Logger/Logger.h"
#include "AStar.h"

CWayPointManager::CWayPointManager()
	: m_Filename("")
{

}

CWayPointManager::~CWayPointManager()
{
	WayPointGroupMapIt itMap = m_WPGroups.begin();
	WayPointGroupMapIt itMapEnd = m_WPGroups.end();

	for (; itMap != itMapEnd; ++itMap)
	{
		WayPointListIt it = itMap->second.begin();
		WayPointListIt itEnd = itMap->second.end();

		for (; it != itEnd; ++it)
		{
			CWayPoint *wp = (*it);

			CHECKED_DELETE(wp);
		}

		itMap->second.clear();
	}

	m_WPGroups.clear();
}

bool CWayPointManager::Load( const std::string& filename )
{
	m_Filename = filename;

	return Reload();
}

bool CWayPointManager::Reload()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "WayPointManager::Load->Error al intentar leer el archivo xml: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}	

	CXMLTreeNode l_wps = newFile["waypoints"];
	if( l_wps.Exists() )
	{
		uint16 l_Count = l_wps.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_wps(i).GetName();

			if( l_Type == "group" )
			{
				CXMLTreeNode groupChild = l_wps(i); 

				std::string groupName = groupChild.GetPszProperty("name", "", true);

				if(m_WPGroups.find(groupName) == m_WPGroups.end())
				{
					m_WPGroups[groupName] = WayPointList();
				}

				uint16 l_CountGroup = groupChild.GetNumChildren();

				for(uint16 j = 0; j < l_CountGroup; ++j)
				{
					std::string l_TypeChild = groupChild(j).GetName();

					if(l_TypeChild == "waypoint")
					{
						std::string wpName = groupChild(j).GetPszProperty("name", "", true);
						Vect3f wpPos = groupChild(j).GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f), true);

						CWayPoint* wp = new CWayPoint(wpName, wpPos);

						m_WPGroups[groupName].push_back(wp);
					}

					if(l_TypeChild == "Brothers")
					{
						CXMLTreeNode broChild = groupChild(j);
						uint16 broChildCount = broChild.GetNumChildren();
						std::string wpName = broChild.GetPszProperty("wave_point_name", "", true);
						CWayPoint* wp = GetWayPoint(groupName, wpName);
						assert(wp);

						for (uint16 x = 0; x < broChildCount; ++x)
						{
							std::string l_TypeBroChild = broChild(x).GetName();

							if(l_TypeBroChild == "Brother")
							{
								std::string broName = broChild(x).GetPszProperty("name", "", true);
								CWayPoint* wpBro = GetWayPoint(groupName, broName);
								assert(wpBro);

								wp->AddBrother(wpBro);
							}
						}
					}

				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;

}

CWayPoint* CWayPointManager::GetWayPoint( const std::string& groupName, const std::string& wpName )
{
	WayPointList& wpList = GetWayPoints(groupName);

	WayPointListIt it = wpList.begin();
	WayPointListIt itEnd = wpList.end();

	for (; it != itEnd; ++it)
	{
		std::string wpNameIt = (*it)->GetName();
		
		if(wpName == wpNameIt)
		{
			return (*it);
		}
	}

	return NULL;
}

CWayPointManager::WayPointList& CWayPointManager::GetWayPoints( const std::string& groupName )
{
	return m_WPGroups[groupName];
}

std::list<Vect3f> CWayPointManager::GetPath(CWayPoint* startPos, CWayPoint* destPos)
{
	std::list<Vect3f> vecPath;

	GraphNodeList openList;
	GraphNodeList closeList;

	GraphNode* gn = new GraphNode();
	gn->SetWP(startPos);
	openList.PushBack(gn);

	GraphNode* endNode = new GraphNode();
	endNode->SetWP(destPos);
	
	while (!openList.IsEmpty())
	{
		GraphNode* current = openList.PopFront();
		
		if(endNode->CompareNode(current))
		{
			endNode->SetParentNode(current->GetParentNode());
			CHECKED_DELETE(current);
			break;
		}

		//Add To close List
		closeList.PushBack(current);

		//Iterate around brothers
		WayPointList& brothers = current->GetWP()->GetBrothers();
		WayPointListIt it = brothers.begin();
		WayPointListIt itEnd = brothers.end();

		for (; it != itEnd; ++it)
		{
			CWayPoint* wp = *it;

			//Set the cost of node_successor to be the cost of node_current plus
			//the cost to get to node_successor from node_current
			//Set h to be the estimated distance to node_goal (Using heuristic function)
			GraphNode* successor = new GraphNode();
			successor->SetWP(wp);
			successor->SetG(current->GetG() + (current->GetWP()->GetPosition().Distance(wp->GetPosition())));
			successor->SetH(wp->GetPosition().Distance(destPos->GetPosition()));

			//find node_successor on the OPEN list
			int32 ofound = openList.IndexOf(successor);

			//if node_successor is on the OPEN list but the existing one is as good
			//or better then discard this successor and continue
			if (ofound>=0)
			{
				GraphNode* temp = openList.At(ofound);
				
				if(temp->GetF() < current->GetF())
				{
					CHECKED_DELETE(successor);
					continue;
				}
			}

			//find node_successor on the CLOSED list
			int32 cFound = closeList.IndexOf(successor);

			//if node_successor is on the CLOSED list but the existing one is as good
			//or better then discard this successor and continue;
			if (cFound>=0)
			{
				GraphNode* temp = closeList.At(cFound);
				
				if(temp->GetF() < current->GetF())
				{
					CHECKED_DELETE(successor);
					continue;
				}
			}

			//Remove occurences of node_successor from OPEN and CLOSED
			if (ofound != -1)
			{
				openList.RemoveAt(ofound);
			}

			if (cFound!=-1)			
			{
				closeList.RemoveAt(cFound);
			}

			//Set the parent of node_successor to node_current;
			successor->SetParentNode(current);

			//Add node_successor to the OPEN list
			openList.PushBack(successor);
		}

		openList.Sort();
	}
	
	if(endNode->GetParentNode() == NULL)
	{
		return vecPath;
	}

	GraphNode* backtrace = endNode;

	while(backtrace != NULL)
	{
		vecPath.push_front(backtrace->GetWP()->GetPosition());

		backtrace = backtrace->GetParentNode();
	}

	CHECKED_DELETE(endNode);

	return vecPath;
}

std::vector<Vect3f> CWayPointManager::GetPath(const std::string& groupName, const Vect3f& curPos, const Vect3f& destPos)
{
	std::vector<Vect3f> vecPath;

	if(m_WPGroups.find(groupName) == m_WPGroups.end())
	{
		return vecPath;
	}

	CWayPoint* startPos = GetClosestWayPoint(groupName, curPos);
	assert(startPos);

	CWayPoint* endPos = GetClosestWayPoint(groupName, destPos);
	assert(endPos);

	std::list<Vect3f> posList = GetPath(startPos, endPos);

	std::list<Vect3f>::iterator it = posList.begin();
	std::list<Vect3f>::iterator itEnd = posList.end();
	for (; it != itEnd; ++it)
	{
		vecPath.push_back(*it);
	}

	return vecPath;
}

CWayPoint* CWayPointManager::GetClosestWayPoint( const std::string& groupName, const Vect3f& pos )
{
	if(m_WPGroups.find(groupName) == m_WPGroups.end())
	{
		return NULL;
	}

	CWayPoint* wp = NULL;
	float distance = -1;

	WayPointListIt it = m_WPGroups[groupName].begin();
	WayPointListIt itEnd = m_WPGroups[groupName].end();

	for (; it != itEnd; ++it)
	{
		CWayPoint *wpIt = (*it);
		float temp = wpIt->GetPosition().SqDistance(pos);

		if(temp < distance || distance == -1)
		{
			distance = temp;
			wp = wpIt;
		}
	}

	return wp;
}
