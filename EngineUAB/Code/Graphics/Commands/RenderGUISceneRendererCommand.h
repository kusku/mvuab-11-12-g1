#pragma once

#ifndef _RENDER_GUI_SCENE_RENDERER_COMMAND_H
#define _RENDER_GUI_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CRenderGUISceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
	CRenderGUISceneRendererCommand	( CXMLTreeNode &_Node );

	//----Main Methods ---------------------------------------------------
	virtual void	Execute			( CRenderManager &_RM );
};

#endif