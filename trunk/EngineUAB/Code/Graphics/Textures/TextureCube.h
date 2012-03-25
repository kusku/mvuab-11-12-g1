
#pragma once

#ifndef _TEXTURE_CUBE_H
#define _TEXTURE_CUBE_H

#include <string>
#include <d3dx9.h>

#include "Base.h"

class CTextureCube
{
	protected:
		LPDIRECT3DCUBETEXTURE9		m_DXCubeTexture;
		std::string					m_Filename;

	public:
		CTextureCube();
		virtual ~CTextureCube();

		bool						Load				(const std::string &FileName);
		bool						Reload				();

		LPDIRECT3DCUBETEXTURE9		GetDXCubeTexture	()									{ return m_DXCubeTexture; }
};

#endif