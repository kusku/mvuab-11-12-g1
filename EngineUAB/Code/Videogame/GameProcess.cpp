#include "GameProcess.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"
#include "ActionToInput.h"
#include "Core.h"
#include "Base.h"

#include "VideogameDefs.h"
#include "Characters\CharacterManager.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

CGameProcess::CGameProcess(HWND hWnd)
	: m_hWnd(hWnd)
	, m_pThPSCamera(NULL)
	, m_pCharacterManager(NULL)
{
}

CGameProcess::~CGameProcess(void)
{
	CHECKED_DELETE(m_pCharacterManager);
}

bool CGameProcess::Init()
{
	//Registra los métodos
	RegisterMethods();

	//Crea los datos para el gameplay
	m_pCharacterManager = new CCharactersManager();

	//Carga los datos en el engine
	if( INIT_GUI )
	{
		CORE->GetScriptManager()->RunCode("load_data()");
	}
	else
	{
		CORE->GetScriptManager()->RunCode("load_all()");
	}

	//Carga los scripts del juego
	CORE->GetScriptManager()->Load("./Data/XML/script_gameplay.xml");
	CORE->GetScriptManager()->RunCode("init_game_data()");

	//Crea la cámara
	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, m_pCharacterManager->GetPlayer(), 12.0f, 4.f, "Caperucita");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);

	return true;
}

void CGameProcess::Update(float elapsedTime)
{
	//Vuelve a cargar los datos si hacemos el reload de LUA
	if( CORE->GetActionToInput()->DoAction("ReloadScripts") )
	{
		CORE->GetScriptManager()->RunCode("init_game_data()");
		m_pThPSCamera->SetObject3D( m_pCharacterManager->GetPlayer());
	}

	m_pCharacterManager->Update(elapsedTime);
}

void CGameProcess::Render(CRenderManager &RM)
{
}

CGameProcess* CGameProcess::GetGameProcess()
{
	CEngineProcess *proces = CORE->GetProcess();
	return static_cast<CGameProcess*>(proces);
}

//-------------------------------------
//--Registrador de métodos en LUA------
//-------------------------------------
void CGameProcess::RegisterMethods()
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		def("get_game_process", GetGameProcess)
	];

	module(state) [
		class_<CGameProcess>("CGameProcess")
			.def("get_character_manager", &CGameProcess::GetCharacterManager)
	];

	CCharactersManager::RegisterMethods();
}