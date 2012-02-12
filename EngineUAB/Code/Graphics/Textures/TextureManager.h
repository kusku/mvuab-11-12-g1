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
	CTextureManager( const std::string &NoTextureName, const std::string &NoTexturePath );
	virtual ~CTextureManager();

	void Reload();

	CTexture*	GetTexture		( const std::string &fileName );
	CTexture*	GetNoTexture	();
		
	void		SetNoTextureName	( const std::string &Name )		{ m_NoTextureName = Name; }	
	void		SetNoTexturePath	( const std::string &Path )		{ m_NoTexturePath = Path; }

	const std::string&		GetNoTextureName	() const			{ return m_NoTextureName; }
	const std::string&		GetNoTexturePath	() const			{ return m_NoTexturePath; }

private:
	std::string		m_NoTextureName;
	std::string		m_NoTexturePath;
};

#endif
