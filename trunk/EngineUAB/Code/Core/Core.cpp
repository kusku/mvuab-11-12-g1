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

#include "DebugGUIManager.h"
#include "DebugOptions\DebugOptions.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Stadistics\Stadistics.h"


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

#include "StatesMachine\EntityManager.h"
#include "StatesMachine\MessageDispatcher.h"

#include "Movement\WayPointManager.h"
#include "EngineProcess.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------
CCore::CCore ( void )
	: m_bIsOk							( false )
	, m_bDebugMode						( false )
	, m_bReleaseMode					( true )
	, m_bIsGameMode						( true )
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
	, m_pEntityManager					( NULL )
	, m_pMessageDispatcher				( NULL )
	, m_WayPointManager					( NULL )
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
	CHECKED_DELETE ( m_WayPointManager );
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
	CHECKED_DELETE ( m_pEntityManager );
	CHECKED_DELETE ( m_pMessageDispatcher );
	
	m_pCamera = NULL; //La cámara la elimina el proceso
	m_pTimer = NULL;
}

bool CCore::Init( HWND _HWnd, const SConfig &config )
{
	m_bIsOk = false;
	LOGGER->AddNewLog(ELL_INFORMATION, "CCore:: Inicializando Core");

	m_Config = config;

	m_bDebugMode = config.bDebugMode;
	m_bReleaseMode = !config.bDebugMode;

	//Inicializa el Render
	m_pRenderManager = new CRenderManager();
	m_pRenderManager->SetColorDebug	 ( config.color_debug );
	m_pRenderManager->SetColorRelease( config.color_release );
	m_pRenderManager->SetFullscreen  ( config.bFullscreen );
	m_pRenderManager->SetScreenSize  ( config.resolution );
	m_bIsOk = m_pRenderManager->Init ( _HWnd );

	//SCRIPT->RunFile( "main.lua" );

	m_pScriptManager = new CScriptManager();

	if ( m_bIsOk )
	{
		//Inicializa las fuentes
		m_pFontManager = new CFontManager();
		/*m_bIsOk = m_pFontManager->Init( m_pRenderManager );
		m_pFontManager->LoadTTFs( config.fonts_path );*/

		if( m_bIsOk )
		{
			//Inicializa los lenguajes
			m_pLanguageManager = new CLanguageManager();
			/*int count = config.languages_path.size();
			for(int i=0; i<count; ++i)
			{
				m_pLanguageManager->SetXmlFile( config.languages_path[i] );
			}
			m_pLanguageManager->LoadXMLs();
			m_pLanguageManager->SetCurrentLanguage( config.default_language );*/

			//Inicializa los inputs
			m_pActionToInput = new CActionToInput();
			m_bIsOk = m_pActionToInput->Init( _HWnd, config.resolution, config.bExclusiveModeInMouse );
			/*m_pActionToInput->LoadXML( config.input_path );
			m_pInputManager = m_pActionToInput->GetInputManager();*/

			//Crea el TextureManager y añade una textura por sí al renderizar algun modelo no encuentra una textura
			m_pTextureManager = new CTextureManager("NoTexture", config.no_texture_path);

			//Inicia los efectos
			m_pEffectManager = new CEffectManager();
			/*m_pEffectManager->Load( config.effects_path );*/

			//Inicia los Renderable Object Techniques
			m_pROTManager = new CRenderableObjectTechniqueManager();
			/*m_pROTManager->Load(config.renderable_object_techniques_path);*/

			//Inicia los meshes
			m_pStaticMeshManager = new CStaticMeshManager();
			/*m_bIsOk = m_pStaticMeshManager->Load( config.static_meshes_path );*/

			m_pAnimatedModelManager = new CAnimatedModelManager();
			/*m_pAnimatedModelManager->Load( config.animated_models_path );*/

			//Inicia el Renderable Objects Layers Manager
			m_pRenderableObjectsLayersManager = new CRenderableObjectsLayersManager();
			/*m_pRenderableObjectsLayersManager->Load(config.renderable_objects_path);*/

			//Inicia las luces
			m_pLightManager = new CLightManager();
			/*m_pLightManager->Load( config.lights_path );*/

			//Inicia el Scene Renderer Command Manager
			m_pSceneRendererCommandManager = new CSceneRendererCommandManager();
			/*m_pSceneRendererCommandManager->Load(config.scene_renderer_command_manager_path);*/

			m_pPhysicsManager = new CPhysicsManager();
			/*m_pPhysicsManager->Init();*/

			//Billboards & Particles
			m_pBillboardManager = new CBillboardManager();
			/*m_pBillboardManager->Load( config.billboards_path );*/

			m_pParticlesManager = new CParticlesSystemManager();
			/*m_pParticlesManager->Load( config.particles_path );*/

			//GUI
			m_pGUIManager = new CGUIManager( CORE->GetRenderManager()->GetScreenSize() );
			/*bool isOk = m_pGUIManager->Init( config.gui_path );*/

			// Inicialización del sistema de triggers
			m_pTriggersManager = new CTriggersManager();
			/*m_pTriggersManager->Load ( config.triggers_system_path );*/

			// Inicialización del sistema de sonido
			m_pSoundManager = new CSoundManager();
			/*m_pSoundManager->Load ( config.sound_system_path );*/

			if( m_bDebugMode )
			{
				//Inicializa las estadísticas
				m_pStadistics = new CStadistics();

				//Inicializa el manager de interfaz de debugeo
				m_pDebugGUIManager = new CDebugGUIManager();
				/*m_bIsOk = m_pDebugGUIManager->Init( config.modifiers_path, config.debug_options_path );		*/
			}

			// Inicialización de la lógica
			m_pEntityManager = new CEntityManager();
			m_pMessageDispatcher = new CMessageDispatcher();
			
			m_WayPointManager = new CWayPointManager();
			m_WayPointManager->Load(config.waypoints_path);

			CCore::RegisterMethods();
			CRenderManager::RegisterMethods();
			CGUIManager::RegisterMethods();
			CSoundManager::RegisterMethods();
			CTriggersManager::RegisterMethods();
			CDebugGUIManager::RegisterMethods();

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

	m_bIsOk = true;
	return m_bIsOk;
}

void CCore::Update( float _ElapsedTime )
{
	if( m_bDebugMode )
	{
		m_pStadistics->ResetAll();
	}

	m_pActionToInput->Update();

	if( m_bIsGameMode )
	{
		m_pPhysicsManager->Update( _ElapsedTime );
		m_pBillboardManager->Update( _ElapsedTime );
		m_pParticlesManager->Update( _ElapsedTime );
	}
	else
	{
		m_pGUIManager->Update( _ElapsedTime );
	}
	
	// Tratamos mensajes en cola en cada frame --> se establecen los cambios de estado de las entidades registradas
	m_pMessageDispatcher->DispatchDelayedMessages();

	m_pSoundManager->Update(_ElapsedTime);

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
bool CCore::LoadFonts()
{
	m_bIsOk = m_pFontManager->Init( m_pRenderManager );
	m_bIsOk = m_pFontManager->LoadTTFs( m_Config.fonts_path );
	return m_bIsOk;
}

bool CCore::LoadLanguages()
{
	int count = m_Config.languages_path.size();
	for(int i=0; i<count; ++i)
	{
		m_pLanguageManager->SetXmlFile( m_Config.languages_path[i] );
	}
	m_pLanguageManager->LoadXMLs();
	m_pLanguageManager->SetCurrentLanguage( m_Config.default_language );

	return true;
}

bool CCore::LoadInputs()
{
	m_pActionToInput->LoadXML( m_Config.input_path );
	m_pInputManager = m_pActionToInput->GetInputManager();
	return true;
}

bool CCore::LoadEffects()
{
	return m_pEffectManager->Load( m_Config.effects_path );
}

bool CCore::LoadROTechniques()
{
	return m_pROTManager->Load(m_Config.renderable_object_techniques_path);
}

bool CCore::LoadStaticMeshes()
{
	m_bIsOk = m_pStaticMeshManager->Load( m_Config.static_meshes_path );
	return m_bIsOk;
}

bool CCore::LoadAnimatedModels()
{
	m_pAnimatedModelManager->Load( m_Config.animated_models_path );
	return true;
}

bool CCore::LoadROLayers()
{
	m_pRenderableObjectsLayersManager->Load(m_Config.renderable_objects_path);
	return true;
}

bool CCore::LoadLights()
{
	return m_pLightManager->Load( m_Config.lights_path );
}

bool CCore::LoadRenderCommands()
{
	m_pSceneRendererCommandManager->Load(m_Config.scene_renderer_command_manager_path, false);
	return m_pSceneRendererCommandManager->Load(m_Config.scene_renderer_gui_command_manager_path, true);
}

bool CCore::LoadPhysics()
{
	return m_pPhysicsManager->Init();
}

bool CCore::LoadBillboards()
{
	return m_pBillboardManager->Load( m_Config.billboards_path );
}

bool CCore::LoadParticles()
{
	return m_pParticlesManager->Load( m_Config.particles_path );
}

bool CCore::LoadGUI()
{
	return m_pGUIManager->Init( m_Config.gui_path );
}

bool CCore::LoadTriggers()
{
	m_pTriggersManager->Init();
	return m_pTriggersManager->Load( m_Config.triggers_system_path );
}

bool CCore::LoadSounds()
{
	return m_pSoundManager->Load( m_Config.sound_system_path );
}

bool CCore::LoadDebugGUI()
{
	return m_pDebugGUIManager->Init( m_Config.modifiers_path, m_Config.debug_options_path );		
}

void CCore::SetGameMode(bool _GameMode)
{ 
	m_bIsGameMode = _GameMode; 
	m_pSceneRendererCommandManager->SetRenderScene(_GameMode); 
}

// -----------------------------------------
//			 MÈTODES UPDATE INPUTS 
// -----------------------------------------

// ----------------------------------------------------------------------------------------------------------------
//	UpdateInputs: Mira si se realizan acciones de teclado básicamente de reloads y debugs que sea propios del motor
// ----------------------------------------------------------------------------------------------------------------
void CCore::UpdateInputs( float _ElapsedTime )
{
	CActionToInput *l_Action2Input = CORE->GetActionToInput();

	if(l_Action2Input == NULL || _ElapsedTime < 0)
	{
		return;
	}
	
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
		CORE->ReloadAnimatedModels();
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
		SCRIPT->RunCode("reload_gui()");
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_SOUNDS ) )				// Recarrega els sons
	{
		SCRIPT->RunCode("reload_sounds()");
	}

	if( l_Action2Input->DoAction( ACTION_RELOAD_TRIGGERS ) )				// Recarrega els triggers
	{
		SCRIPT->RunCode("reload_triggers()");
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
	bool action  =_Action2Input.DoAction( ACTION_CONSOLE );
	if(action)						// Activa/Desactiva la consola
	{
		SCRIPT->RunCode("toggle_console()");
	}

	//Show & Unshow de debuggers info									// Activa/Desactiva info de debug 
	if( _Action2Input.DoAction( ACTION_DEBUG_INFO ) )
	{
		bool visible = CORE->GetDebugGUIManager()->GetDebugRender()->GetVisible();
		CORE->GetDebugGUIManager()->GetDebugRender()->SetVisible(!visible);
	}

	//Show & Unshow de debuggers options
	if( _Action2Input.DoAction( ACTION_DEBUG_OPTIONS ) )				// Activa/Desactiva opciones de debug gui
	{
		l_DebugOptions->SetActive( !l_DebugOptions->GetActive() );
	}

	//Show & Unshow de modifiers
	if( _Action2Input.DoAction( ACTION_SHOW_MODIFIERS ) )				// Activa/Desactiva modificadores
	{
		bool visible = l_pModifierManager->GetVisible();
		l_pModifierManager->SetVisible( !visible );
	}

	//Show & Unshow el Logger
	if( _Action2Input.DoAction( ACTION_LOGGER ) )						// Activa/Desactiva consola
	{
		bool visible = CORE->GetDebugGUIManager()->GetLogRender()->GetVisible();
		CORE->GetDebugGUIManager()->GetLogRender()->SetVisible(!visible);
	}

	//Logger actions
	if( _Action2Input.DoAction( ACTION_LOGGER_PAGEDOWN ) )
	{
		CORE->GetDebugGUIManager()->GetLogRender()->PageDown();
	}

	if( _Action2Input.DoAction( ACTION_LOGGER_PAGEUP ) )
	{
		CORE->GetDebugGUIManager()->GetLogRender()->PageUp();
	}

	if( _Action2Input.DoAction( ACTION_LOGGER_PREVLINE ) )
	{
		CORE->GetDebugGUIManager()->GetLogRender()->PrevLine();
	}

	if( _Action2Input.DoAction( ACTION_LOGGER_NEXTLINE ) )
	{
		CORE->GetDebugGUIManager()->GetLogRender()->NextLine();
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

void CCore::ReloadParticles()
{
	m_pParticlesManager->Reload();
}

void CCore::ReloadTriggers()
{
	m_pTriggersManager->Reload();
}

void CCore::ReloadGUI()
{
	m_pGUIManager->ReloadGuiFiles();
}

void CCore::ReloadSounds()
{
	m_pSoundManager->Reload();
}

void CCore::RegisterMethods()
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CEngineProcess>("CEngineProcess")
	];

	module(state) [
		class_<CCore>("CCore")
			.def("reload_all", &CCore::Reload)
			.def("reload_fonts", &CCore::ReloadTTFs)
			.def("reload_languages", &CCore::ReloadLanguages)
			.def("reload_inputs", &CCore::ReloadInputs)
			.def("reload_render_commands", &CCore::ReloadSceneRendererCommandManager)
			.def("reload_renderable_objects_layers", &CCore::ReloadRenderableObjectsLayersManager)
			.def("reload_effects", &CCore::ReloadEffects)
			.def("reload_meshes", &CCore::ReloadMeshes)
			.def("reload_pools", &CCore::ReloadPools)
			.def("reload_scripts", &CCore::ReloadScripts)
			.def("reload_lights", &CCore::ReloadLights)
			.def("reload_physics", &CCore::ReloadPhysics)
			.def("reload_billboards", &CCore::ReloadBillboards)
			.def("reload_particles", &CCore::ReloadParticles)
			.def("reload_triggers", &CCore::ReloadTriggers)
			.def("reload_gui", &CCore::ReloadGUI)
			.def("reload_sounds", &CCore::ReloadSounds)
			.def("get_debug_gui_manager", &CCore::GetDebugGUIManager)
			.def("get_stadistics", &CCore::GetStadistics)
			.def("get_gui_manager", &CCore::GetGUIManager)
			.def("get_sound_manager", &CCore::GetSoundManager)
			.def("get_process", &CCore::GetProcess)
			.def("load_fonts", &CCore::LoadFonts)
			.def("load_languages", &CCore::LoadLanguages)
			.def("load_inputs", &CCore::LoadInputs)
			.def("load_effects", &CCore::LoadEffects)
			.def("load_renderable_objects_techniques", &CCore::LoadROTechniques)
			.def("load_static_meshes", &CCore::LoadStaticMeshes)
			.def("load_animated_models", &CCore::LoadAnimatedModels)
			.def("load_renderable_objects_layers", &CCore::LoadROLayers)
			.def("load_lights", &CCore::LoadLights)
			.def("load_render_commands", &CCore::LoadRenderCommands)
			.def("load_physics", &CCore::LoadPhysics)
			.def("load_billboards", &CCore::LoadBillboards)
			.def("load_particles", &CCore::LoadParticles)
			.def("load_gui", &CCore::LoadGUI)
			.def("load_triggers", &CCore::LoadTriggers)
			.def("load_sounds", &CCore::LoadSounds)
			.def("load_debug_gui", &CCore::LoadDebugGUI)
			.def("is_game_mode", &CCore::IsGameMode)
	];
}