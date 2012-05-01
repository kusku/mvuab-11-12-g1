#pragma once

#ifndef __CLASS_DISABLE_ALPHA_BLENDING_SCENE_RENDERER_COMMAND_H__
#define __CLASS_DISABLE_ALPHA_BLENDING_SCENE_RENDERER_COMMAND_H__

#include "Commands\SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CDisableAlphaBlendingSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
				CDisableAlphaBlendingSceneRendererCommand ( CXMLTreeNode &_atts );
	
	//----Main Methods ---------------------------------------------------
	void		Execute									  ( CRenderManager &_RM );
	
};

#endif __CLASS_DISABLE_ALPHA_BLENDING_SCENE_RENDERER_COMMAND_H__