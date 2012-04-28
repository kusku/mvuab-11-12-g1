#include "stdafx.h"
#include "ElementManager.h"
#include "Base.h"

CElementManager::CElementManager()
	: m_ElementToAdd(NONE)
	, m_WindowToAdd("")
{
}

CElementManager::~CElementManager()
{
}

CElementManager* CElementManager::m_pElementManager = NULL;

CElementManager* CElementManager::GetInstance()
{
	if( m_pElementManager == NULL )
	{
		m_pElementManager = new CElementManager();
	}

	return m_pElementManager;
}

void CElementManager::CleanUp()
{
	CHECKED_DELETE(m_pElementManager);
}