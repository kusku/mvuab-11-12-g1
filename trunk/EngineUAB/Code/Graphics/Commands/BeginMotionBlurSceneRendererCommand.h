#pragma once

#ifndef _BEGIN_MOTION_BLUR_SCENE_RENDERER_COMMAND_H
#define _BEGIN_MOTION_BLUR_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
class CTexture;
//-------------------------//

class CBeginMotionBlurSceneRendererCommand : public CSceneRendererCommand
{
private:
	CTexture*	m_PrevMotionBlurVelocity;
	CTexture*	m_CurrentMotionBlurVelocity;

public:
	//--- Init and End protocols------------------------------------------
	CBeginMotionBlurSceneRendererCommand ( CXMLTreeNode &_Node );
	
	//----Main Methods ---------------------------------------------------
	virtual void	Execute		( CRenderManager &RM );
};

#endif