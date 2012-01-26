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
#include <string>

class CTextureManager : public CMapManager<CTexture>
{
public:
	CTextureManager();
	virtual ~CTextureManager();

	void Reload();

	CTexture*	GetTexture		( const std::string &fileName );
	CTexture*	GetNoTexture	();
		
	void		SetNoTextureName	( const std::string &Name )		{ m_NoTextureName = Name; }	

private:
	std::string		m_NoTextureName;
};

#endif
