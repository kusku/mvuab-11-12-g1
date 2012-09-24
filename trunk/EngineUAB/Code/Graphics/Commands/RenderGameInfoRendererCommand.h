#pragma once

#ifndef _RENDER_GAME_INFO_RENDERER_COMMAND_H
#define _RENDER_GAME_INFO_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CRenderGameInfoRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
	CRenderGameInfoRendererCommand (CXMLTreeNode &Node);

	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &RM );
};

#endif //_RENDER_GAME_INFO_RENDERER_COMMAND_H