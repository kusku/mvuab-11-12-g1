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
	Vect2i pos;
	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	//Establece la cámara
	m_StaticCamera.SetPosition(Vect3f(0.f,1.f,0.f));
	m_StaticCamera.SetPitch(0.0f);
	m_StaticCamera.SetYaw(0.0f);
	m_StaticCamera.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_StaticCamera, 10.0f, 0.f, "Static");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);

	//Registra los métodos
	RegisterMethods();
	CCharactersManager::RegisterMethods();

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

	CORE->GetScriptManager()->Load("./Data/XML/script_gameplay.xml");
	CORE->GetScriptManager()->RunCode("init_game_data()");

	return true;
}

void CGameProcess::Update(float elapsedTime)
{
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
}