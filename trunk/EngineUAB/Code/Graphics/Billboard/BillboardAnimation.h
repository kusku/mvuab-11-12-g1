#pragma once

#ifndef _BILLBOARD_ANIMATION_H
#define _BILLBOARD_ANIMATION_H

#include "Utils\Named.h"
#include "Billboard.h"
#include "Utils\Types.h"
#include <vector>

class CTexture;
class CCamera;
class CRenderManager;

class CBillboardAnimation : public CBillboard, public CNamed
{
public:
	CBillboardAnimation();
	CBillboardAnimation(float height, float width, const Vect3f &position, float rotation, bool loop, const CColor& color);
	~CBillboardAnimation();

	void		Update			( float elapsedTime, CCamera &camera );
	void		Render			( CRenderManager &RM );

	void		AddTexture		( CTexture* texture )			{ m_vTextures.push_back(texture); }
	void		SetTimeToUpdate	( float time )					{ m_fTimeToUpdate = time; }

private:
	std::vector<CTexture*>			m_vTextures;
	uint16							m_uCurrentFrame;
	float							m_fTimeToUpdate;
	float							m_fTimeBeforeUpdate;
	bool							m_bLoop;
};

#endif