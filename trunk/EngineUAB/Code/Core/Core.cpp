#include "Core.h"
#include "Base.h"

CCore::CCore()
	: m_pRenderManager(NULL)
	, m_pLanguageManager(NULL)
	, m_pFontManager(NULL)
{
}

CCore::~CCore()
{
	Release();
}

void CCore::Init(HWND hWnd, const SConfig &config)
{
	m_pRenderManager = new CRenderManager();
	m_pRenderManager->SetColorDebug( config.color_debug );
	m_pRenderManager->SetColorRelease( config.color_release );
	m_pRenderManager->SetFullscreen( config.fullscreen );
	m_pRenderManager->SetScreenSize( config.resolution );
	m_pRenderManager->Init(hWnd);

	m_pFontManager = new CFontManager();
	m_pFontManager->Init( m_pRenderManager );
	m_pFontManager->LoadTTFs( config.fonts_path );

	m_pLanguageManager = new CLanguageManager();
	int count = config.languages_path.size();
	for(int i=0; i<count; ++i)
	{
		m_pLanguageManager->SetXmlFile( config.languages_path[i] );
	}
	m_pLanguageManager->LoadXMLs();
	m_pLanguageManager->SetCurrentLanguage( config.default_language );
}

void CCore::Release()
{
	CHECKED_DELETE(m_pRenderManager);
	CHECKED_DELETE(m_pLanguageManager);
	CHECKED_DELETE(m_pFontManager);
}

void CCore::Update(float ElapsedTime)
{
}

void CCore::Render()
{
}