#pragma once

#ifndef _RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H
#define _RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "Math\Vector2.h"

class CRenderManager;
class CXMLTreeNode;

class CRenderDebugShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
protected:
	float			m_ScreenWidth, m_ScreenHeight;

	void AdvanceTexturePosition(Vect2f &Position);
public:
	CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &Node);
	void	Execute				( CRenderManager &RM );
};

#endif