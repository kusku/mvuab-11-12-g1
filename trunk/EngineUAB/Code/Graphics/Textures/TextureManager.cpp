#include "TextureManager.h"


CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
}

void CTextureManager::Reload()
{
	TMapResource::iterator l_It;
	for(l_It = m_Resources.begin(); l_It != m_Resources.end(); ++l_It)
	{
		l_It->second->Reload();
	}
}