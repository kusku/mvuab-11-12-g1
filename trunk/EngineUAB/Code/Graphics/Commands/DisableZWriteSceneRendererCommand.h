#pragma once

#ifndef _DISABLE_Z_WRITE_SCENE_RENDERER_COMMAND_H
#define _DISABLE_Z_WRITE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CDisableZWriteSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
					CDisableZWriteSceneRendererCommand( CXMLTreeNode &_Node );
	
	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &_RM );
};

#endif