#pragma once

#ifndef _RENDER_SCENE_SCENE_RENDERER_COMMAND_H
#define _RENDER_SCENE_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

//--- Foward Declarations ---//
class CRenderableObjectsLayersManager;
class CRenderManager;
class CXMLTreeNode;
//---------------------------//

class CRenderSceneSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderSceneSceneRendererCommand(CXMLTreeNode &Node);
	virtual ~CRenderSceneSceneRendererCommand();

	virtual void	Execute		( CRenderManager &RM );

private:
	std::string								m_Layer;
	CRenderableObjectsLayersManager*		m_pROLManager;
};

#endif