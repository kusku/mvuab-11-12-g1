#pragma once

#ifndef _BILLBOARD_H
#define _BILLBOARD_H

#include "Math\Vector3.h"
#include "Math\Color.h"
#include <string>

class CRenderManager;
class CTexture;
class CCamera;

class CBillboard
{
public:
	CBillboard();
	CBillboard(float height, float width, const Vect3f &position, float rotation, const CColor& color, const std::string &textureName);
	~CBillboard();

	void		Update			( CCamera &Camera );
	void		Render			( CRenderManager &RM );

	void		SetTexture		( const std::string &name );
	void		SetPosition		( const Vect3f &pos )			{ m_vPosition = pos; }
	void		SetHeight		( float height )				{ m_fHeight = height; }
	void		SetWidth		( float width )					{ m_fWidth = width; }
	void		SetColor		( const CColor &color )			{ m_Color = color; }
	void		SetRotation		( float rotation )				{ m_fRotation = rotation; }

private:
	Vect3f			m_vPosition;
	float			m_fHeight;
	float			m_fWidth;
	float			m_fRotation;
	CColor			m_Color;
	CTexture		*m_pTexture;
	

	Vect3f			m_APoint;
	Vect3f			m_BPoint;
	Vect3f			m_CPoint;
	Vect3f			m_DPoint;
};

#endif