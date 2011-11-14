#pragma once

#ifndef _RENDERMANAGER_H
#define _RENDERMANAGER_H

#include <d3dx9.h>
#include "Utils\Types.h"
#include "Math\Color.h"

class CRenderManager
{
protected:
	HWND				m_hWnd;         // 3D render window handle
	LPDIRECT3D9			m_pD3D;			// direct3d interface
	LPDIRECT3DDEVICE9	m_pD3DDevice;   // direct3d device

	uint32 m_uWidth;
	uint32 m_uHeight;
	bool m_bPaintSolid;

	CColor m_BackbufferColor_debug;
	CColor m_BackbufferColor_release;

	void GetWindowRect( HWND hWnd );

public:
	CRenderManager();
	virtual ~CRenderManager();

	bool Init( HWND hWnd );
	
	void BeginRendering();
	void EndRendering();
	void SetupMatrices(/*CCamera* camera*/);

	void SetPaintSolid ( bool paintSolid );
	bool GetPaintSolid () const;

	HWND				GetHWND () const;
	LPDIRECT3D9			GetD3D () const;
	LPDIRECT3DDEVICE9	GetDevice () const;
};

#endif

