//----------------------------------------------------------------------------------
// CMapManager class
// Author: Marc Cuscullola
//
// Description:
// Template class for creating Map Managers
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_MAP_MANAGER_H_
#define INC_MAP_MANAGER_H_

#include "Base.h"
#include <string>
#include <map>

template<class T>
class CMapManager
{
public:
	virtual T * GetResource(const std::string & Name)
	{
		std::map<std::string, T*>::iterator it = m_Resources.find(Name);
		if (it != m_Resources.end())
		{
			return it->second;
		}

		return NULL;
	}

	virtual bool RemoveResource(const std::string& Name)
	{
		std::map<std::string, T*>::iterator it = m_Resources.find(Name);
		if (it != m_Resources.end())
		{
			CHECKED_DELETE(it->second);
			m_Resources.erase(it);
			return true;
		}

		return false;
	}

	virtual bool AddResource(const std::string & Name, T * Resource)
	{
		if(Name == "" )
		{
			return false;
		}

		if (m_Resources.find(Name) == m_Resources.end())
		{
			m_Resources[Name]=Resource;
			return true;
		}

		return false;
	}

	void Destroy()
	{
		std::map<std::string, T*>::iterator it = m_Resources.begin();
		std::map<std::string, T*>::iterator itEnd = m_Resources.end();

		for(; it != itEnd; ++it)
		{
			CHECKED_DELETE(it->second);
		}

		m_Resources.clear();
	}

protected:
	typedef std::map<std::string, T*>	TMapResource;
	TMapResource						m_Resources;
};

#endif //INC_MAP_MANAGER_H_