#include "ItemManager.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CItemManager::CItemManager()
	: m_Filename("")
{
}

CItemManager::~CItemManager()
{
	CleanUp();
}

void CItemManager::CleanUp()
{
	m_ItemsVector.clear();
}

bool CItemManager::Load(const std::string &_Filename)
{
	m_Filename = _Filename;
	return LoadXML();
}

bool CItemManager::Reload()
{
	CleanUp();
	return LoadXML();
}

bool CItemManager::LoadXML()
{
	return true;
}