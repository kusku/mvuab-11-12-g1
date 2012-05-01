#pragma once

#ifndef __CLASS_ENABLE_ALPHA_BLENDING_SCENE_RENDERER_COMMAND_H__
#define __CLASS_ENABLE_ALPHA_BLENDING_SCENE_RENDERER_COMMAND_H__

#include "Commands\SceneRendererCommand.h"


class CRenderManager;
class CXMLTreeNode;

class CEnableAlphaBlendingSceneRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
				CEnableAlphaBlendingSceneRendererCommand ( CXMLTreeNode &_atts );
	
	//----Funcions principals---------------------------------------
	void		Execute									 ( CRenderManager &_RM );
	
	//----Funcions ---------------------------------------
	
	//----Properties ( get & Set )---------------------------------------

	//----Membres ---------------------------------------
};

#endif __CLASS_ENABLE_ALPHA_BLENDING_SCENE_RENDERER_COMMAND_H__