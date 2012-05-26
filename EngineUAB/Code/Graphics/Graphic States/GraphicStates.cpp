
#include "GraphicStates.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


const TGraphicBlendStates TGraphicBlendStates::DefaultState = TGraphicBlendStates();
const TGraphicBlendStates TGraphicBlendStates::NonPremultiplied = TGraphicBlendStates(true, D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);
const TGraphicBlendStates TGraphicBlendStates::Additive = TGraphicBlendStates(true, D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_ONE);
const TGraphicBlendStates TGraphicBlendStates::AlphaBlend = TGraphicBlendStates(true, D3DBLENDOP_ADD, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA);

