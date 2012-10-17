#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Math\Vector3.h"
#include "LimitManager.h"
#include "ActionToInput.h"
#include "InputManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CLimitManager::CLimitManager()
	: m_Filename("")
{

}

CLimitManager::~CLimitManager()
{
	CleanUp();
}

void CLimitManager::CleanUp()
{
	LimitsMapIt it = m_LimitsMap.begin();
	LimitsMapIt itEnd = m_LimitsMap.end();

	for (; it != itEnd; ++it)
	{
		LimitVect& lv = it->second;

		int size = (int)lv.size();
		for (int i = 0; i < size; ++i)
		{
			CHECKED_DELETE(lv[i]);
		}

		lv.clear();
	}

	m_LimitsMap.clear();
}


bool CLimitManager::Load( const std::string& filename )
{
	m_Filename = filename;

	return Reload();
}

bool CLimitManager::Reload()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "CLimitManager::Load->Error al intentar leer el archivo xml: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CleanUp();

	CXMLTreeNode l_xml = newFile["limits_objs"];
	if( l_xml.Exists() )
	{
		uint16 l_Count = l_xml.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_xml(i).GetName();

			if( l_Type == "limit" )
			{
				std::string name = l_xml(i).GetPszProperty("name", "", true);				
				Vect3f pos = l_xml(i).GetVect3fProperty("pos", Vect3f(0, 0, 0), true);
				float yaw = l_xml(i).GetFloatProperty("yaw", 0.0f, true);
				float pitch = l_xml(i).GetFloatProperty("pitch", 0.0f, true);
				float roll = l_xml(i).GetFloatProperty("roll", 0.0f, true);
				std::string groupName = l_xml(i).GetPszProperty("group", "trigger", true);
				Vect3f dimensions = l_xml(i).GetVect3fProperty("dimension", Vect3f(0, 0, 0), true);

				CLimit* lim = new CLimit();

				lim->SetGroup(groupName);
				lim->SetName(name);
				lim->SetPosition(pos);
				lim->SetYaw(yaw);
				lim->SetPitch(pitch);
				lim->SetRoll(roll);
				lim->SetDimension(dimensions);

				AddToGroup(groupName, lim);

				lim->Init();
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CLimitManager::AddToGroup( const std::string& group, CLimit* lim )
{
	LimitsMapIt it = m_LimitsMap.find(group);

	if(it == m_LimitsMap.end())
	{
		m_LimitsMap[group] = LimitVect();
	}

	m_LimitsMap[group].push_back(lim);

	return true;
}

bool CLimitManager::ActivateGroup( const std::string& group, bool activate)
{
	LimitsMapIt it = m_LimitsMap.find(group);

	if(it == m_LimitsMap.end())
	{
		return false;
	}

	LimitVect& lv = m_LimitsMap[group];

	int size = (int)lv.size();
	for (int i = 0; i < size; ++i)
	{
		lv[i]->SetActive(activate);
	}

	return true;
}

void CLimitManager::DeActivateAll()
{
	LimitsMapIt it = m_LimitsMap.begin();
	LimitsMapIt itEnd = m_LimitsMap.end();

	for (; it != itEnd; ++it)
	{
		LimitVect& lv = it->second;

		int size = (int)lv.size();
		for (int i = 0; i < size; ++i)
		{
			lv[i]->SetActive(false);
		}
	}
}

void CLimitManager::ActivateAll()
{
	LimitsMapIt it = m_LimitsMap.begin();
	LimitsMapIt itEnd = m_LimitsMap.end();

	for (; it != itEnd; ++it)
	{
		LimitVect& lv = it->second;

		int size = (int)lv.size();
		for (int i = 0; i < size; ++i)
		{
			lv[i]->SetActive(true);
		}
	}
}

void CLimitManager::Update()
{
#ifdef _DEBUG

	if(CORE->GetActionToInput()->GetInputManager()->IsDownUp(IDV_KEYBOARD, KEY_9))
	{
		DeActivateAll();
	}

	if(CORE->GetActionToInput()->GetInputManager()->IsDownUp(IDV_KEYBOARD, KEY_0))
	{
		ActivateAll();
	}
#endif
}
