#pragma once

#ifndef _RENDER_DEBUG_INFO_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_INFO_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CRenderDebugInfoSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugInfoSceneRendererCommand (CXMLTreeNode &Node);
	virtual void	Execute		( CRenderManager &RM );

private:
	bool		m_Grid;
	bool		m_Axis;

	bool		m_bRenderConsole;
	bool		m_bRenderLogger;
	bool		m_bRenderModifiers;
	bool		m_bRenderDebugOptions;
	bool		m_bRenderDebugRender;
};

#endif