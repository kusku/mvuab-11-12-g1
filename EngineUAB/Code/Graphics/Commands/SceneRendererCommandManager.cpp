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


// -----------------------------------------
//			CONSTRUCTORS I DESTRUCTORS
// -----------------------------------------
CSceneRendererCommandManager::CSceneRendererCommandManager ( void ) 
	: m_FileName("")
{}

CSceneRendererCommandManager::~CSceneRendererCommandManager ( void )
{
	CleanUp();
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CSceneRendererCommandManager::CleanUp()
{
	m_SceneRendererCommands.Destroy();
}

bool CSceneRendererCommandManager::Load( const std::string &_FileName )
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CSceneRendererCommandManager::Load->Cargando los comandos de renderizado.");
	m_FileName = _FileName;
	return LoadXML ();
}

bool CSceneRendererCommandManager::Reload ( void )
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CSceneRendererCommandManager::Reload -> Reload de los Scene Renderer Commands.");
	CleanUp();
	return LoadXML ( );
}

void CSceneRendererCommandManager::Execute ( CRenderManager &_RM )
{
	CTemplatedVectorMapManager<CSceneRendererCommand>::TVectorResources l_VectorComandes = m_SceneRendererCommands.GetResourcesVector ();
	
	// Recorrem tots els comandos en ordre del vector i els executem
	for ( unsigned short i = 0; i < l_VectorComandes.size () ; ++i )
	{
		l_VectorComandes[i]->Execute ( _RM );
	}
}

// -----------------------------------------
//				MÈTODES PRIVATS
// -----------------------------------------
bool CSceneRendererCommandManager::LoadXML ( void )
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CSceneRendererCommandManager::LoadXML->Error al intentar leer el archivo de scene renderer commands: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_Commands = newFile["scene_renderer_commands"];
	if( l_Commands.Exists() )
	{
		std::string l_Type;
		std::string l_CommandName;
		CSceneRendererCommand *l_ActiveCommand = NULL;

		uint16 l_Count = l_Commands.GetNumChildren();
		for( uint16 i = 0; i < l_Count; ++i )
		{
			l_ActiveCommand = NULL;

			//Read de commands
			l_Type = l_Commands(i).GetName();
			if( l_Type == "begin_scene" )
			{
				l_ActiveCommand = new CBeginSceneRendererCommand ( l_Commands(i) );
			}
			else if( l_Type == "bloom_post_process" )
			{
				l_ActiveCommand = new CBloomPostProcessCommand( l_Commands(i) );
			}
			else if( l_Type == "capture_frame_buffer" )
			{
				l_ActiveCommand = new CCaptureFrameBufferSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "clear_scene" )
			{
				l_ActiveCommand = new CClearSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "disable_alpha_blending" )
			{
				l_ActiveCommand = new CDisableAlphaBlendingSceneRendererCommand ( l_Commands(i) );
			}
			else if( l_Type == "disable_z_test" )
			{
				l_ActiveCommand = new CDisableZTestSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "disable_z_write" )
			{
				l_ActiveCommand = new CDisableZWriteSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "end_scene" )
			{
				l_ActiveCommand = new CEndRenderSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "enable_alpha_blending" )
			{
				l_ActiveCommand = new CEnableAlphaBlendingSceneRendererCommand ( l_Commands(i) );
			}
			else if( l_Type == "end_scene" )
			{
				l_ActiveCommand = new CEndRenderSceneRendererCommand ( l_Commands(i) );
			}
			else if( l_Type == "enable_z_write" )
			{
				l_ActiveCommand = new CEnableZWriteSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "enable_z_test" )
			{
				l_ActiveCommand = new CEnableZTestSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "generate_shadow_maps" )
			{
				l_ActiveCommand = new CGenerateShadowMapsSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "present" )
			{
				l_ActiveCommand = new CPresentSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "render_deferred_shading" )
			{
				l_ActiveCommand = new CDeferredShadingSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "render_gui" )
			{
				l_ActiveCommand = new CRenderGUISceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "render_draw_quad" )
			{
				l_ActiveCommand = new CDrawQuadRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "render_particles" )
			{
				l_ActiveCommand = new CRenderParticlesSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "render_scene" )
			{
				l_ActiveCommand = new CRenderSceneSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "save_screen_to_file" )
			{
				l_ActiveCommand = new CSaveSceneToFileRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "setup_matrices" )
			{
				l_ActiveCommand = new CSetupMatricesSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "set_pool_renderable_objects_technique" )
			{
				l_ActiveCommand = new CSetPoolRenderableObjectsTechniqueCommand( l_Commands(i) );
			}
			else if( l_Type == "set_render_target" )
			{
				l_ActiveCommand = new CSetRenderTargetSceneRendererCommand( l_Commands(i) );
			}
			else if( l_Type == "SkySphere" )
			{
				l_ActiveCommand = new CSkySphereCommand( l_Commands(i) );
			}
			else if( l_Type == "unset_render_target" )
			{
				std::string l_RenderTarget = l_Commands(i).GetPszProperty("render_target", "");

				CSetRenderTargetSceneRendererCommand* l_SRT = static_cast<CSetRenderTargetSceneRendererCommand*>(m_SceneRendererCommands.GetResource(l_RenderTarget));

				l_ActiveCommand = new CUnsetRenderTargetSceneRendererCommand(l_SRT, l_Commands(i));
			}
			
#if defined(_DEBUG)
			 else if( l_Type == "render_debug_info" )
			 {
				 l_ActiveCommand = new CRenderDebugInfoSceneRendererCommand( l_Commands(i) );
			 }
			 else if( l_Type == "render_debug_lights" )
			 {
				 l_ActiveCommand = new CRenderDebugLightsSceneRendererCommand( l_Commands(i) );
			 }
			 else if( l_Type == "render_debug_physics" )
			 {
				 l_ActiveCommand = new CRenderDebugPhysicsSceneRendererCommand( l_Commands(i) );
			 }
			 else if( l_Type == "render_debug_process" )
			 {
				 l_ActiveCommand = new CRenderDebugProcessSceneRendererCommand( l_Commands(i) );
			 }
			 else if( l_Type == "render_debug_Sounds" )
			 {
				 l_ActiveCommand = new CRenderDebugSoundsSceneRendererCommand( l_Commands(i) );
			 }			


#endif

			 //Add the command into the map
			if ( l_ActiveCommand != NULL )
			{
				l_CommandName = l_ActiveCommand->GetName ();
				m_SceneRendererCommands.AddResource ( l_CommandName, l_ActiveCommand );
			}
		}
	}
	return true;
}

std::string	CSceneRendererCommandManager::GetNextName ( CXMLTreeNode &_atts )
{
	std::stringstream out;
	out << "_";
	out << m_SceneRendererCommands.GetResourcesVector().size();
	
	return ( _atts.GetName () +  out.str() );
}
