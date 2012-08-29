
#pragma once

#ifndef _GRAPHIC_STATES_H
#define _GRAPHIC_STATES_H

#include <d3dx9.h>
#include <d3d9.h>

#define ALL_COLOR_CHANNELS (D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN)

struct TGraphicBlendStates 
{
	static const TGraphicBlendStates DefaultState;
	static const TGraphicBlendStates NonPremultiplied;
	static const TGraphicBlendStates Additive;
	static const TGraphicBlendStates AlphaBlend;
	static const TGraphicBlendStates NoColorWriteChannels;

	//Variables
	DWORD						m_AlphaBlendEnable;
	D3DBLENDOP					m_BlendOp;
	D3DBLEND					m_SrcBlend;
	D3DBLEND					m_DestBlend;
	DWORD						m_ColorWriteChannels;

	TGraphicBlendStates(DWORD alphaBlendEnable = FALSE, D3DBLENDOP blendOp = D3DBLENDOP_ADD, D3DBLEND srcBlend = D3DBLEND_ONE, D3DBLEND destBlend = D3DBLEND_ZERO, DWORD colorWriteChannels = ALL_COLOR_CHANNELS)
		: m_AlphaBlendEnable(alphaBlendEnable)
		, m_BlendOp(blendOp)
		, m_SrcBlend(srcBlend)
		, m_DestBlend(destBlend)
		, m_ColorWriteChannels(colorWriteChannels)
	{

	}
};

#endif