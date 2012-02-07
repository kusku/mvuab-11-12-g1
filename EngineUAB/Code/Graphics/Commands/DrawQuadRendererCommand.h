#pragma once

#ifndef _DRAW_QUAD_RENDERER_COMMAND_H
#define _DRAW_QUAD_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"
#include "Math\Color.h"

class CRenderManager;
class CXMLTreeNode;

class CDrawQuadRendererCommand : public CStagedTexturedRendererCommand
{
public:
	CDrawQuadRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );

private:
	CColor			m_Color;
};

#endif