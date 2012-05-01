#pragma once

#ifndef _END_SCENE_RENDERER_COMMAND_H
#define _END_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//
class CEndRenderSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
					CEndRenderSceneRendererCommand ( CXMLTreeNode &_Node );
	
	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &_RM );
};

#endif