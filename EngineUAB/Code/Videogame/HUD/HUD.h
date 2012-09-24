#pragma once

#ifndef _HUD_H_
#define _HUD_H_

#include "Math\Vector2.h"
#include <string>

class CRenderManager;
class CTexture;

class CHud
{
public:
	CHud();
	~CHud();

	bool	Load		( const std::string &_Filename );
	bool	Reload		();
	void	CleanUp		();

	void	Init		( int _iPlayerLife );
	void	Update		( float _fElapsedTime, int _iPlayerLife );
	void	Render		( CRenderManager &RM );

private:
	bool	LoadFile	();

private:
	std::string		m_Filename;

	//Player info
	int				m_iPlayerLife;

	//Texture properties
	Vect2i			m_MaskPosition;
	Vect2i			m_MaskSize;

	Vect2i			m_BarPosition;
	Vect2i			m_BarSize;
	Vect2i			m_BarRealSize;
	float			m_PerCentSize;

	Vect2i			m_BackgroundPosition;
	Vect2i			m_BackgroundSize;

	bool			m_bBackgroundActive;
	bool			m_bBarActive;
	bool			m_bMaskActive;

	CTexture*		m_pBar;
	CTexture*		m_pMask;
	CTexture*		m_pBackground;
};

#endif //_HUD_H_