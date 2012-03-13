#include "Engine.h"
#include "Core.h"
#include "Base.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Fonts\FontManager.h"
#include "Location\LanguageManager.h"
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

CEngine::CEngine()
	: m_pCore(NULL)
	, m_pProcess(NULL)	
	, m_pLogger(NULL)
	, m_Timer(30)
{
}

CEngine::~CEngine()
{
	m_Config.languages_path.clear();

	CHECKED_DELETE(m_pCore);
	CHECKED_DELETE(m_pProcess);

	CHECKED_DELETE(m_pLogger);
}

void CEngine::Init(HWND hWnd)
{
	m_pLogger = new CLogger();

	LOGGER->AddNewLog(ELL_INFORMATION, "CEngine::Init-> Inicializando Engine");

	m_pCore = new CCore();
	m_pCore->Init(hWnd, m_Config);
	m_pProcess->Init();
	m_pCore->SetProcess(m_pProcess);
}

void CEngine::Update()
{
	m_Timer.Update();
	float elapsedTime = m_Timer.GetElapsedTime();

	m_pCore->Update(elapsedTime);
	m_pProcess->Update(elapsedTime);

	m_pCore->SetTimer(&m_Timer);

#if defined(_DEBUG)
	UpdateDebugInputs();
#endif
}

void CEngine::UpdateDebugInputs()
{
	CActionToInput* action2Input = CORE->GetActionToInput();
	if( action2Input->DoAction( ACTION_LOGGER ) )
	{
		bool visible = CORE->GetDebugGUIManager()->GetLogRender()->GetVisible();
		CORE->GetDebugGUIManager()->GetLogRender()->SetVisible(!visible);
	}

	if( action2Input->DoAction( ACTION_LOGGER_PAGEDOWN ) )
	{
		CORE->GetDebugGUIManager()->GetLogRender()->PageDown();
	}

	if( action2Input->DoAction( ACTION_LOGGER_PAGEUP ) )
	{
		CORE->GetDebugGUIManager()->GetLogRender()->PageUp();
	}

	if( action2Input->DoAction( ACTION_LOGGER_PREVLINE ) )
	{
		CORE->GetDebugGUIManager()->GetLogRender()->PrevLine();
	}

	if( action2Input->DoAction( ACTION_LOGGER_NEXTLINE ) )
	{
		CORE->GetDebugGUIManager()->GetLogRender()->NextLine();
	}

	if( action2Input->DoAction(ACTION_DEBUG_INFO) )
	{
		bool visible = CORE->GetDebugGUIManager()->GetDebugRender()->GetVisible();
		CORE->GetDebugGUIManager()->GetDebugRender()->SetVisible(!visible);
	}

	if( action2Input->DoAction( ACTION_RELOAD_ALL ) )
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
				std::string color = l_ConfigNode(i).GetPszProperty( "colorDebug", "blue" );
				m_Config.color_debug = string2Color( color );

				color = l_ConfigNode(i).GetPszProperty( "colorRelease", "black" );
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
			}
			else if( l_Name == "RenderableObjectTechniques" )
			{
				m_Config.renderable_object_techniques_path = l_ConfigNode(i).GetPszProperty("poolsXML", "");
			}
			else if( l_Name == "Scripts" )
			{
				m_Config.scripts_path = l_ConfigNode(i).GetPszProperty("scriptsXML", "");
			}
#if defined (_DEBUG)
			else if( l_Name == "Modifiers" )
			{
				m_Config.modifiers_path = l_ConfigNode(i).GetPszProperty("modifiersXML", "");
			}
			else if( l_Name == "Debug_Options" )
			{
				m_Config.debug_options_path = l_ConfigNode(i).GetPszProperty("debug_options_xml", "");
			}
#endif
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
	else
	{
		return colBLACK;
	}
}