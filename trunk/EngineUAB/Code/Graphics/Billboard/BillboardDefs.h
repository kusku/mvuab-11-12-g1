#pragma once

#ifndef _BILLBOARD_DEFS_H
#define _BILLBOARD_DEFS_H

#include "Textures\Texture.h"
#include "Math\Color.h"
#include <vector>

class CBillboardCore
{
public:
	CBillboardCore()
		: m_fSize(1.f)
		, m_fTimePerImage(1.f)
		, m_bLoop(false)
		, m_Color(colWHITE)
	{
	}

	CBillboardCore(float size, float time, bool loop, CColor color)
		: m_fSize(size)
		, m_fTimePerImage(time)
		, m_bLoop(loop)
		, m_Color(color)
	{
	}

	~CBillboardCore()
	{	
		m_TexturesVector.clear();
	}

	void AddTexture(CTexture *texture)
	{
		m_TexturesVector.push_back(texture);
	}

public:
	float		m_fSize;
	float		m_fTimePerImage;
	bool		m_bLoop;
	CColor		m_Color;

	std::vector<CTexture*>		m_TexturesVector;
};

#endif