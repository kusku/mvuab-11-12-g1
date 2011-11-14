//----------------------------------------------------------------------------------
// CMapManager class
// Author: Enric Vergara
//
// Description:
// Template class for creating Map Managers
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_MAP_MANAGER_H_
#define INC_MAP_MANAGER_H_

#include "Base.h"
#include "Logger/Logger.h"
#include <string>
#include <map>

template<class T>
class CMapManager
{
  public:
		CMapManager(const std::string & MapManagerName): m_sMapManagerName(MapManagerName) {}
		

    virtual T * GetResource(const std::string & Name)
    {
      std::map<std::string, T*>::iterator it = m_Resources.find(Name);
      if (it == m_Resources.end())
      {
        return NULL;
      }
      else
      {
        return it->second;
      }

    }

    virtual void AddResource(const std::string & Name, T * Resource)
    {
      if (m_Resources.find(Name)==m_Resources.end())
			{
				LOGGER->AddNewLog(ELL_WARNING, "%s::AddResource -> El resource %s ya existe en el mapa",m_sMapManagerName.c_str(), Name.c_str());
			}
			else
			{
				m_Resources[Name]=Resource;
			}
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
    typedef std::map<std::string, T*> TMapResource;
    TMapResource											m_Resources;
		std::string												m_sMapManagerName;
};

#endif //INC_MAP_MANAGER_H_