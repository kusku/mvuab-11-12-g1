#pragma once

#ifndef _CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H
#define _CAPTURE_FRAME_BUFFER_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CCaptureFrameBufferSceneRendererCommand : public CStagedTexturedRendererCommand
{
public:
	CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
};

#endif