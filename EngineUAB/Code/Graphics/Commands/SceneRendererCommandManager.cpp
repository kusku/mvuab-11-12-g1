#include "SceneRendererCommandManager.h"
#include "CommandsIncludes.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"
#include <sstream>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSceneRendererCommandManager::CSceneRendererCommandManager()
	: m_FileName("")
{
}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
	CleanUp();
}

void CSceneRendererCommandManager::CleanUp()
{
	m_SceneRendererCommands.Destroy();
}

void CSceneRendererCommandManager::Load(const std::string &FileName)
{
	m_FileName = FileName;
	LoadXML();
}

void CSceneRendererCommandManager::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CSceneRendererCommandManager::Reload -> Reload de los Scene Renderer Commands.");
	CleanUp();
	LoadXML();
}

void CSceneRendererCommandManager::Execute(CRenderManager &RM)
{
	std::vector<CSceneRendererCommand*> l_VectorResources = m_SceneRendererCommands.GetResourcesVector();

	uint16 l_Count = l_VectorResources.size();
	for(uint16 i=0; i < l_Count; ++i)
	{
		l_VectorResources[i]->Execute(RM);
	}
}

std::string CSceneRendererCommandManager::GetNextName()
{
	return "";
}

void CSceneRendererCommandManager::LoadXML()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CSceneRendererCommandManager::LoadXML->Error al intentar leer el archivo de scene renderer commands: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	CXMLTreeNode l_SRC = newFile["scene_renderer_commands"];
	if( l_SRC.Exists() )
	{
		std::string l_Type;
		std::stringstream out;
		std::string l_NumCommand;
		std::string l_CommandName;
		CSceneRendererCommand *l_Command = NULL;

		uint16 l_Count = l_SRC.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			l_Command = NULL;

			//Converts the iteration number to string
			out << i;
			l_NumCommand = out.str();

			//Read de commands
			 l_Type = l_SRC(i).GetName();
			 if( l_Type == "begin_scene" )
			 {
				 l_Command = new CBeginSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "begin_scene_" + l_NumCommand;
			 }
			 else if( l_Type == "end_scene" )
			 {
				 l_Command = new CEndSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "end_scene_" + l_NumCommand;
			 }
			 else if( l_Type == "clear_scene" )
			 {
				 l_Command = new CClearSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "clear_scene_" + l_NumCommand;
			 }
			 else if( l_Type == "enable_z_write" )
			 {
				 l_Command = new CEnableZWriteSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "enable_z_write_" + l_NumCommand;
			 }
			 else if( l_Type == "disable_z_write" )
			 {
				 l_Command = new CDisableZWriteSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "disable_z_write_" + l_NumCommand;
			 }
			 else if( l_Type == "enable_z_test" )
			 {
				 l_Command = new CEnableZTestSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "enable_z_test_" + l_NumCommand;
			 }
			 else if( l_Type == "disable_z_test" )
			 {
				 l_Command = new CDisableZTestSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "disable_z_test_" + l_NumCommand;
			 }
			 else if( l_Type == "render_debug_lights" )
			 {
				 l_Command = new CRenderDebugLightsSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("render_debug_lights_" + l_NumCommand, l_Command);

			 }
			 else if( l_Type == "render_scene" )
			 {
				 l_Command = new CRenderSceneSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("render_scene_" + l_NumCommand, l_Command);
			 }
			 else if( l_Type == "setup_matrices" )
			 {
				 l_Command = new CSetupMatricesSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("setup_matrices_" + l_NumCommand, l_Command);
			 }
			 else if( l_Type == "set_pool_renderable_objects_technique" )
			 {
				 l_Command = new CSetPoolRenderableObjectsTechniqueCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("set_pool_renderable_objects_technique_" + l_NumCommand, l_Command);
			 }

			 //Add the command into the map
			 if( l_Command != NULL )
			 {
				 m_SceneRendererCommands.AddResource(l_CommandName, l_Command);
			 }
		}
	}
}