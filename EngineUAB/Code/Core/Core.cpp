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
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Commands\SceneRendererCommandManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Modifiers\ModifierManager.h"
#include "ScriptManager.h"
#include "PhysicsManager.h"
#include "DebugGUIManager.h"
#include "Stadistics\Stadistics.h"
#include "Billboard\BillboardManager.h"
#include "Particles\ParticlesSystemManager.h"

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
	, m_pROTManager(NULL)
	, m_pSceneRendererCommandManager(NULL)
	, m_pRenderableObjectsLayersManager(NULL)
	, m_pCamera(NULL)
	, m_pTimer(NULL)
	, m_pScriptManager(NULL)
	, m_pPhysicsManager(NULL)
	, m_pDebugGUIManager(NULL)
	, m_pStadistics(NULL)
	, m_pBillboardManager(NULL)
	, m_pParticleManager(NULL)
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
	CHECKED_DELETE(m_pROTManager);
	CHECKED_DELETE(m_pSceneRendererCommandManager);
	CHECKED_DELETE(m_pRenderableObjectsLayersManager);
	CHECKED_DELETE(m_pScriptManager);
	CHECKED_DELETE(m_pPhysicsManager);
	CHECKED_DELETE(m_pDebugGUIManager);
	CHECKED_DELETE(m_pStadistics);
	CHECKED_DELETE(m_pBillboardManager);
	CHECKED_DELETE(m_pParticleManager);

	m_pCamera = NULL; //La cámara la elimina el proceso
	m_pTimer = NULL;
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
			m_pTextureManager = new CTextureManager("NoTexture", config.no_texture_path);

			//Inicia los efectos
			m_pEffectManager = new CEffectManager();
			m_pEffectManager->Load( config.effects_path );

			//Inicia los Renderable Object Techniques
			m_pROTManager = new CRenderableObjectTechniqueManager();
			m_pROTManager->Load(config.renderable_object_techniques_path);

			//Inicia los meshes
			m_pStaticMeshManager = new CStaticMeshManager();
			m_bIsOk = m_pStaticMeshManager->Load( config.static_meshes_path );

			m_pAnimatedModelManager = new CAnimatedModelManager();
			m_pAnimatedModelManager->Load( config.animated_models_path );

			//Inicia el Renderable Objects Layers Manager
			m_pRenderableObjectsLayersManager = new CRenderableObjectsLayersManager();
			m_pRenderableObjectsLayersManager->Load(config.renderable_objects_path);

			//Inicia las luces
			m_pLightManager = new CLightManager();
			m_pLightManager->Load( config.lights_path );

			//Inicia el Scene Renderer Command Manager
			m_pSceneRendererCommandManager = new CSceneRendererCommandManager();
			m_pSceneRendererCommandManager->Load(config.scene_renderer_command_manager_path);

			m_pPhysicsManager = new CPhysicsManager();
			m_pPhysicsManager->Init("");

			//Billboards & Particles
			m_pBillboardManager = new CBillboardManager();
			m_pBillboardManager->Load( config.billboards_path );

			m_pParticleManager = new CParticlesSystemManager();
			m_pParticleManager->Load( config.particles_path );

#if defined (_DEBUG)
			//Inicializa las estadísticas
			m_pStadistics = new CStadistics();

			//Inicializa el manager de interfaz de debugeo
			m_pDebugGUIManager = new CDebugGUIManager();
			m_bIsOk = m_pDebugGUIManager->Init( config.modifiers_path, config.debug_options_path );		
#endif

			m_pScriptManager = new CScriptManager();
			m_pScriptManager->Load( config.scripts_path );
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
#if defined(_DEBUG)
	m_pStadistics->ResetAll();
#endif

	//m_pInputManager->Update();
	m_pActionToInput->Update();
	m_pPhysicsManager->Update(ElapsedTime);
	m_pBillboardManager->Update(ElapsedTime);
	m_pParticleManager->Update(ElapsedTime);

#if defined(_DEBUG)
	m_pDebugGUIManager->Update(ElapsedTime);
#endif
}

void CCore::Render()
{
}

void CCore::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CCore::Reload->Reload de todo el motor.");

	m_pFontManager->ReloadTTFs();
	m_pLanguageManager->LoadXMLs();
	m_pActionToInput->Reload();

	//Obtenemos los valores de la textura NoTexture
	std::string l_noTexName = m_pTextureManager->GetNoTextureName();
	std::string l_noTexPath = m_pTextureManager->GetNoTexturePath();
	CHECKED_DELETE(m_pTextureManager);
	m_pTextureManager = new CTextureManager(l_noTexName, l_noTexPath);
	
	m_pLightManager->Reload();
	m_pEffectManager->Reload();
	m_pROTManager->Reload();
	m_pStaticMeshManager->Reload();
	//m_pAnimatedModelManager->Reload();
	m_pRenderableObjectsLayersManager->Reload();
	m_pSceneRendererCommandManager->Reload();
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

void CCore::ReloadSceneRendererCommandManager()
{
	m_pSceneRendererCommandManager->Reload();
}

void CCore::ReloadRenderableObjectsLayersManager()
{
	m_pRenderableObjectsLayersManager->Reload();
}

void CCore::ReloadEffects()
{
	m_pEffectManager->Reload();
}

void CCore::ReloadMeshes()
{
	m_pStaticMeshManager->Reload();
	//m_pAnimatedModelManager->Reload();
	m_pRenderableObjectsLayersManager->Reload();
}

void CCore::ReloadPools()
{
	m_pROTManager->Reload();
	//TODO: Cambiar el Reload de EffectManager para uno que recargue los XML
	m_pEffectManager->Reload();
	m_pStaticMeshManager->CreateRenderableObjectsTechniques();
	m_pSceneRendererCommandManager->Reload();
}

void CCore::ReloadScripts()
{
	m_pScriptManager->Reload();
}

void CCore::ReloadLights()
{
	m_pLightManager->Reload();
}