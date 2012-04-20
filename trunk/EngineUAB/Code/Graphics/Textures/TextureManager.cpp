#include "TextureManager.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CTextureManager::CTextureManager()
	: m_NoTextureName("")
	, m_NoTexturePath("")
{
}

CTextureManager::CTextureManager(const std::string &NoTextureName, const std::string &NoTexturePath)
	: m_NoTextureName(NoTextureName)
	, m_NoTexturePath(NoTexturePath)
{
	CTexture *l_Tex = new CTexture();
	l_Tex->Load( NoTexturePath );
	AddResource( NoTextureName, l_Tex );

	LOGGER->AddNewLog(ELL_INFORMATION, "CTextureManager::CTextureManager->Textura de defecto cargada.");
}

CTextureManager::~CTextureManager()
{
	this->Destroy();
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
			LOGGER->AddNewLog(ELL_INFORMATION, "CTextureManager::GetTexture->Textura cargada: %s", fileName.c_str() );
			AddResource(fileName, l_Tex);
		}
		else
		{
			CHECKED_DELETE(l_Tex);
			return GetResource(m_NoTextureName);
		}
	}

	return l_Tex;
}

CTexture* CTextureManager::GetNoTexture()
{
	return GetResource(m_NoTextureName);
}