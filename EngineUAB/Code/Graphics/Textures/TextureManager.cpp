#include "TextureManager.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CTextureManager::CTextureManager()
	: m_NoTextureName("")
{
}


CTextureManager::~CTextureManager()
{
}

void CTextureManager::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CTextureManager: Reload de las texturas.");

	TMapResource::iterator l_It = m_Resources.begin();
	TMapResource::iterator l_End = m_Resources.end();
	for(; l_It != l_End; ++l_It)
	{
		l_It->second->Reload();
	}
}

CTexture* CTextureManager::GetTexture(const std::string &fileName)
{
	CTexture* l_Tex = GetResource(fileName);
	if( l_Tex == NULL )
	{
		l_Tex = new CTexture();
		if( l_Tex->Load(fileName) )
		{
			AddResource(fileName, l_Tex);
		}
		else
		{
			return NULL;
		}
	}

	return l_Tex;
}

CTexture* CTextureManager::GetNoTexture()
{
	return GetResource(m_NoTextureName);
}