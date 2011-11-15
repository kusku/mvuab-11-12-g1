#include "Engine.h"
#include "Base.h"
#include "XML\XMLTreeNode.h"

CEngine::CEngine()
	: m_pCore(NULL)
	, m_pProcess(NULL)
{
}

CEngine::~CEngine()
{
	CHECKED_DELETE(m_pCore);
}

void CEngine::Init(HWND hWnd, const SConfig &config)
{
	m_pCore = new CCore();
	m_pCore->Init(hWnd, config);
	m_pProcess->Init();
}

void CEngine::Update(float ElapsedTime)
{
	m_pCore->Update(ElapsedTime);
	m_pProcess->Update(ElapsedTime);
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
	m_pProcess->Render();
}

void CEngine::SetProcess(CProcess *process)
{
	m_pProcess = process;
}

void CEngine::LoadConfigXML(const std::string &configFile, SConfig &configStruct_)
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
				configStruct_.fullscreen = l_EngineNode(i).GetBoolProperty( "fullscreen", false );

				std::string color = l_EngineNode(i).GetPszProperty( "color_debug", "" );
				configStruct_.color_debug = string2Color( color );

				color = l_EngineNode(i).GetPszProperty( "color_release", "" );
				configStruct_.color_release = string2Color( color );

				configStruct_.position = l_EngineNode(i).GetVect2iProperty( "position", Vect2i(100, 100) );
				configStruct_.resolution = l_EngineNode(i).GetVect2iProperty( "resolution", Vect2i(800, 600) );
			}

			//Parsea el nodo <languages>
			if( l_Name == "languages" )
			{
				configStruct_.default_language = l_EngineNode(i).GetPszProperty( "default", "english" );

				int countLanguages = l_EngineNode(i).GetNumChildren();
				for(int j=0; j<countLanguages; ++j)
				{
					std::string l_Name = l_EngineNode(i)(j).GetName();

					if( l_Name == "fonts" )
					{
						configStruct_.fonts_path = l_EngineNode(i)(j).GetPszProperty( "path", "" );
					}
					else //Parsea las direcciones de los lenguajes
					{
						std::string path = l_EngineNode(i)(j).GetPszProperty( "path", "" );
						if( path != "" )
						{
							configStruct_.languages_path.push_back( path );
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
		return CColor(0.f, 0.f, 0.f);
	}

}