#pragma once

#ifndef _CLEAR_SCENE_RENDERER_COMMAND_H
#define _CLEAR_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CClearSceneRendererCommand : public CSceneRendererCommand
{
public:
	CClearSceneRendererCommand(CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );

protected:
	bool		m_Color;
	bool		m_Depth;
	bool		m_Stencil;
};

#endif