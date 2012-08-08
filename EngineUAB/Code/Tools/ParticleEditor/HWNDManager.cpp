#include "stdafx.h"
#include "HWNDManager.h"
#include "Base.h"

CHWNDManager::CHWNDManager()
	: m_hWndOutput(NULL)
{
}

CHWNDManager::~CHWNDManager()
{
}

void CHWNDManager::CleanUp()
{
	CHECKED_DELETE(m_pHWNDManager );
}

CHWNDManager* CHWNDManager::m_pHWNDManager = NULL;

CHWNDManager* CHWNDManager::GetInstance()
{
	if( m_pHWNDManager == NULL )
	{
		m_pHWNDManager = new CHWNDManager();
	}

	return m_pHWNDManager;
}