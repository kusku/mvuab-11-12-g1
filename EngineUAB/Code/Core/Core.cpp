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

void CCore::Init(HWND hWnd)
{
	m_pRenderManager = new CRenderManager();
	m_pRenderManager->Init(hWnd);

	m_pFontManager = new CFontManager();
	m_pFontManager->Init( m_pRenderManager );
	m_pFontManager->LoadTTFs( "./data/fonts/fonts.xml" );

	m_pLanguageManager = new CLanguageManager();
	m_pLanguageManager->SetXmlFile( "./data/language/catalan.xml" );
	m_pLanguageManager->SetXmlFile( "./data/language/spanish.xml" );
	m_pLanguageManager->SetXmlFile( "./data/language/english.xml" );
	m_pLanguageManager->LoadXMLs();
	m_pLanguageManager->SetCurrentLanguage( "catalan" );
}

void CCore::Release()
{
	CHECKED_DELETE(m_pRenderManager);
	CHECKED_DELETE(m_pLanguageManager);
	CHECKED_DELETE(m_pFontManager);
}

void CCore::Update(float ElapsedTime)
{
	int a=1;
}

void CCore::Render()
{
	int a=1;
}

CRenderManager* CCore::GetRenderManager() const
{
	return m_pRenderManager;
}

CFontManager* CCore::GetFontManager() const
{
	return m_pFontManager;
}

CLanguageManager* CCore::GetLanguageManager() const
{
	return m_pLanguageManager;
}