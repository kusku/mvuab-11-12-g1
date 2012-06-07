
#pragma once

#ifndef _WAY_POINT_MANAGER_H
#define _WAY_POINT_MANAGER_H

#include <list>
#include <map>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "Math\Vector3.h"

class CWayPoint;

class CWayPointManager
{
	typedef std::list<CWayPoint*> WayPointList;
	typedef WayPointList::iterator WayPointListIt;

	typedef std::map<std::string, WayPointList> WayPointGroupMap;
	typedef WayPointGroupMap::iterator WayPointGroupMapIt;

	private:
		WayPointGroupMap		m_WPGroups;
		std::string				m_Filename;

		CWayPoint*				GetWayPoint				(const std::string& groupName, const std::string& wpName);

		CWayPoint*				GetClosestWayPoint		(const std::string& groupName, const Vect3f& pos);

		std::list<Vect3f>		GetPath					(CWayPoint* startPos, CWayPoint* destPos);

	public:
		CWayPointManager();
		~CWayPointManager();

		bool					Load					(const std::string& filename);
		bool					Reload					();

		WayPointList&			GetWayPoints			(const std::string& groupName);

		CWayPoint*				GetRandomWayPoint		(const std::string& groupName);

		std::vector<Vect3f>		GetPath					(const std::string& groupName, const Vect3f& curPos, const Vect3f& destPos);

		void					DebugRender				();

		void					RegisterMethods			();

};

#endif