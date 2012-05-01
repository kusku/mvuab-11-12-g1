#pragma once

#ifndef _DISABLE_Z_TEST_SCENE_RENDERER_COMMAND_H
#define _DISABLE_Z_TEST_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CDisableZTestSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
					CDisableZTestSceneRendererCommand( CXMLTreeNode &_Node );
	
	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &_RM );
};

#endif