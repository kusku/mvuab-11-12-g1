#pragma once

#ifndef _CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H
#define _CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"


//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//

class CCaptureFrameBufferSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
			CCaptureFrameBufferSceneRendererCommand ( CXMLTreeNode &_Node );
	
	
	//----Main Methods ---------------------------------------------------
	void	Execute	 ( CRenderManager &_RM );
};

#endif