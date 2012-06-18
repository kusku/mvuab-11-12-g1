
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

class CAnimalManager
{
	typedef std::map<std::string, std::string> AnimalToWPG;
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
		
		void					Update					(float elapsedTime, const std::string& name);

	public:
		CAnimalManager();
		virtual ~CAnimalManager();

		bool					Load					(const std::string& filename);
		bool					Reload					();
		void					CleanUp					();

		void					Update					(float elapsedTime);
};

#endif