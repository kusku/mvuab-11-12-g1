//----------------------------------------------------------------------------------
// CDebugRender class
// Author: Marc Cuscullola
//
// Description:
// Clase encargada de pintar por pantalla la informacion de debug
//----------------------------------------------------------------------------------
#pragma once

#ifndef _DEBUGRENDER_H
#define _DEBUGRENDER_H

#include "Math\Color.h"
#include "Math\Vector2.h"

//---Forward Declarations---
class CRenderManager;
class CFontManager;
class CTimer;
//--------------------------

class CDebugRender
{
public:
	CDebugRender();
	~CDebugRender();

	void					Init						( CTimer *timer );
	void					Render						( CRenderManager *rm, CFontManager *fm, CTimer *timer, CColor color = colWHITE );

	void					SetVisible					( bool flag ) { m_bIsVisible = flag; }
	bool					GetVisible					() const { return m_bIsVisible; }

private:
	void AnalizeSizeInfo ( CTimer *timer );

	bool					m_bIsVisible;
	Vect2i					m_SizeRectangle;
};

#endif

