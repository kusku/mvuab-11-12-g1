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
#include "Characters\Player\Player.h"
#include "Characters\Properties\Properties.h"
#include "Characters\Properties\PropertiesManager.h"
#include "Characters\Enemy\Enemy.h"

#include "StatesMachine\ScriptedStateMachine.h"
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
	//, m_pPlayer ( NULL )
 {
	m_pCharactersManager	= new CCharactersManager();
	m_pScene = new CScene();
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


	Vect2i pos;
	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	//Establece la cámara
	m_StaticCamera.SetPosition(Vect3f(0.f,1.f,0.f));
	m_StaticCamera.SetPitch(0.0f);
	m_StaticCamera.SetYaw(0.0f);
	m_StaticCamera.SetRoll(0.0f);

	//float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	//m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, &m_StaticCamera, 10.0f, 0.f, "Static");
	////m_pThPSCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_Player, 12.0f, 4.f, "Wolf");
	//m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	//CORE->SetCamera(m_pCamera);

	/*m_FreeCamera.SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_FreeCamera.SetPitch(-D3DX_PI/6);
	m_FreeCamera.SetYaw(0.0f);
	m_FreeCamera.SetRoll(0.0f);

	m_pThPSFreeCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, &m_FreeCamera, 10.0f, 0.f, "Free");
	m_pFreeCamera = static_cast<CCamera*>(m_pThPSFreeCamera);*/

	//m_Player->SetLockCamera( false );

	
	// Creamos la escena de debug. Esto solo si no se tiene escenario
	m_pScene->Init();
	
	// Inicializa el gestor de player y enemigos. Carga propiedades y estados de todo.
	if ( !m_pCharactersManager->Initialize ( ) )
	{
		return false;
	}

	m_Player = m_pCharactersManager->GetPlayer();
	m_Player->SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_Player->SetPitch(-D3DX_PI/8);
	m_Player->SetYaw(0.0f);
	m_Player->SetRoll(0.0f);
	m_Player->Init();

	float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_Player, 12.0f, 4.f, "Wolf");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	// Asignamos la camera activa al core
	CORE->SetCamera( m_pCamera );

	m_FreeCamera.SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_FreeCamera.SetPitch(-D3DX_PI/6);
	m_FreeCamera.SetYaw(0.0f);
	m_FreeCamera.SetRoll(0.0f);

	m_pThPSFreeCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, &m_FreeCamera, 10.0f, 0.f, "Free");
	m_pFreeCamera = static_cast<CCamera*>(m_pThPSFreeCamera);

	m_Player->SetLockCamera( false );

	/*CProperties* l_pPlayerProperties = new CProperties();
	l_pPlayerProperties->SetAnimationInstance( "Caperucita1" );
	l_pPlayerProperties->SetCore( "Caperucita" );
	l_pPlayerProperties->SetLife( 50 );
	l_pPlayerProperties->SetName( "Caperucita" );
	l_pPlayerProperties->SetPosition( Vect3f(0.f,0.f,0.f ) );
	l_pPlayerProperties->SetRespawnPosition( Vect3f(0.f,0.f,0.f ) );
	l_pPlayerProperties->SetSpeed( 10 );

	m_pPlayer = new CPlayer( "Jolete" );
	m_pPlayer->SetProperties ( l_pPlayerProperties );

	m_pPlayer->Init();*/

	bool l_ok = CORE->GetScriptManager()->Load("./Data/XML/script_gameplay.xml");
	CORE->GetScriptManager()->RunCode("init_game_data()");
	//CORE->GetScriptManager()->RunCode("load_enemy()");
	//CORE->GetScriptManager()->RunFile("foo.lua");

	return true;
}

void CGameProcess::Update( float _ElapsedTime )
{
	CORE->SetCamera( m_pCamera );
	
	m_pCharactersManager->Update ( _ElapsedTime );
		
	//m_Player->Update(_ElapsedTime, m_pCamera );
	if( m_Player->GetLockCamera() )
	{
		m_FreeCamera.Update(_ElapsedTime ,m_pCamera);
	}

	CORE->GetRenderableObjectsLayersManager()->Update( _ElapsedTime );

	UpdateInputs( _ElapsedTime );	
}

void CGameProcess::Render(CRenderManager &RM)
{
}

void CGameProcess::UpdateInputs ( float _ElapsedTime )
{
	if( CORE->GetActionToInput()->DoAction("CommutacioCamera") )
	{
		if( m_pCamera == m_pThPSCamera )
		{
			m_pCamera = m_pThPSFreeCamera;
			CORE->SetCamera( m_pThPSFreeCamera );
			m_Player->SetLockCamera( true );
		}
		else
		{
			m_pCamera = m_pThPSCamera;
			CORE->SetCamera( m_pThPSCamera );
			m_Player->SetLockCamera( false );
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

//-------------------------------------
//--Registrador de métodos en LUA------
//-------------------------------------
void CGameProcess::RegisterMethods( void )
{
	lua_State * l_State = SCRIPT->GetLuaState();

	/*RegisterToLuaTelegram(l_State);
	RegisterToLuaMessageDispacher(l_State);*/
	RegisterToLuaCNamed					( l_State );
	RegisterToLuaCObject3D				( l_State );
	RegisterToLuaBaseGameEntity			( l_State );
	RegisterToLuaCharacter				( l_State );
	RegisterToLuaCharacterManager		( l_State );
	RegisterToLuaScriptedStateMachine	( l_State );
	RegisterToLuaGameProcess			( l_State );
	
}

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------

CGameProcess* CGameProcess::GetGameProcess( void )
{
	CEngineProcess *l_pProces = CORE->GetProcess();
	return static_cast<CGameProcess*> ( l_pProces );
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


