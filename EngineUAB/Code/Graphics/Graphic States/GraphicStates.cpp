
#include "GraphicStates.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


const TGraphicBlendStates TGraphicBlendStates::DefaultState = TGraphicBlendStates();
const TGraphicBlendStates TGraphicBlendStates::NonPremultiplied = TGraphicBlendStates(true, D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, ALL_COLOR_CHANNELS);
const TGraphicBlendStates TGraphicBlendStates::Additive = TGraphicBlendStates(true, D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_ONE, ALL_COLOR_CHANNELS);
const TGraphicBlendStates TGraphicBlendStates::AlphaBlend = TGraphicBlendStates(true, D3DBLENDOP_ADD, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA, ALL_COLOR_CHANNELS);
const TGraphicBlendStates TGraphicBlendStates::NoColorWriteChannels = TGraphicBlendStates(false, D3DBLENDOP_ADD, D3DBLEND_ONE, D3DBLEND_ZERO, 0);

