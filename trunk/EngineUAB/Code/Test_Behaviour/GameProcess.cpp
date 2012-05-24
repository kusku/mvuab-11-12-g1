#include "GameProcess.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"

#include "Characters\CharacterManager.h"
#include "Characters\Character.h"
#include "StatesMachine\ScriptedStateMachine.h"
#include "DebugSceneBehaviour\Scene.h"

#include "Characters\Player\Player.h"
#include "Characters\Properties\Properties.h"



#include "Core.h"
#include "Base.h"
#include "TestBehaviourDefs.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

CGameProcess::CGameProcess(HWND hWnd)
	: m_hWnd	( hWnd )
	//, m_pPlayer ( NULL )
{
	m_pScene = new CScene();
}

CGameProcess::~CGameProcess(void)
{
}

bool CGameProcess::Init()
{
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

	float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, &m_StaticCamera, 10.0f, 0.f, "Static");
	//m_pThPSCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_Player, 12.0f, 4.f, "Wolf");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);

	/*m_FreeCamera.SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_FreeCamera.SetPitch(-D3DX_PI/6);
	m_FreeCamera.SetYaw(0.0f);
	m_FreeCamera.SetRoll(0.0f);

	m_pThPSFreeCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, &m_FreeCamera, 10.0f, 0.f, "Free");
	m_pFreeCamera = static_cast<CCamera*>(m_pThPSFreeCamera);*/

	//m_Player->SetLockCamera( false );

	
	// Creamos la escena de debug. Esto solo si no se tiene escenario
	m_pScene->Init();

	//m_PM = new CPropertiesManager();

	// Inicializa el gestor de player y enemigos. Carga propiedades y estados de todo.
	/*if ( !m_pCharactersManager->Initialize ( ) )
	{
		return false;
	}*/

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


	return true;
}

void CGameProcess::Update(float elapsedTime)
{
}

void CGameProcess::Render(CRenderManager &RM)
{
}

//-------------------------------------
//--Registrador de métodos en LUA------
//-------------------------------------
void CGameProcess::RegisterMethods()
{
	module(SCRIPT->GetLuaState()) [
		class_<CScriptedStateMachine<CCharacter>>("CScriptedStateMachine")
			.def("ChangeState", &CScriptedStateMachine<CCharacter>::ChangeState)
			.def("CurrentState", &CScriptedStateMachine<CCharacter>::CurrentState)
			.def("SetCurrentState", &CScriptedStateMachine<CCharacter>::SetCurrentState)
	];
}