//----------------------------------------------------------------------------------
// CTexture class
// Author: Marc Cuscullola
//
// Description:
// Clase se encarga de una textura concreta.
//----------------------------------------------------------------------------------
#pragma once

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <d3dx9.h>
#include <string>

class CTexture
{
protected:
	LPDIRECT3DTEXTURE9		m_Texture;
	std::string				m_FileName;
	bool					m_LockFromFile;

	virtual bool	LoadFile();
	void			Unload();

public:
	CTexture();
	virtual ~CTexture();

	bool		Load		(const std::string &FileName);
	bool		Reload		();
	void		Activate	(size_t StageId);
	
	/**Gets**/
	const std::string &		GetFileName		() const		{ return m_FileName; }
	LPDIRECT3DTEXTURE9		GetDXTexture	() const		{ return m_Texture; }
	bool					GetLockFromFile	() const		{ return m_LockFromFile; }

	/**Sets**/
	void	SetDXTexture	(LPDIRECT3DTEXTURE9 texture)		{ if(!m_LockFromFile){m_Texture = texture;} }
};

#endif