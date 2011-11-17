#include "Core.h"
#include "Base.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "FontManager.h"
#include "Location\LanguageManager.h"

CCore::CCore()
	: m_pRenderManager(NULL)
	, m_pLanguageManager(NULL)
	, m_pFontManager(NULL)
	, m_pInputManager(NULL)
	, m_pActionToInput(NULL)
{
}

CCore::~CCore()
{
	Release();
}

void CCore::Init( HWND hWnd, const SConfig &config )
{
	//Inicializa el Render
	m_pRenderManager = new CRenderManager();
	m_pRenderManager->SetColorDebug( config.color_debug );
	m_pRenderManager->SetColorRelease( config.color_release );
	m_pRenderManager->SetFullscreen( config.bFullscreen );
	m_pRenderManager->SetScreenSize( config.resolution );
	m_pRenderManager->Init(hWnd);

	//Inicializa las fuentes
	m_pFontManager = new CFontManager();
	m_pFontManager->Init( m_pRenderManager );
	m_pFontManager->LoadTTFs( config.fonts_path );

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
	m_pInputManager = new CInputManager();
	m_pInputManager->Init( hWnd, config.resolution, config.bExclusiveModeInMouse );
	m_pActionToInput = new CActionToInput();
}

void CCore::Release()
{
	CHECKED_DELETE(m_pRenderManager);
	CHECKED_DELETE(m_pLanguageManager);
	CHECKED_DELETE(m_pFontManager);
	CHECKED_DELETE(m_pInputManager);
	CHECKED_DELETE(m_pActionToInput);
}

void CCore::Update(float ElapsedTime)
{
	m_pInputManager->Update();
}

void CCore::Render()
{
}