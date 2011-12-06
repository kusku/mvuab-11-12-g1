//----------------------------------------------------------------------------------
// CTextureManager class
// Author: Marc Cuscullola
//
// Description:
// Clase se encarga de gestionar todas las texturas.
//----------------------------------------------------------------------------------
#pragma once

#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "Texture.h"
#include "Utils\MapManager.h"

class CTextureManager : public CMapManager<CTexture>
{
public:
	CTextureManager();
	virtual ~CTextureManager();

	void Reload();

	CTexture* GetTexture ( const std::string &fileName );
};

#endif
