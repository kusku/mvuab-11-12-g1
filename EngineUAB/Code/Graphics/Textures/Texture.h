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

#include "Utils\Named.h"
#include "Utils\Types.h"
#include <d3dx9.h>
#include <string>

class CTexture : public CNamed
{
public:
	enum TPoolType {
		DEFAULT = 0,
		SYSTEMMEM
	};

	enum TUsageType {
		DYNAMIC = 0,
		RENDERTARGET
	};

	enum TTextureType {
		TGA = 0,
		JPG,
		BMP
	};

	enum TFormatType {
		A8R8G8B8 = 0,
		R8G8B8,
		X8R8G8B8,
		R32F
	};

public:
	CTexture();
	virtual ~CTexture();

	bool		Load		(const std::string &FileName);
	bool		Reload		();
	void		Activate	(size_t StageId);
	void		Deactivate	(size_t StageId);

	bool		Create		( const std::string &Name, uint32 Width, uint32 Height, uint32 MipMaps,
								TUsageType UsageType, TPoolType PoolType, TFormatType FormatType );

	bool		SetAsRenderTarget		( size_t IdStage = 0);
	void		UnsetAsRenderTarget		( size_t IdStage = 0);

	CTexture::TFormatType	GetFormatTypeFromString			( const std::string &FormatType );

	const std::string &		GetFileName		() const		{ return m_FileName; }
	LPDIRECT3DTEXTURE9		GetDXTexture	() const		{ return m_Texture; }

protected:
	LPDIRECT3DTEXTURE9		m_Texture;
	IDirect3DSurface9		*m_DepthStencilRenderTargetTexture;
	IDirect3DSurface9		*m_RenderTargetTexture;
	IDirect3DSurface9		*m_OldDepthStencilRenderTarget;
	IDirect3DSurface9		*m_OldRenderTarget;

	std::string				m_FileName;
	uint32					m_Width;
	uint32					m_Height;

	virtual bool	LoadFile();
	void			Unload();

};

#endif