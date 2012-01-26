#include "Core.h"
#include "Base.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Fonts\FontManager.h"
#include "Location\LanguageManager.h"
#include "Textures\TextureManager.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "RenderableObjects\StaticMesh\StaticMeshManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "Lights\LightManager.h"
#include "Effects\EffectManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CCore::CCore()
	: m_pRenderManager(NULL)
	, m_pLanguageManager(NULL)
	, m_pFontManager(NULL)
	, m_pInputManager(NULL)
	, m_pActionToInput(NULL)
	, m_pTextureManager(NULL)
	, m_pStaticMeshManager(NULL)
	, m_pRenderableObjectsManager(NULL)
	, m_pAnimatedModelManager(NULL)
	, m_pLightManager(NULL)
	, m_bIsOk(false)
{
}

CCore::~CCore()
{
	Release();
}

void CCore::Release()
{
	CHECKED_DELETE(m_pRenderManager);
	CHECKED_DELETE(m_pLanguageManager);
	CHECKED_DELETE(m_pFontManager);
	//CHECKED_DELETE(m_pInputManager);
	CHECKED_DELETE(m_pActionToInput);
	CHECKED_DELETE(m_pTextureManager);
	CHECKED_DELETE(m_pStaticMeshManager);
	CHECKED_DELETE(m_pRenderableObjectsManager);
	CHECKED_DELETE(m_pAnimatedModelManager);
	CHECKED_DELETE(m_pLightManager);
	CHECKED_DELETE(m_pEffectManager);
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

			//Crea el TextureManager y añade una textura por sí al renderizar algun modelo no encuentra una textura
			m_pTextureManager = new CTextureManager();
			CTexture* l_Tex = new CTexture();
			l_Tex->Load( config.no_texture_path );
			m_pTextureManager->AddResource( "NoTexture", l_Tex );
			m_pTextureManager->SetNoTextureName("NoTexture");
			l_Tex = NULL;

			//Inicia los meshes
			m_pStaticMeshManager = new CStaticMeshManager();
			m_bIsOk = m_pStaticMeshManager->Load( config.static_meshes_path );

			m_pAnimatedModelManager = new CAnimatedModelManager();
			m_pAnimatedModelManager->Load( config.animated_models_path );

			m_pRenderableObjectsManager = new CRenderableObjectsManager();
			m_pRenderableObjectsManager->Load( config.renderable_objects_path );

			m_pLightManager = new CLightManager();
			m_pLightManager->Load( config.lights_path );

			m_pEffectManager = new CEffectManager();
			m_pEffectManager->Load( config.effects_path );
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

void CCore::Update(float ElapsedTime)
{
	//m_pInputManager->Update();
	m_pActionToInput->Update();
}

void CCore::Render()
{
}

void CCore::Reload()
{
	m_pFontManager->ReloadTTFs();
	m_pLanguageManager->LoadXMLs();
	m_pActionToInput->Reload();
	CHECKED_DELETE(m_pTextureManager);
	m_pTextureManager = new CTextureManager();
	m_pStaticMeshManager->Reload();
}

void CCore::ReloadTTFs()
{
	m_pFontManager->ReloadTTFs();
}

void CCore::ReloadLanguages()
{
	m_pLanguageManager->LoadXMLs();
}

void CCore::ReloadInputs()
{
	m_pActionToInput->Reload();
}

void CCore::ReloadRenderableObjects()
{
	m_pRenderableObjectsManager->Reload();
}