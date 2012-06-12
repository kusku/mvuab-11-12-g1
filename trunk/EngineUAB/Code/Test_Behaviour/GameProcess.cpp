#include "GameProcess.h"
#include "TestBehaviourDefs.h"
#include "Characters\PlayerDef.h"

#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"

#include "RenderManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"

#include "Characters\CharacterManager.h"
#include "Characters\Character.h"
#include "Characters\Properties\Properties.h"
#include "Characters\Properties\PropertiesManager.h"
//#include "Characters\Player\Player.h"
//#include "Characters\Enemy\Enemy.h"

#include "StatesMachine\ScriptedStateMachine.h"
#include "StatesMachine\BaseGameEntity.h"

#include "DebugSceneBehaviour\Scene.h"
#include "SoundManager.h"
#include "ActionToInput.h"

#include "RegisterToLua.h"
#include "Foo.h"

#include "Core.h"
#include "Base.h"

#if defined (_DEBUG)
	#include "Memory/MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CGameProcess::CGameProcess(HWND hWnd)
	: m_hWnd	( hWnd )
	, m_pScene	( NULL )
	, m_IsOK	( false )
{
}

CGameProcess::~CGameProcess(void)
{
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CGameProcess::Init()
{
	RegisterMethods();

	if( INIT_GUI )
	{
		CORE->GetScriptManager()->RunCode("load_data()");
	}
	else
	{
		CORE->GetScriptManager()->RunCode("load_all()");
	}
	
	LoadGameObjects();

	return true;
}

void CGameProcess::Update( float _ElapsedTime )
{
	if (!m_IsOK) 
	{
		return;
	}

	UpdateInputs( _ElapsedTime );	

	if ( m_pCharactersManager->GetPlayer()->GetLocked() )
	{
		m_FreeCamera.Update( _ElapsedTime, m_pCamera );
	}

	m_pCharactersManager->Update ( _ElapsedTime );

	CORE->GetRenderableObjectsLayersManager()->Update( _ElapsedTime );
}

void CGameProcess::Render(CRenderManager &RM)
{
}

void CGameProcess::UpdateInputs ( float _ElapsedTime )
{
	if( CORE->GetActionToInput()->DoAction("ReloadScripts") )
	{
		ReloadGameObjects();
		/*m_pThPSCamera->SetObject3D( m_pCharacterManager->GetPlayer());*/
	}

	if( CORE->GetActionToInput()->DoAction("CommutationCamera") )
	{
		if( m_pCamera == m_pThPSCamera )
		{
			m_pCamera = m_pThPSFreeCamera;
			CORE->SetCamera( m_pThPSFreeCamera );
			m_pCharactersManager->GetPlayer()->SetLocked( true );
		}
		else
		{
			m_pCamera = m_pThPSCamera;
			CORE->SetCamera( m_pThPSCamera );
			m_pCharactersManager->GetPlayer()->SetLocked( false );
		}
	}

	/*if( m_pCamera == static_cast<CCamera*>(m_pThPSCamera) )
	{
		if( CORE->GetActionToInput()->DoAction(ACTION_MOVE_PLAYER_FOWARD) || 
			CORE->GetActionToInput()->DoAction(ACTION_MOVE_PLAYER_BACK) ||
			CORE->GetActionToInput()->DoAction(ACTION_MOVE_PLAYER_LEFT) ||
			CORE->GetActionToInput()->DoAction(ACTION_MOVE_PLAYER_RIGHT))
		{
			if( m_AudioIsFade )
			{
				CORE->GetSoundManager()->FadeInSource(m_uAudioID, 0.5f);
				m_AudioIsFade = false;
			}
		}
		else
		{
			if( !m_AudioIsFade )
			{
				CORE->GetSoundManager()->FadeOutSource(m_uAudioID, 0.5f);
				m_AudioIsFade = true;
			}
		}
	}*/
}

//void CGameProcess::AddEnemy ( CEnemy* _pEnemy )
//{
//	m_pCharactersManager->AddEnemy ( _pEnemy );
//}

void CGameProcess::ReloadGameObjects( void )
{
	CHECKED_DELETE(m_pCharactersManager);
	LoadGameObjects();
}

bool CGameProcess::LoadMainScript( void )
{
	return SCRIPT->Load("./Data/XML/script_gameplay.xml");
}

void CGameProcess::LoadGameObjects()
{
	//Crea los datos para el gameplay
	m_pCharactersManager = new CCharactersManager();
	
	//Crea escena debug 
	//m_pScene = new CScene();

	//Carga los scripts del juego
	m_IsOK = LoadMainScript();

	if ( !m_IsOK )
		return;

	// por si se desea hacer alguna mariconada...
	//SCRIPT->RunCode("init_game_data()");

	CBaseGameEntity::Initialize();

	// Inicializa el gestor de player y enemigos. Carga propiedades y estados de todo.
	if ( !m_pCharactersManager->Initialize ( ) )
		return;

	CCharacter * l_Player = m_pCharactersManager->GetPlayer();

	//Crea la cámara
	float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_pCharactersManager->GetPlayer(), 12.0f, 4.f, 0.f, "Caperucita");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);
}

//-------------------------------------
//--Registrador de métodos en LUA------
//-------------------------------------
void CGameProcess::RegisterMethods( void )
{
	lua_State * l_State = SCRIPT->GetLuaState();

	/*RegisterToLuaTelegram(l_State);
	RegisterToLuaMessageDispacher(l_State);*/
//	RegisterToLuaCNamed					( l_State );
	RegisterToLuaBaseGameEntity			( l_State );
	RegisterToLuaCharacter				( l_State );
	RegisterToLuaCharacterManager		( l_State );
	RegisterToLuaStateMachine			( l_State );
	RegisterToLuaState					( l_State );
	RegisterToLuaProperties				( l_State );
	RegisterToLuaGameProcess			( l_State );
}

void CGameProcess::RegisterToLuaGameProcess( lua_State* _pLua )
{
	module(_pLua) [
		def("get_game_process", GetGameProcess)
	];

	module(_pLua) [
		class_<CGameProcess>("CGameProcess")
			//.def("get_game_process"		, &CGameProcess::GetGameProcess)
			.def("get_character_manager", &CGameProcess::GetCharactersManager)
			//.def("add_enemy",			  &CGameProcess::AddEnemy)
	];
}

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------

CGameProcess* CGameProcess::GetGameProcess( void )
{
	CEngineProcess *l_pProces = CORE->GetProcess();
	return static_cast<CGameProcess*> ( l_pProces );
}



