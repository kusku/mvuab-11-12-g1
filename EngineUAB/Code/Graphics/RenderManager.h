//----------------------------------------------------------------------------------
// CRenderManaer class
// Author: Marc Cuscullola
//
// Description:
// Inicializa DirectX y permite el dibujado de primitivas.
//----------------------------------------------------------------------------------
#pragma once

#ifndef _RENDERMANAGER_H
#define _RENDERMANAGER_H

#include <d3dx9.h>
#include "Utils\Types.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"
#include "GraphicsDefs.h"
#include "Cameras\Frustum.h"

class CCamera;

class CRenderManager
{
public:
	// Init and End protocols
	CRenderManager();
	virtual ~CRenderManager();

	bool	Init	( HWND hWnd );
	void	Done	();
	bool	IsOk	() const { return m_bIsOk; }

	void	BeginRendering();
	void	EndRendering();
	void	SetupMatrices(CCamera* camera);

	void	SetTransform	( const D3DXMATRIX &mat );
	void	SetTransform	( const Mat44f &mat );

	//Draw primitives
	void	DrawLine		( const Vect3f &PosA, const Vect3f &PosB, CColor Color = colWHITE );
	void	DrawAxis		( float size );
	void	DrawGrid		( float sizeW, float sizeH, uint16 lines, CColor color = colWHITE  );
	void	DrawCube		( const Vect3f &dimensions, CColor color = colWHITE );
	void	DrawSphere		( float radius, uint32 edges, CColor color = colWHITE );
	void	DrawCone		( float size, uint16 edges, CColor color = colWHITE );
	void	DrawTower		( float size, CColor color = colWHITE );
	void	DrawRectangle2D ( const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor, uint32 edge_w, uint32 edge_h, CColor& edgeColor );

	// Set & Get Methods
	void	SetPaintSolid		( bool paintSolid )		{ m_bPaintSolid = paintSolid; }
	void	SetColorDebug		( CColor color )		{ m_BackbufferColor_debug = color; }
	void	SetColorRelease		( CColor color )		{ m_BackbufferColor_release = color; }
	void	SetFullscreen		( bool fullscreen )		{ m_bFullscreen = fullscreen; }
	void	SetScreenSize		( Vect2i size )			{ m_SizeScreen = size; }

	bool				GetPaintSolid		() const	{ return m_bPaintSolid; }
	Vect2i				GetScreenSize		() const	{ return Vect2i(m_uWidth, m_uHeight); }
	float				GetAspectRatio		() const	{ return m_AspectRatio; }

	HWND				GetHWND				() const	{ return m_hWnd; }
	LPDIRECT3D9			GetD3D				() const	{ return m_pD3D; }
	LPDIRECT3DDEVICE9	GetDevice			() const	{ return m_pD3DDevice; }

protected:
	void Release ();
	void GetWindowRect( HWND hWnd );
	void CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos);
	void DrawQuad2D ( const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color = colBLUE);

	bool	m_bIsOk;

	CFrustum			m_Frustum;

	HWND				m_hWnd;         // 3D render window handle
	LPDIRECT3D9			m_pD3D;			// direct3d interface
	LPDIRECT3DDEVICE9	m_pD3DDevice;   // direct3d device

	uint32		m_uWidth;
	uint32		m_uHeight;
	Vect2i		m_SizeScreen;
	float		m_AspectRatio;
	bool		m_bFullscreen;
	bool		m_bPaintSolid;

	CColor m_BackbufferColor_debug;
	CColor m_BackbufferColor_release;
};

#endif

