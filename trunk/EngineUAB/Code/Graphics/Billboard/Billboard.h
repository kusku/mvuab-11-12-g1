#pragma once

#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

#include "Math\Color.h"
#include "Math\Vector3.h"

//---Forward Declarations---
class CRenderManager;
class CCamera;
class CTexture;
//--------------------------


class CBillboard
{
public:
	//--- Init and End protocols------------------------------------------
				CBillboard		( void );
				~CBillboard		( void );	

	//----Funcions principals -------------------------------------------
	void		Render			( CRenderManager &_RM, const CColor &_Color = colWHITE );
	void		RenderByHardware( CRenderManager &_RM, const CColor &_Color = colWHITE );
	void		RenderBySoftware( CRenderManager &_RM, const CColor &_Color = colWHITE );
	void		Update			( CCamera * _pCamera, float _Angle );
	
	//----Funcions privades ------------------------------------------------------
protected:
	void		Destroy			( void );
	
	//----Properties ( get & Set )---------------------------------------
public:
	void		SetVisible		( const bool &_Value )			{ m_bIsVisible = _Value; }
	bool		GetVisible		( void ) const					{ return m_bIsVisible; }

	void		SetPosition		( const Vect3f &_Position )		{ m_vPosition = _Position; }
	Vect3f		GetPosition		( void ) const					{ return m_vPosition; }

	void		SetWidth		( float _Width )				{ m_fWidth = _Width; }
	float		GetWidth		( void ) const					{ return m_fWidth; }
	void		SetHeight		( float _Height )				{ m_fHeight = _Height; }
	float		GetHeight		( void ) const					{ return m_fHeight; }

	void		SetPointA		( const Vect3f &_Point )		{ m_PointA = _Point; }
	Vect3f		GetPointA		( void ) const					{ return m_PointA; }
	void		SetPointB		( const Vect3f &_Point )		{ m_PointB = _Point; }
	Vect3f		GetPointB		( void ) const					{ return m_PointB; }
	void		SetPointC		( const Vect3f &_Point )		{ m_PointC = _Point; }
	Vect3f		GetPointC		( void ) const					{ return m_PointC; }
	void		SetPointD		( const Vect3f &_Point )		{ m_PointD = _Point; }
	Vect3f		GetPointD		( void ) const					{ return m_PointD; }

	void		SetTexture		( CTexture *_pTexture )			{ m_Texture = _pTexture; }
	CTexture *	GetTexture		( void ) const					{ return m_Texture; }

private:
	
	bool		m_bIsVisible;
	Vect3f		m_vPosition;
	
	float		m_fWidth, m_fHeight;

	CTexture*	m_Texture;

	Vect3f		m_PointA, m_PointB, m_PointC, m_PointD;
};

#endif __BILLBOARD_H__