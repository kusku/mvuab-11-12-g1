#pragma once

#ifndef _DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H
#define _DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;
class CRenderableObjectTechnique;

class CDeferredShadingSceneRendererCommand : public CStagedTexturedRendererCommand
{
private:
	CRenderableObjectTechnique			*m_pRenderableObjectTechnique;

	void	SetLightsData	( CRenderManager &RM );
public:
	CDeferredShadingSceneRendererCommand(CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
};

#endif