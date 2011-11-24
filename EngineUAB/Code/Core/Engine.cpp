#include "Engine.h"
#include "Base.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Fonts\FontManager.h"
#include "Location\LanguageManager.h"
#include "XML\XMLTreeNode.h"
#include "ActionToInput.h"
#include "Logger\Logger.h"
#include "LogRender\LogRender.h"
#include "Exceptions\Exception.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEngine::CEngine()
	: m_pCore(NULL)
	, m_pProcess(NULL)	
	, m_pLogger(NULL)
	, m_pLogRender(NULL)
	, m_pTimer(30)
{
}

CEngine::~CEngine()
{
	m_Config.languages_path.clear();

	CHECKED_DELETE(m_pCore);
	CHECKED_DELETE(m_pProcess);

	CHECKED_DELETE(m_pLogRender);
	CHECKED_DELETE(m_pLogger);
}

void CEngine::Init(HWND hWnd)
{
	m_pLogger = new CLogger();

	LOGGER->AddNewLog(ELL_INFORMATION, "CEngine:: Inicializando Engine");

	m_pCore = new CCore();
	m_pCore->Init(hWnd, m_Config);
	m_pProcess->Init();

	m_pLogRender = new CLogRender();
}

void CEngine::Update()
{
	m_pTimer.Update();
	float elapsedTime = m_pTimer.GetElapsedTime();

	m_pCore->Update(elapsedTime);
	m_pProcess->Update(elapsedTime);
	m_pLogRender->Update(elapsedTime);
}

void CEngine::Render()
{
	CRenderManager* l_RenderManager = m_pCore->GetRenderManager();
	CCamera *l_Camera = m_pProcess->GetCamera();

	l_RenderManager->BeginRendering();
	l_RenderManager->SetupMatrices( l_Camera );
	{
		RenderScene( l_RenderManager );		
	}
	l_RenderManager->EndRendering();
}

void CEngine::RenderScene(CRenderManager *renderManager)
{
	m_pProcess->Render( renderManager );

	m_pLogRender->Render( renderManager, CORE->GetFontManager() );

	/*float l_FPS = m_pTimer.GetFPS();
	CORE->GetFontManager()->DrawDefaultText( 1, 1, colWHITE, "FPS: %f", l_FPS );*/
}

void CEngine::SetProcess(CProcess *process)
{
	m_pProcess = process;
}

void CEngine::Reload()
{
	m_pCore->GetFontManager()->ReloadTTFs();
	m_pCore->GetLanguageManager()->LoadXMLs();
	m_pCore->GetActionToInput()->Reload();
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

			if( l_Name == "WindowsPosition" )
			{
				m_Config.position = l_ConfigNode(i).GetVect2iProperty( "position", Vect2i(100, 100) );
			}

			if( l_Name == "RenderMode" )
			{
				m_Config.bFullscreen = l_ConfigNode(i).GetBoolProperty( "fullscreenMode", false );
			}

			if( l_Name == "RenderColor" )
			{
				std::string color = l_ConfigNode(i).GetPszProperty( "colorDebug", "blue" );
				m_Config.color_debug = string2Color( color );

				color = l_ConfigNode(i).GetPszProperty( "colorRelease", "black" );
				m_Config.color_release = string2Color( color );
			}

			if( l_Name == "ActionToInput" )
			{
				m_Config.input_path = l_ConfigNode(i).GetPszProperty( "inputXML", "" ); 
			}

			if( l_Name == "Mouse" )
			{
				m_Config.bExclusiveModeInMouse = l_ConfigNode(i).GetBoolProperty( "exclusiveModeInMouse", true );
				m_Config.bDrawPointerMouse = l_ConfigNode(i).GetBoolProperty( "drawPointerMouse", true );
			}

			if( l_Name == "Fonts" )
			{
				m_Config.fonts_path = l_ConfigNode(i).GetPszProperty( "fontsXML", "" );
			}

			if( l_Name == "Languages" )
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