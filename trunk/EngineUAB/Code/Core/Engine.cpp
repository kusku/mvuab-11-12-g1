#include "Engine.h"
#include "Base.h"
#include "XML\XMLTreeNode.h"

CEngine::CEngine()
	: m_pCore(NULL)
	, m_pProcess(NULL)	
	, m_pTimer(30)
{
}

CEngine::~CEngine()
{
	CHECKED_DELETE(m_pCore);
	CHECKED_DELETE(m_pProcess);
}

void CEngine::Init(HWND hWnd)
{
	m_pCore = new CCore();
	m_pCore->Init(hWnd, m_Config);
	m_pProcess->Init();
}

void CEngine::Update()
{
	m_pTimer.Update();
	float elapsedTime = m_pTimer.GetElapsedTime();

	m_pCore->Update(elapsedTime);
	m_pProcess->Update(elapsedTime);
}

void CEngine::Render()
{
	CRenderManager* l_RenderManager = m_pCore->GetRenderManager();

	l_RenderManager->BeginRendering();
	l_RenderManager->SetupMatrices();
	{
		RenderScene(l_RenderManager);		
	}
	l_RenderManager->EndRendering();
}

void CEngine::RenderScene(CRenderManager *renderManager)
{
	float l_FPS = m_pTimer.GetFPS();
	CORE->GetFontManager()->DrawDefaultText( 1, 1, colWHITE, "FPS: %f", l_FPS );

	m_pProcess->Render();
}

void CEngine::SetProcess(CProcess *process)
{
	m_pProcess = process;
}

void CEngine::Reload()
{
	m_pCore->GetFontManager()->ReloadTTFs();
	m_pCore->GetLanguageManager()->LoadXMLs();
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
		assert(0);
		return;
	}

	CXMLTreeNode l_EngineNode = newFile["Engine"];
	if (l_EngineNode.Exists())
	{
		//Parsea los hijos del nodo <Engine>
		int count = l_EngineNode.GetNumChildren();
		for(int i=0; i<count; ++i)
		{
			std::string l_Name = l_EngineNode(i).GetName();
			
			//Parsea el nodo <screen>
			if( l_Name == "screen" )
			{
				m_Config.fullscreen = l_EngineNode(i).GetBoolProperty( "fullscreen", false );

				std::string color = l_EngineNode(i).GetPszProperty( "color_debug", "" );
				m_Config.color_debug = string2Color( color );

				color = l_EngineNode(i).GetPszProperty( "color_release", "" );
				m_Config.color_release = string2Color( color );

				m_Config.position = l_EngineNode(i).GetVect2iProperty( "position", Vect2i(100, 100) );
				m_Config.resolution = l_EngineNode(i).GetVect2iProperty( "resolution", Vect2i(800, 600) );
			}

			//Parsea el nodo <languages>
			if( l_Name == "languages" )
			{
				m_Config.default_language = l_EngineNode(i).GetPszProperty( "default", "english" );

				int countLanguages = l_EngineNode(i).GetNumChildren();
				for(int j=0; j<countLanguages; ++j)
				{
					std::string l_Name = l_EngineNode(i)(j).GetName();

					if( l_Name == "fonts" )
					{
						m_Config.fonts_path = l_EngineNode(i)(j).GetPszProperty( "path", "" );
					}
					else //Parsea las direcciones de los lenguajes
					{
						std::string path = l_EngineNode(i)(j).GetPszProperty( "path", "" );
						if( path != "" )
						{
							m_Config.languages_path.push_back( path );
						}
					}
				}
			}
		}
	}
}

const CColor CEngine::string2Color(const std::string &color)
{
	if( color == "colBLACK" )
	{
		return colBLACK;
	}
	else if( color == "colWHITE" )
	{
		return colWHITE;
	}
	else if( color == "colRED" )
	{
		return colRED;
	}
	else if( color == "colGREEN" )
	{
		return colGREEN;
	}
	else if( color == "colBLUE" )
	{
		return colBLUE;
	}
	else if( color == "colMAGENTA" )
	{
		return colMAGENTA;
	}
	else if( color == "colYELLOW" )
	{
		return colYELLOW;
	}
	else if( color == "colCYAN" )
	{
		return colCYAN;
	}
	else
	{
		return colBLACK;
	}

}