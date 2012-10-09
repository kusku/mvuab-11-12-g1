#pragma once

#ifndef _HUD_H_
#define _HUD_H_

#include "Utils\TemplatedVectorMapManager.h"
#include "Math\Vector2.h"
#include <string>

class CRenderManager;
class CTexture;

//------------------------------------------------
struct STexture
{
	CTexture	*pTexture;
	bool		bActive;
	Vect2i		position;
	Vect2i		size;
};

//------------------------------------------------
class CHud : public CTemplatedVectorMapManager<STexture>
{
public:
	CHud();
	~CHud();

	bool	Load			( const std::string &_Filename );
	bool	Reload			();
	void	CleanUp			();

	void	Init			( int _iPlayerLife );
	void	InitWolf		( int _iWolfLife = 100 );
	void	Update			( float _fElapsedTime, int _iPlayerLife );
	void	Render			( CRenderManager &RM );

	bool	IsTextureActive	( const std::string &_name );
	void	ActiveTexture	( const std::string &_name, bool _bActive );

	inline void	SetActivePlayerBar	( bool _active )		{ m_bPlayerActive = _active; }
	inline void	SetWolfLife			( int _life )			{ m_iCurrentWolfLife = _life; }
	inline void	SetActiveWolfBar	( bool _active )		{ m_bWolfActive = _active; }
	inline bool IsActiveWolfBar		() const				{ return m_bWolfActive; }

private:
	bool	LoadFile		();
	void	CalculatePositions();

private:
	std::string		m_Filename;

	float			m_fThresholdDyingEffect;

	//--------------------------------
	//--Player Info-------------------
	//--------------------------------
	int				m_iPlayerLife;
	int				m_iPlayerPreviousLife;
	bool			m_bPlayerActive;

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

	//--------------------------------
	//--Wolf Info---------------------
	//--------------------------------
	bool			m_bWolfActive;
	int				m_iWolfLife;
	int				m_iCurrentWolfLife;
	int				m_iWolfPreviousLife;

	Vect2i			m_WolfMaskPosition;
	Vect2i			m_WolfMaskSize;

	Vect2i			m_WolfBarPosition;
	Vect2i			m_WolfBarSize;
	Vect2i			m_WolfBarRealSize;
	float			m_WolfPerCentSize;

	Vect2i			m_WolfBackgroundPosition;
	Vect2i			m_WolfBackgroundSize;

	bool			m_bWolfBackgroundActive;
	bool			m_bWolfBarActive;
	bool			m_bWolfMaskActive;

	CTexture*		m_pWolfBar;
	CTexture*		m_pWolfMask;
	CTexture*		m_pWolfBackground;

};
//------------------------------------------------

#endif //_HUD_H_