#include "Core.h"
#include "Base.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Fonts\FontManager.h"
#include "Location\LanguageManager.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CCore::CCore()
	: m_pRenderManager(NULL)
	, m_pLanguageManager(NULL)
	, m_pFontManager(NULL)
	, m_pInputManager(NULL)
	, m_pActionToInput(NULL)
	, m_bIsOk(false)
{
}

CCore::~CCore()
{
	Release();
}

void CCore::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

bool CCore::Init( HWND hWnd, const SConfig &config )
{
	m_bIsOk = false;
	LOGGER->AddNewLog(ELL_INFORMATION, "CCore:: Inicializando Core");

	//Inicializa el Render
	m_pRenderManager = new CRenderManager();
	m_pRenderManager->SetColorDebug( config.color_debug );
	m_pRenderManager->SetColorRelease( config.color_release );
	m_pRenderManager->SetFullscreen( config.bFullscreen );
	m_pRenderManager->SetScreenSize( config.resolution );
	m_bIsOk = m_pRenderManager->Init(hWnd);

	if( m_bIsOk )
	{
		//Inicializa las fuentes
		m_pFontManager = new CFontManager();
		m_bIsOk = m_pFontManager->Init( m_pRenderManager );
		m_pFontManager->LoadTTFs( config.fonts_path );

		if( m_bIsOk )
		{
			//Inicializa los lenguajes
			m_pLanguageManager = new CLanguageManager();
			int count = config.languages_path.size();
			for(int i=0; i<count; ++i)
			{
				m_pLanguageManager->SetXmlFile( config.languages_path[i] );
			}
			m_pLanguageManager->LoadXMLs();
			m_pLanguageManager->SetCurrentLanguage( config.default_language );

			//Inicializa los inputs
			m_pActionToInput = new CActionToInput();
			m_bIsOk = m_pActionToInput->Init(hWnd, config.resolution, config.bExclusiveModeInMouse);
			m_pActionToInput->LoadXML( config.input_path );
			m_pInputManager = m_pActionToInput->GetInputManager();
		}
	}

	if ( !m_bIsOk )
	{
		Release();
		std::string msg_error = "Error al inicializar CCore";
		LOGGER->AddNewLog(ELL_ERROR, "CCore:: Error al inicializar Core");
		throw CException(__FILE__, __LINE__, msg_error);
	}

	return m_bIsOk;
}

void CCore::Release()
{
	CHECKED_DELETE(m_pRenderManager);
	CHECKED_DELETE(m_pLanguageManager);
	CHECKED_DELETE(m_pFontManager);
	//CHECKED_DELETE(m_pInputManager);
	CHECKED_DELETE(m_pActionToInput);
}

void CCore::Update(float ElapsedTime)
{
	//m_pInputManager->Update();
	m_pActionToInput->Update();
}

void CCore::Render()
{
}