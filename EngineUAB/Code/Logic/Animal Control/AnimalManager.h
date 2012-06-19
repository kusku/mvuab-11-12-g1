
#pragma once

#ifndef _ANIMAL_MANAGER_H
#define _ANIMAL_MANAGER_H

#include <string>
#include <map>
#include <vector>

#include "Base.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

class CWayPoint;
class CObject3D;

class CAnimalManager
{

	struct AnimalLayerPathSettings
	{
		std::string m_AnimalLayer;
		std::string m_WayPointGroup;

		AnimalLayerPathSettings()
			: m_AnimalLayer("")
			, m_WayPointGroup("")
		{
		}

		AnimalLayerPathSettings(const std::string& animalLayer, const std::string& wayPointGroup)
			: m_AnimalLayer(animalLayer)
			, m_WayPointGroup(wayPointGroup)
		{
		}
	};

	typedef std::map<std::string, AnimalLayerPathSettings> AnimalToWPG;
	typedef AnimalToWPG::iterator AnimalToWPGIt;

	struct AnimalPath
	{
		std::string m_AnimalName;
		uint32 m_Current;
		std::vector<Vect3f> m_Path;

		AnimalPath()
			: m_AnimalName("")
			, m_Current(0)
		{
		}
	};

	typedef std::map<std::string, AnimalPath> AnimalToWP;
	typedef AnimalToWP::iterator AnimalToWPIt;

	private:
		AnimalToWPG				m_AnimalToWPGMap;
		AnimalToWP				m_AnimalToWPMap;

		std::string				m_Filename;
		
		void					Update					(float elapsedTime, const AnimalLayerPathSettings& settings, CObject3D* obj);

	public:
		CAnimalManager();
		virtual ~CAnimalManager();

		bool					Load					(const std::string& filename);
		bool					Reload					();
		void					CleanUp					();

		void					Update					(float elapsedTime);
};

#endif