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
	bool		Load	( const std::string &_FileName, bool _IsGUI );
	void		Execute	( CRenderManager &_RM );

	//----Métodos ---------------------------------------
	bool		Reload  ( void );

	void		SetRenderScene		( bool render )		{ m_bIsExecutedSceneCommands = render; }
	bool		IsRenderScene		() const			{ return m_bIsExecutedSceneCommands; }

private:
	void		CleanUp				( void );
	bool		LoadXML				( bool _IsGUI );

	//----Properties ( get & Set )---------------------------------------
public:
	std::string				GetNextName			( CXMLTreeNode &_atts );
	CSceneRendererCommand	*GetCommand			( std::string _ComandName ) { return m_SceneRendererCommands.GetResource( _ComandName ); }

	//----Membres ---------------------------------------
private:
	CTemplatedVectorMapManager<CSceneRendererCommand>	m_SceneRendererCommands;	// llistat de comandes a executar que provenen del XML
	CTemplatedVectorMapManager<CSceneRendererCommand>	m_GUIRendererCommands;

	std::string		m_szSceneFileName;
	std::string		m_szGUIFileName;

	bool		m_bIsExecutedSceneCommands;
};

#endif __CLASS_SCENE_RENDERER_COMMAND_MANAGER_H__