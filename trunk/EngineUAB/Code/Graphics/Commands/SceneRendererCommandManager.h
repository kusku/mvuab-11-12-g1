#pragma once
#ifndef __CLASS_SCENE_RENDERER_COMMAND_MANAGER_H__
#define __CLASS_SCENE_RENDERER_COMMAND_MANAGER_H__

#include <string>

#include "Utils\TemplatedVectorMapManager.h"
#include "SceneRendererCommand.h"

class CRenderManager;

class CSceneRendererCommandManager
{
public:
	//--- Init and End protocols------------------------------------------
				CSceneRendererCommandManager	( void );
	virtual		~CSceneRendererCommandManager	( void );
	
	//----Funcions principals---------------------------------------
	bool		Load	( const std::string &_FileName );
	void		Execute	( CRenderManager &_RM );

	//----Funcions ---------------------------------------
	bool		Reload  ( void );

private:
	void		CleanUp							( void );
	bool		LoadXML							( void );

	//----Properties ( get & Set )---------------------------------------
public:
	std::string				GetNextName			( CXMLTreeNode &_atts );
	CSceneRendererCommand	*GetCommand			( std::string _ComandName ) { return m_SceneRendererCommands.GetResource( _ComandName ); }

	//----Membres ---------------------------------------
private:
	CTemplatedVectorMapManager<CSceneRendererCommand>	m_SceneRendererCommands;	// llistat de comandes a executar que provenen del XML
	
	std::string		m_FileName;
};

#endif __CLASS_SCENE_RENDERER_COMMAND_MANAGER_H__