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
#include <string>

#include "Utils\Types.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"
#include "GraphicsDefs.h"
#include "Cameras\Frustum.h"
#include "Math\Vector2.h"
#include "Graphic States\GraphicStates.h"
#include "BoundingObjects.h"

class CCamera;
class CTexture;
class CEffectTechnique;

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
	void	ClearTarget(CColor color);
	void	ClearTargetOnly(CColor color);

	void	SetTransform	( const D3DXMATRIX &mat );
	void	SetTransform	( const Mat44f &mat );

	//Draw primitives
	void    DrawPlane       (float size, const Vect3f& normal, float distance, CColor Color, int GridX, int GridZ ); 
	void	DrawLine		( const Vect3f &PosA, const Vect3f &PosB, CColor Color = colWHITE );
	void	DrawAxis		( float size );
	void	DrawGrid		( float sizeW, float sizeH, uint16 lines, CColor color = colWHITE  );
	void	DrawCube		( const Vect3f &dimensions, CColor color = colWHITE );
	void	DrawSphere		( float radius, uint32 edges, CColor color = colWHITE );
	void	DrawCone		( float size, uint16 edges, CColor color = colWHITE );
	void	DrawTower		( float size, CColor color = colWHITE );
	void	DrawQuad2D		( const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color = colBLUE);
	void	DrawQuad2D		( const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, ETypeFlip flip = NONE_FLIP);
	void	DrawTexturedQuad2D( const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture *Texture, CColor color = colWHITE );
	void	DrawTexturedQuad2D(const Vect2i& pos, const Vect2f& startTexCoords, const Vect2f& endTexCoords, uint32 w, uint32 h, ETypeAlignment alignment, CTexture *Texture, CColor color = colWHITE );
	void	DrawRectangle2D ( const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor, uint32 edge_w, uint32 edge_h, CColor& edgeColor );
	void	DrawColoredQuad2DTexturedInPixelsByEffectTechnique( CEffectTechnique* EffectTechnique,const CColor &color, Vect2f vec1 = Vect2f(-1.0f, -1.0f), Vect2f vec2 = Vect2f(1.0f, 1.0f), 
																float U0 = 0.0f, float V0 = 0.0f, float U1 = 1.0f, float V1 = 1.0f);
	void	DrawQuad2DTexturedInPixelsInFullScreen( CEffectTechnique* EffectTechnique );

	void	DrawAABB		(const TBoundingBox& bb, const Mat44f& world);

	// Export Video/Picture
	bool		DebugDumpBuffer		( const std::string &_FileName, const std::string &_FileNamePath );

	// Set & Get Methods
	void	SetPaintSolid		( bool paintSolid )		{ m_bPaintSolid = paintSolid; }
	void	SetColorDebug		( CColor color )		{ m_BackbufferColor_debug = color; }
	void	SetColorRelease		( CColor color )		{ m_BackbufferColor_release = color; }
	void	SetFullscreen		( bool fullscreen )		{ m_bFullscreen = fullscreen; }
	void	SetScreenSize		( Vect2i size )			{ m_SizeScreen = size; }
	void	SetWindowSize		( Vect2i size )			{ m_SizeWindow = size; }
	void	SetRenderTargetSize	( Vect2f size )			{ m_NowTargetSize = size; }

	bool				GetPaintSolid		() const	{ return m_bPaintSolid; }
	Vect2i				GetScreenSize		() const	{ return Vect2i(m_uWidth, m_uHeight); }
	Vect2i				GetScreenSizeOriginal	() const	{ return m_SizeScreen; }
	Vect2i				GettWindowSize	() const	{ return m_SizeWindow; }
	float				GetAspectRatio		() const	{ return m_AspectRatio; }
	CColor				GetColorDebug		() const	{ return m_BackbufferColor_debug; }
	CColor				GetColorRelease		() const	{ return m_BackbufferColor_release; }
	Vect2f				GetHalfPixel		() const	{ return m_HalfPixel; }
	Vect2f				GetRenderTargetSize	() const	{ return m_NowTargetSize; }
	CFrustum*		GetFrustum			() 	{ return &m_Frustum; }
	bool			GetFullScreen		() const { return m_bFullscreen; }
	Vect2i			GetWindowRectSize	() { return GetWindowRectRet(m_hWnd); }


	HWND						GetHWND				() const	{ return m_hWnd; }
	LPDIRECT3D9					GetD3D				() const	{ return m_pD3D; }
	inline LPDIRECT3DDEVICE9	GetDevice			() const	{ return m_pD3DDevice; }

	bool				 SetGraphicBlendState		(const TGraphicBlendStates& state);

protected:
	void Release ();
	void GetWindowRect( HWND hWnd );
	Vect2i GetWindowRectRet( HWND hwnd );
	void CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos);
	void CreateQuadBuffers();
	void DrawColoredQuad2DTexturedInPixels(Vect2f vec1, Vect2f vec2, const CColor& color, float U0, float V0, float U1, float V1);

	bool	m_bIsOk;

	CFrustum			m_Frustum;

	HWND				m_hWnd;         // 3D render window handle
	LPDIRECT3D9			m_pD3D;			// direct3d interface
	LPDIRECT3DDEVICE9	m_pD3DDevice;   // direct3d device

	uint32		m_uWidth;
	uint32		m_uHeight;
	Vect2i		m_SizeScreen;
	Vect2i		m_SizeWindow;
	float		m_AspectRatio;
	bool		m_bFullscreen;
	bool		m_bPaintSolid;
	Vect2f		m_HalfPixel;
	Vect2f		m_NowTargetSize;

	CColor m_BackbufferColor_debug;
	CColor m_BackbufferColor_release;

	LPDIRECT3DVERTEXBUFFER9		m_VBQuad;
	LPDIRECT3DINDEXBUFFER9		m_IBQuad;

	TGraphicBlendStates m_CurrentBlendState;
};

#endif

