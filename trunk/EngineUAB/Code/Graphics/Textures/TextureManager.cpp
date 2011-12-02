#include "TextureManager.h"


CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
}

void CTextureManager::Reload()
{
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
		l_Tex->Load(fileName);

		AddResource(fileName, l_Tex);
	}

	return l_Tex;
}