#pragma once

#ifndef _SCENE_RENDERER_COMMAND_MANAGER_H
#define _SCENE_RENDERER_COMMAND_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "SceneRendererCommand.h"
#include <string>

class CRenderManager;

class CSceneRendererCommandManager
{
public:
	CSceneRendererCommandManager();
	virtual ~CSceneRendererCommandManager();

	void		Load		( const std::string &FileName );
	void		Execute		( CRenderManager &RM );
	void		Reload		();

private:
	void			CleanUp			();
	std::string		GetNextName		();

	CTemplatedVectorMapManager<CSceneRendererCommand>	m_SceneRendererCommands;
	std::string											m_Filename;
};

#endif