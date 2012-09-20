#include "Engine.h"
#include "Core.h"
#include "Base.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Fonts\FontManager.h"
#include "Location\LanguageManager.h"
#include "XML\XMLControl.h"
#include "XML\XMLTreeNode.h"
#include "ActionToInput.h"
#include "Math\Color.h"
#include "Utils/BaseUtils.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Effects\EffectManager.h"
#include "Commands\SceneRendererCommandManager.h"
#include "Stadistics\Stadistics.h"
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

CEngine::CEngine( void )
	: m_bIsOk		( false )
	, m_pCore		( NULL )
	, m_pProcess	( NULL )	
	, m_pLogger		( NULL )
	, m_Timer		( 30 )
{}

CEngine::~CEngine( void )
{
	Done();

	
}

// -----------------------------------------
//				MÈTODES 
// -----------------------------------------
void CEngine::Done ( void )
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

void CEngine::Release ( void )
{
	m_Config.languages_path.clear();

	//Finaliza el lector de XML
	XML::FinalizeParser();

	CHECKED_DELETE( m_pCore );
	//CHECKED_DELETE( m_pProcess );

	CHECKED_DELETE( m_pLogger );
}

bool CEngine::Init( HWND _HWnd )
{
	//Crea el Logger
	m_pLogger = new CLogger();

	LOGGER->AddNewLog(ELL_INFORMATION, "CEngine::Init-> Inicializando Engine");

	//Inicializa el lector de XML
	XML::InitParser();

	//Cra el Core
	m_pCore = new CCore();
	m_bIsOk = m_pCore->Init( _HWnd, m_Config );
	m_pCore->SetProcess( m_pProcess );
	
	//Inicializa el proceso
	if ( m_bIsOk )
		m_bIsOk = m_pProcess->Init();

	if ( m_bIsOk )
		//m_pCore->SetProcess ( m_pProcess );
		m_pCore->SetTimer( &m_Timer );

	if ( m_bIsOk )
		LOGGER->AddNewLog( ELL_INFORMATION, "CEngine::Init-> Engine inicializado correctamente.");
	else
		LOGGER->AddNewLog( ELL_ERROR, "CEngine::Init-> Engine inicializado incorrectamente.");

	return m_bIsOk;
}

void CEngine::Update( void )
{
	m_Timer.Update();
	float l_ElapsedTime = m_Timer.GetElapsedTime();

	m_pCore->Update( l_ElapsedTime );
	m_pProcess->Update( l_ElapsedTime );

	m_pCore->SetTimer( &m_Timer );

	if( CORE->IsDebugMode() )
	{
		UpdateDebugInputs();
	}
}

void CEngine::UpdateDebugInputs()
{
	if( CORE->GetActionToInput()->DoAction( ACTION_RELOAD_ALL ) )
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CEngine->Iniciando reload de todo el Core.");

		HWND hWnd = m_pCore->GetRenderManager()->GetHWND();
		CHECKED_DELETE(m_pCore);
		m_pCore = new CCore();
		m_pCore->Init(hWnd, m_Config);

		m_pCore->SetTimer(&m_Timer);

		LOGGER->AddNewLog(ELL_INFORMATION, "CEngine->Reload hecho de todo el Core.");
	}
}

void CEngine::Render()
{
	CRenderManager* l_RenderManager = m_pCore->GetRenderManager();
	RenderScene( l_RenderManager );		
}

void CEngine::RenderScene(CRenderManager *renderManager)
{
	m_pCore->GetSceneRendererCommandManager()->Execute( *renderManager );
}

void CEngine::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CEngine: Reload de todo el juego");
	m_pCore->Reload();
}

void CEngine::SetProcess(CEngineProcess *_Process)
{
	 m_pProcess = _Process; 
	 //m_pCore->SetProcess(_Process);
}

/*
*<summary>
* Lee un fichero xml de configuración del Engine
*</summary>
*<param name="configFile">Path del fichero de configuración</param>
*/
void CEngine::LoadConfigXML(const std::string &configFile)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(configFile.c_str()))
	{
		std::string msg_error = "CEngine::LoadConfigXML->Error al intentar leer el archivo de configuración: " + configFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	CXMLTreeNode l_ConfigNode = newFile["Config"];
	if (l_ConfigNode.Exists())
	{
		m_Config.bDebugMode = l_ConfigNode.GetBoolProperty("debug", false);

		//Parsea los hijos del nodo <Engine>
		int count = l_ConfigNode.GetNumChildren();
		for(int i=0; i<count; ++i)
		{
			std::string l_Name = l_ConfigNode(i).GetName();
			
			if( l_Name == "ScreenResolution" )
			{
				m_Config.resolution.x = l_ConfigNode(i).GetIntProperty( "width", 800 );
				m_Config.resolution.y = l_ConfigNode(i).GetIntProperty( "height", 600 );
			}
			else if( l_Name == "WindowsPosition" )
			{
				m_Config.position = l_ConfigNode(i).GetVect2iProperty( "position", Vect2i(100, 100) );
			}
			else if( l_Name == "RenderMode" )
			{
				m_Config.bFullscreen = l_ConfigNode(i).GetBoolProperty( "fullscreenMode", false );
			}
			else if( l_Name == "RenderColor" )
			{
				std::string color = l_ConfigNode(i).GetPszProperty( "colorDebug", "transparent" );
				m_Config.color_debug = string2Color( color );

				color = l_ConfigNode(i).GetPszProperty( "colorRelease", "transparent" );
				m_Config.color_release = string2Color( color );
			}
			else if( l_Name == "ActionToInput" )
			{
				m_Config.input_path = l_ConfigNode(i).GetPszProperty( "inputXML", "" ); 
			}
			else if( l_Name == "Mouse" )
			{
				m_Config.bExclusiveModeInMouse = l_ConfigNode(i).GetBoolProperty( "exclusiveModeInMouse", true );
				m_Config.bDrawPointerMouse = l_ConfigNode(i).GetBoolProperty( "drawPointerMouse", true );
			}
			else if( l_Name == "Fonts" )
			{
				m_Config.fonts_path = l_ConfigNode(i).GetPszProperty( "fontsXML", "" );
			}
			else if( l_Name == "Languages" )
			{
				m_Config.default_language = l_ConfigNode(i).GetPszProperty( "default", "catalan" );

				int countLanguages = l_ConfigNode(i).GetNumChildren();
				for( int j=0; j < countLanguages; ++j )
				{
					l_Name = l_ConfigNode(i)(j).GetName();
					if( l_Name == "Language" )
					{
						std::string languagePath = l_ConfigNode(i)(j).GetPszProperty( "languageXML", "" );
						if( languagePath != "" )
						{
							m_Config.languages_path.push_back( languagePath );
						}
					}
				}
			}
			else if( l_Name == "Meshes" )
			{
				m_Config.static_meshes_path = l_ConfigNode(i).GetPszProperty( "meshesXML", "" );
			}
			else if( l_Name == "RenderableObjects" )
			{
				m_Config.renderable_objects_path = l_ConfigNode(i).GetPszProperty( "objectsXML", "" );
			}
			else if( l_Name == "AnimatedModels" )
			{
				m_Config.animated_models_path = l_ConfigNode(i).GetPszProperty( "animatedXML", "" );
			}
			else if( l_Name == "Lights" )
			{
				m_Config.lights_path = l_ConfigNode(i).GetPszProperty( "lightXML", "" );
			}
			else if( l_Name == "Effects" )
			{
				m_Config.effects_path = l_ConfigNode(i).GetPszProperty( "effectsXML", "" );
			}
			else if( l_Name == "Textures" )
			{
				m_Config.no_texture_path = l_ConfigNode(i).GetPszProperty( "no_texture", "" );
			}
			else if( l_Name == "SceneRendererCommands" )
			{
				m_Config.scene_renderer_command_manager_path = l_ConfigNode(i).GetPszProperty("commandsXML", "");
				m_Config.scene_renderer_gui_command_manager_path = l_ConfigNode(i).GetPszProperty("guiComanndsXML" ,"");
			}
			else if( l_Name == "RenderableObjectTechniques" )
			{
				m_Config.renderable_object_techniques_path = l_ConfigNode(i).GetPszProperty("poolsXML", "");
			}
			else if( l_Name == "Scripts" )
			{
				m_Config.scripts_path = l_ConfigNode(i).GetPszProperty("scriptsXML", "");
			}
			else if( l_Name == "Billboards" )
			{
				m_Config.billboards_path = l_ConfigNode(i).GetPszProperty("billboardsXML", "");
			}
			else if( l_Name == "ParticlesEmitters" )
			{
				m_Config.particle_emitters_path = l_ConfigNode(i).GetPszProperty("particlesEmittersXML", "");
			}
			else if( l_Name == "ParticlesSettings" )
			{
				m_Config.particle_settings_path = l_ConfigNode(i).GetPszProperty("particlesSettingsXML", "");
			}
			else if( l_Name == "ParticlesSystems" )
			{
				m_Config.particle_systems_path = l_ConfigNode(i).GetPszProperty("particlesSystemsXML", "");
			}
			else if( l_Name == "ParticlesEmitterSystems" )
			{
				m_Config.particle_emitter_systems_path = l_ConfigNode(i).GetPszProperty("particlesEmitterSystemsXML", "");
			}
			else if( l_Name == "ParticlesStartUpInstances" )
			{
				m_Config.particle_startup_instances_path = l_ConfigNode(i).GetPszProperty("particlesStartupInstancesXML", "");
			}
			else if( l_Name == "GUI" )
			{
				m_Config.gui_path = l_ConfigNode(i).GetPszProperty("guiXML", "");
			}
			else if( l_Name == "Triggers" )
			{
				m_Config.triggers_system_path = l_ConfigNode(i).GetPszProperty( "triggersXML", "" );
			}
			else if( l_Name == "Sound" )
			{
				m_Config.sound_banks_path = l_ConfigNode(i).GetPszProperty( "soundbanks", "" );
				m_Config.speakers_path = l_ConfigNode(i).GetPszProperty( "speakers", "" );
			}
			else if( l_Name == "Modifiers" )
			{
				m_Config.modifiers_path = l_ConfigNode(i).GetPszProperty("modifiersXML", "");
			}
			else if( l_Name == "Debug_Options" )
			{
				m_Config.debug_options_path = l_ConfigNode(i).GetPszProperty("debug_options_xml", "");
			}
			else if( l_Name == "Waypoints" )
			{
				m_Config.waypoints_path = l_ConfigNode(i).GetPszProperty("wayPointsXML", "");
			}
			else if( l_Name == "AnimalMovement" )
			{
				m_Config.animal_movement_path = l_ConfigNode(i).GetPszProperty("animalMovementXML", "");
			}
			else if( l_Name == "SteeringBehaviorsSettings" )
			{
				m_Config.steering_behavior_settings_path= l_ConfigNode(i).GetPszProperty("SteeringBehaviorsSettingsXML", "");
			}
			else if( l_Name == "Rails" )
			{
				m_Config.rails_path = l_ConfigNode(i).GetPszProperty("railsXML", "");
			}
		}
	}

}

const CColor CEngine::string2Color(const std::string &color)
{
	if( color == "black" )
	{
		return colBLACK;
	}
	else if( color == "white" )
	{
		return colWHITE;
	}
	else if( color == "red" )
	{
		return colRED;
	}
	else if( color == "green" )
	{
		return colGREEN;
	}
	else if( color == "blue" )
	{
		return colBLUE;
	}
	else if( color == "magenta" )
	{
		return colMAGENTA;
	}
	else if( color == "yellow" )
	{
		return colYELLOW;
	}
	else if( color == "cyan" )
	{
		return colCYAN;
	}
	else if( color == "transparent" )
	{
		return colTRANSPARENT;
	}
	else
	{
		return colBLACK;
	}
}