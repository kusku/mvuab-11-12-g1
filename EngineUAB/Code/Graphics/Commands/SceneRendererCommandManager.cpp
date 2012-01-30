#include "SceneRendererCommandManager.h"
#include "CommandsIncludes.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

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
	std::vector<CSceneRendererCommand*>::iterator l_It = l_VectorResources.begin();
	std::vector<CSceneRendererCommand*>::iterator l_End = l_VectorResources.end();

	for(; l_It != l_End; ++l_It)
	{
		(*l_It)->Execute(RM);
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
		uint16 l_Count = l_SRC.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			 l_Type = l_SRC(i).GetName();
			 if( l_Type == "begin_scene" )
			 {
				 CSceneRendererCommand *l_Command = new CBeginSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("begin_scene", l_Command);
			 }
			 else if( l_Type == "end_scene" )
			 {
				 CSceneRendererCommand *l_Command = new CEndSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("end_scene", l_Command);
			 }
			 else if( l_Type == "clear_scene" )
			 {
				 CSceneRendererCommand *l_Command = new CClearSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("clear_scene", l_Command);
			 }
			 else if( l_Type == "enable_z_write" )
			 {
				 CSceneRendererCommand *l_Command = new CEnableZWriteSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("enable_z_write", l_Command);
			 }
			 else if( l_Type == "disable_z_write" )
			 {
				 CSceneRendererCommand *l_Command = new CDisableZWriteSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("disable_z_write", l_Command);
			 }
			 else if( l_Type == "enable_z_test" )
			 {
				 CSceneRendererCommand *l_Command = new CEnableZTestSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("enable_z_test", l_Command);
			 }
			 else if( l_Type == "disable_z_test" )
			 {
				 CSceneRendererCommand *l_Command = new CDisableZTestSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("disable_z_test", l_Command);
			 }
			 else if( l_Type == "render_debug_lights" )
			 {
				 CSceneRendererCommand *l_Command = new CRenderDebugLightsSceneRendererCommand( l_SRC(i) );
				 m_SceneRendererCommands.AddResource("render_debug_lights", l_Command);
			 }
		}
	}
}