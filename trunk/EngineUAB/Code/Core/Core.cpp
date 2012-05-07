#include "Core.h"
#include "Base.h"
#include "CoreDefs.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Commands\SceneRendererCommandManager.h"

#include "Fonts\FontManager.h"
#include "Location\LanguageManager.h"
#include "Textures\TextureManager.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"

#include "RenderableObjects\StaticMesh\StaticMeshManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"

#include "Lights\LightManager.h"
#include "Effects\EffectManager.h"

#include "DebugOptions\DebugOptions.h"
#include "DebugGUIManager.h"
#include "Console\Console.h"
#include "Modifiers\ModifierManager.h"
#include "DebugGUIManager.h"
#include "Stadistics\Stadistics.h"
#include "Scripting\ScriptManager.h"

#include "PhysicsManager.h"
#include "Billboard\BillboardManager.h"
#include "Particles\ParticlesSystemManager.h"
#include "GUIManager.h"
#include "Triggers\TriggersManager.h"
#include "SoundManager.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------
CCore::CCore ( void )
	: m_bIsOk							( false )
	, m_bDebugMode						( false )
	, m_pRenderManager					( NULL )
	, m_pFontManager					( NULL )
	, m_pLanguageManager				( NULL )
	, m_pInputManager					( NULL )
	, m_pActionToInput					( NULL )
	, m_pTextureManager					( NULL )
	, m_pStaticMeshManager				( NULL )
	, m_pAnimatedModelManager			( NULL )
	, m_pLightManager					( NULL )
	, m_pRenderableObjectsManager		( NULL )
	, m_pEffectManager					( NULL )
	, m_pROTManager						( NULL ) 
	, m_pSceneRendererCommandManager	( NULL ) 
	, m_pRenderableObjectsLayersManager	( NULL )
	, m_pCamera							( NULL )
	, m_pTimer							( NULL ) 
	, m_pDebugGUIManager				( NULL ) 
	, m_pStadistics						( NULL )
	, m_pScriptManager					( NULL )
	, m_pPhysicsManager					( NULL )
	, m_pGUIManager						( NULL )
	, m_pBillboardManager				( NULL )
	, m_pParticlesManager				( NULL )
	, m_pTriggersManager				( NULL )
	, m_pSoundManager					( NULL )
{
}

CCore::~CCore ( void )
{
	Done();
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CCore::Done ( void )
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

void CCore::Release ( void )
{
	CHECKED_DELETE ( m_pRenderManager );
	CHECKED_DELETE ( m_pLanguageManager );
	CHECKED_DELETE ( m_pFontManager );
	CHECKED_DELETE ( m_pActionToInput );
	CHECKED_DELETE ( m_pTextureManager );
	CHECKED_DELETE ( m_pStaticMeshManager );
	CHECKED_DELETE ( m_pRenderableObjectsManager );
	CHECKED_DELETE ( m_pAnimatedModelManager );
	CHECKED_DELETE ( m_pLightManager );
	CHECKED_DELETE ( m_pEffectManager );
	CHECKED_DELETE ( m_pROTManager );
	CHECKED_DELETE ( m_pSceneRendererCommandManager );
	CHECKED_DELETE ( m_pRenderableObjectsLayersManager );
	CHECKED_DELETE ( m_pScriptManager );
	CHECKED_DELETE ( m_pDebugGUIManager );
	CHECKED_DELETE ( m_pStadistics );
	CHECKED_DELETE ( m_pBillboardManager );
	CHECKED_DELETE ( m_pParticlesManager );
	CHECKED_DELETE ( m_pGUIManager );
	CHECKED_DELETE ( m_pTriggersManager );
	CHECKED_DELETE ( m_pPhysicsManager );
	CHECKED_DELETE ( m_pSoundManager );

	m_pCamera = NULL; //La cámara la elimina el proceso
	m_pTimer = NULL;
}

bool CCore::Init( HWND _HWnd, const SConfig &config )
{
	m_bIsOk = false;
	LOGGER->AddNewLog(ELL_INFORMATION, "CCore:: Inicializando Core");

	m_bDebugMode = config.bDebugMode;

	//Inicializa el Render
	m_pRenderManager = new CRenderManager();
	m_pRenderManager->SetColorDebug	 ( config.color_debug );
	m_pRenderManager->SetColorRelease( config.color_release );
	m_pRenderManager->SetFullscreen  ( config.bFullscreen );
	m_pRenderManager->SetScreenSize  ( config.resolution );
	m_bIsOk = m_pRenderManager->Init ( _HWnd );

	if ( m_bIsOk )
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
			m_bIsOk = m_pActionToInput->Init( _HWnd, config.resolution, config.bExclusiveModeInMouse );
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
			m_pPhysicsManager->Init();

			//Billboards & Particles
			m_pBillboardManager = new CBillboardManager();
			m_pBillboardManager->Load( config.billboards_path );

			m_pParticlesManager = new CParticlesSystemManager();
			m_pParticlesManager->Load( config.particles_path );

			//GUI
			m_pGUIManager = new CGUIManager( CORE->GetRenderManager()->GetScreenSize() );
			bool isOk = m_pGUIManager->Init( config.gui_path );

			// Inicialización del sistema de triggers
			m_pTriggersManager = new CTriggersManager( );
			m_pTriggersManager->Load ( config.triggers_system_path );

			// Inicialización del sistema de sonido
			m_pSoundManager = new CSoundManager();
			m_pSoundManager->Load ( config.sound_system_path );

			if( m_bDebugMode )
			{
				//Inicializa las estadísticas
				m_pStadistics = new CStadistics();

				//Inicializa el manager de interfaz de debugeo
				m_pDebugGUIManager = new CDebugGUIManager();
				m_bIsOk = m_pDebugGUIManager->Init( config.modifiers_path, config.debug_options_path );		
			}

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

void CCore::Update( float _ElapsedTime )
{
	if( m_bDebugMode )
	{
		m_pStadistics->ResetAll();
	}

	m_pActionToInput->Update();
	m_pPhysicsManager->Update	( _ElapsedTime );
	m_pBillboardManager->Update	( _ElapsedTime );
	m_pParticlesManager->Update	( _ElapsedTime );
	m_pGUIManager->Update		( _ElapsedTime );


	if( m_bDebugMode )
	{
		m_pDebugGUIManager->Update( _ElapsedTime );
		UpdateInputs ( _ElapsedTime );
	}
}

void CCore::Render()
{
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------

// -----------------------------------------
//			 MÈTODES UPDATE INPUTS 
// -----------------------------------------

// ----------------------------------------------------------------------------------------------------------------
//	UpdateInputs: Mira si se realizan acciones de teclado básicamente de reloads y debugs que sea propios del motor
// ----------------------------------------------------------------------------------------------------------------
void CCore::UpdateInputs( float _ElapsedTime )
{
	CActionToInput *l_Action2Input = CORE->GetActionToInput();
	
	if( l_Action2Input->DoAction( ACTION_RELOAD_TTFS ) )					// Recarrega de fonts
	{
		SCRIPT->RunCode("reload_fonts()");
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_LANGUAGES) )				// Recarrega de llenguatges
	{
		SCRIPT->RunCode("reload_languages()");
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_ACTIONS ) )					// Recarrega de tecles
	{
		SCRIPT->RunCode("reload_inputs()");
	}

	else if ( l_Action2Input->DoAction ( ACTION_RELOAD_LAYERS ) )			// Recarrega del fitxer de capes i RO
	{
		//CORE->ReloadRenderableObjectsLayersManager();
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_RENDERABLE_OBJECTS ) )		// Recarrega els RO
	{
		SCRIPT->RunCode("reload_renderable_objects_layers()");
	}
	
	if ( l_Action2Input->DoAction ( ACTION_RELOAD_ANIMATIONS ) )			// Recarrega de les animacions
	{ 
		//CORE->ReloadAnimatedModels();
	}
	
	if( l_Action2Input->DoAction( ACTION_RELOAD_MESHES ) )					// Recarrega dels meshes
	{
		SCRIPT->RunCode("reload_meshes()");
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_EFFECTS ) )					// Recarrega dels effectes
	{
		SCRIPT->RunCode("reload_effects()");
	}

	if ( l_Action2Input->DoAction ( ACTION_RELOAD_SHADERS ) )				// Recarrega dels Shaders
	{ 
		SCRIPT->RunCode("reload_shaders()");
		//CORE->ReloadShaders();
	}

	if ( l_Action2Input->DoAction ( ACTION_RELOAD_LIGHTS ) )				// Recarrega de les llums
	{
		SCRIPT->RunCode("reload_debug_lights()");
		//CORE->ReloadLightsModels();
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_POOLS ) )					// Recarrega les pools
	{
		SCRIPT->RunCode("reload_pools()");
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_SCRIPTS ) )					// Recarrega els scripts
	{
		SCRIPT->RunCode("reload_scripts()");
	}

	if ( l_Action2Input->DoAction ( ACTION_RELOAD_PHYSIS ) )				// Recarrega la física
	{
		SCRIPT->RunCode("reload_physics()");
		//CORE->ReloadPhysXManager();
	}
	
	if ( l_Action2Input->DoAction ( ACTION_RELOAD_GUI ) )					// Recarrega la GUI
	{
		CORE->ReloadGUIManager();
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_SOUNDS ) )					// Recarrega els sons
	{
		CORE->GetSoundManager()->Reload();
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_COMMANDS ) )				// Recarrega de comandes
	{
		SCRIPT->RunCode("reload_render_commands()");
	}

	// Solo en mode debug comprovamos estas teclas
	UpdateDebugInputs( _ElapsedTime, *l_Action2Input);
}

// ----------------------------------------------------------------------------------------------------------------
//	UpdateDebugInputs: Mira si se realizan acciones de teclado básicamente de modo debug
// ----------------------------------------------------------------------------------------------------------------
void CCore::UpdateDebugInputs( float _ElapsedTime, CActionToInput &_Action2Input )
{
	CModifierManager *l_pModifierManager = CORE->GetDebugGUIManager()->GetModifierManager();
	CDebugOptions *l_DebugOptions = CORE->GetDebugGUIManager()->GetDebugOptions();

	//Show & Unshow la consola
	if( _Action2Input.DoAction( ACTION_CONSOLE ) )					// Activa/Desactiva la consola
	{
		SCRIPT->RunCode("toggle_console()");
	}

	//Show & Unshow de debuggers
	if( _Action2Input.DoAction( ACTION_DEBUG_OPTIONS ) )				// Activa/Desactiva opciones de debug gui
	{
		l_DebugOptions->SetActive( !l_DebugOptions->GetActive() );
	}

	if( _Action2Input.DoAction( ACTION_SHOW_MODIFIERS ) )				// Activa/Desactiva modificadores
	{
		bool visible = l_pModifierManager->GetVisible();
		l_pModifierManager->SetVisible( !visible );
	}

	//Modifiers actions
	if( !CORE->GetDebugGUIManager()->GetConsole()->IsActive() && !l_DebugOptions->GetActive() )
	{
		if( _Action2Input.DoAction( ACTION_PREVIOUS_MODIFIERS ) && !_Action2Input.DoAction( ACTION_LOGGER_PREVLINE ) )
		{
			l_pModifierManager->MoveToPreviousModifier();
		}

		if( _Action2Input.DoAction( ACTION_NEXT_MODIFIERS ) && !_Action2Input.DoAction( ACTION_LOGGER_NEXTLINE ) )
		{
			l_pModifierManager->MoveToNextModifier();
		}

		if( _Action2Input.DoAction( ACTION_GOTO_MODIFIERS ) )
		{
			l_pModifierManager->GoToModifier();
		}

		if( _Action2Input.DoAction( ACTION_GOTO_ROOT_MODIFIERS ) )
		{
			l_pModifierManager->GoToRoot();
		}

		if( _Action2Input.DoAction( ACTION_ADD_VALUE_MODIFIERS ) || _Action2Input.DoAction( ACTION_ADD_VALUE_MODIFIERS ) )
		{
			l_pModifierManager->AddValueToModifier();
		}

		if( _Action2Input.DoAction( ACTION_SUBS_VALUE_MODIFIERS ) || _Action2Input.DoAction( ACTION_SUBS_VALUE_MODIFIERS ) )
		{
			l_pModifierManager->SubsValueToModifier();
		}
	}

	//Debug Options actions
	if( l_DebugOptions->GetActive() )
	{
		if( _Action2Input.DoAction( ACTION_DEBUG_NEXT_PAGE ) )
		{
			l_DebugOptions->MoveToNextPage();
		}

		if( _Action2Input.DoAction( ACTION_DEBUG_PREVIOUS_PAGE ) )
		{
			l_DebugOptions->MoveToPrevPage();
		}

		if( _Action2Input.DoAction( ACTION_DEBUG_NEXT_LINE ) )
		{
			l_DebugOptions->MoveToNextLine();
		}

		if( _Action2Input.DoAction( ACTION_DEBUG_PREVIOUS_LINE ) )
		{
			l_DebugOptions->MoveToPrevLine();
		}

		if( _Action2Input.DoAction( ACTION_DEBUG_DO_ACTION ) )
		{
			l_DebugOptions->DoAction();
		}
	}
}

// -----------------------------------------
//			  MÈTODES RELOADS
// -----------------------------------------
void CCore::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CCore::Reload->Reload de todo el motor.");

	m_pFontManager->ReloadTTFs		();
	m_pLanguageManager->LoadXMLs	();
	m_pActionToInput->Reload		();

	//Obtenemos los valores de la textura NoTexture
	std::string l_noTexName = m_pTextureManager->GetNoTextureName();
	std::string l_noTexPath = m_pTextureManager->GetNoTexturePath();
	CHECKED_DELETE(m_pTextureManager);
	m_pTextureManager = new CTextureManager(l_noTexName, l_noTexPath);
	
	m_pLightManager->Reload						();
	m_pEffectManager->Reload					();
	m_pROTManager->Reload						();
	m_pStaticMeshManager->Reload				();
	//m_pAnimatedModelManager->Reload();
	m_pRenderableObjectsLayersManager->Reload	();
	m_pPhysicsManager->Reload					();
	m_pBillboardManager->Reload					();
	m_pParticlesManager->Reload					();
	m_pTriggersManager->Reload					();
	m_pGUIManager->ReloadGuiFiles				();
	m_pSoundManager->Reload						();
	m_pScriptManager->Reload					();
	m_pSceneRendererCommandManager->Reload		();
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

void CCore::ReloadAnimatedModels ( void )
{
	/*m_pAnimatedModelsManager->Reload();
	m_pRenderableObjectsLayersManager->Reload ();*/
}

void CCore::ReloadShaders ( void )
{
	m_pEffectManager->ReloadShaders();
}

void CCore::ReloadEffects ( void )
{
	/*m_pEffectManager->Reload ();
	m_pROTManager->Reload ();
	m_pStaticMeshManager->ReloadTecniquesCores();
	m_pSceneRendererCommandManager->Reload();*/
}

void CCore::ReloadMeshes ( void )
{
	m_pStaticMeshManager->Reload();
	//m_pAnimatedModelManager->Reload();
	m_pRenderableObjectsLayersManager->Reload();
}

void CCore::ReloadPools()
{
	//TODO: Cambiar el Reload de EffectManager para uno que recargue los XML
	m_pEffectManager->Reload();

	// Canvio. Antes cargabamos los ROT y después los Efectos. Esto es incorrecto. Antes debemos cambiar los efectos y luego realignarlos al ROTManager. Este manager va a buscar los efectos del EffectManager para ser asignados. 
	m_pROTManager->Reload();
	m_pStaticMeshManager->CreateRenderableObjectsTechniques();
	m_pSceneRendererCommandManager->Reload();
}

void CCore::ReloadScripts()
{
	m_pScriptManager->Reload();
}

void CCore::ReloadPhysics()
{
	m_pPhysicsManager->Reload();
}

void CCore::ReloadLights()
{
	m_pLightManager->Reload();
}

void CCore::ReloadBillboards()
{
	m_pBillboardManager->Reload();
}

void CCore::ReloadParticlesManager()
{
	m_pParticlesManager->Reload();
}

void CCore::ReloadTriggersManager()
{
	m_pTriggersManager->Reload();
}

void CCore::ReloadGUIManager()
{
	m_pGUIManager->ReloadGuiFiles();
}

void CCore::ReloadSoundManager()
{
	m_pSoundManager->Reload();
}


// -----------------------------------------
//				PROPIETATS 
// -----------------------------------------
