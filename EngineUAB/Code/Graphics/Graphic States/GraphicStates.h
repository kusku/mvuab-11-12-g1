
#pragma once

#ifndef _GRAPHIC_STATES_H
#define _GRAPHIC_STATES_H

#include <d3dx9.h>
#include <d3d9.h>

struct TGraphicBlendStates 
{
	static const TGraphicBlendStates DefaultState;
	static const TGraphicBlendStates NonPremultiplied;
	static const TGraphicBlendStates Additive;
	static const TGraphicBlendStates AlphaBlend;

	//Variables
	DWORD						m_AlphaBlendEnable;
	D3DBLENDOP					m_BlendOp;
	D3DBLEND					m_SrcBlend;
	D3DBLEND					m_DestBlend;

	TGraphicBlendStates(DWORD alphaBlendEnable = FALSE, D3DBLENDOP blendOp = D3DBLENDOP_ADD, D3DBLEND srcBlend = D3DBLEND_ONE, D3DBLEND destBlend = D3DBLEND_ZERO)
		: m_AlphaBlendEnable(alphaBlendEnable)
		, m_BlendOp(blendOp)
		, m_SrcBlend(srcBlend)
		, m_DestBlend(destBlend)
	{

	}
};

#endif