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
	LOGGER->AddNewLog(ELL_INFORMATION, "CSceneRendererCommandManager::Load->Cargando los comandos de renderizado.");

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
		return;
	}

	CXMLTreeNode l_SRC = newFile["scene_renderer_commands"];
	if( l_SRC.Exists() )
	{
		std::string l_Type;
		std::string l_NumCommand;
		std::string l_CommandName;
		CSceneRendererCommand *l_Command = NULL;

		uint16 l_Count = l_SRC.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			l_Command = NULL;

			//Converts the iteration number to string
			std::stringstream out;
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

			 else if( l_Type == "render_scene" )
			 {
				 l_Command = new CRenderSceneSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "render_scene_" + l_NumCommand;
			 }
			 else if( l_Type == "setup_matrices" )
			 {
				 l_Command = new CSetupMatricesSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "setup_matrices_" + l_NumCommand;
			 }
			 else if( l_Type == "set_pool_renderable_objects_technique" )
			 {
				 l_Command = new CSetPoolRenderableObjectsTechniqueCommand( l_SRC(i) );
				 l_CommandName = "set_pool_renderable_objects_technique_" + l_NumCommand;
			 }
			 else if( l_Type == "set_render_target" )
			 {
				 l_Command = new CSetRenderTargetSceneRendererCommand( l_SRC(i) );
				 l_CommandName = l_Command->GetName();
			 }
			 else if( l_Type == "unset_render_target" )
			 {
				 std::string l_RenderTarget = l_SRC(i).GetPszProperty("render_target", "");

				 CSetRenderTargetSceneRendererCommand* l_SRT = static_cast<CSetRenderTargetSceneRendererCommand*>(m_SceneRendererCommands.GetResource(l_RenderTarget));

				 l_Command = new CUnsetRenderTargetSceneRendererCommand(l_SRT, l_SRC(i));
				 l_CommandName = "unset_render_target_" + l_NumCommand;
			 }
			 else if( l_Type == "render_draw_quad" )
			 {
				 l_Command = new CDrawQuadRendererCommand( l_SRC(i) );
				 l_CommandName = l_Command->GetName() + "_" + l_NumCommand;
			 }
			 else if( l_Type == "render_deferred_shading" )
			 {
				 l_Command = new CDeferredShadingSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "render_deferred_shading_" + l_NumCommand;
			 }
			 else if( l_Type == "present" )
			 {
				 l_Command = new CPresentSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "present_" + l_NumCommand;
			 }
			 else if( l_Type == "generate_shadow_maps" )
			 {
				 l_Command = new CGenerateShadowMapsSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "generate_shadow_maps_" + l_NumCommand;
			 }
			 else if( l_Type == "capture_frame_buffer" )
			 {
				 l_Command = new CCaptureFrameBufferSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "capture_frame_buffer_" + l_NumCommand;
			 }
			 else if( l_Type == "SkySphere" )
			 {
				 l_Command = new CSkySphereCommand( l_SRC(i) );
				 l_CommandName = "SkySphere_" + l_NumCommand;
			 }
#if defined(_DEBUG)
			 else if( l_Type == "render_debug_info" )
			 {
				 l_Command = new CRenderDebugInfoSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "render_debug_info_" + l_NumCommand;
			 }
			 else if( l_Type == "render_debug_lights" )
			 {
				 l_Command = new CRenderDebugLightsSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "render_debug_lights_" + l_NumCommand;
			 }
			 else if( l_Type == "render_modifiers" )
			 {
				 l_Command = new CRenderDebugModifiersSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "render_modifiers_" + l_NumCommand;
			 }
			 else if( l_Type == "render_debug_physics" )
			 {
				 l_Command = new CRenderDebugPhysicsSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "render_debug_physics" + l_NumCommand;
			 }
			 else if( l_Type == "render_debug_process" )
			 {
				 l_Command = new CRenderDebugProcessSceneRendererCommand( l_SRC(i) );
				 l_CommandName = "render_debug_process" + l_NumCommand;
			 }
#endif

			 //Add the command into the map
			 if( l_Command != NULL )
			 {
				 m_SceneRendererCommands.AddResource(l_CommandName, l_Command);
			 }
		}
	}
}