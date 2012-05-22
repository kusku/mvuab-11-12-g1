#include "GUIProcess.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "Math\Vector2.h"
#include "Core.h"
#include "Base.h"
#include "VideogameDefs.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CGUIProcess::CGUIProcess(HWND hWnd)
	: m_hWnd(hWnd)
	, m_pThPSCamera(NULL)
{
}

CGUIProcess::~CGUIProcess()
{
	Done();
}

void CGUIProcess::Release()
{
	CHECKED_DELETE( m_pThPSCamera );
	m_pCamera = NULL;
}

bool CGUIProcess::Init()
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

	//Registra los métodos de él mismo
	RegisterMethods();

	//Carga los datos básicos
	CORE->GetScriptManager()->RunCode("load_basics()");

	return true;
}

void CGUIProcess::Update(float elapsedTime)
{
}

void CGUIProcess::Render(CRenderManager &RM)
{
}

//-------------------------------------
//--Métodos registrados en LUA---------
//-------------------------------------
void CGUIProcess::ChangeProcess()
{
	PostMessage(m_hWnd, WM_GAME_PROCESS, 0, 0);
}

void CGUIProcess::ExitGame()
{
	PostMessage(m_hWnd, WM_KEYDOWN, (WPARAM)VK_ESCAPE, 0);
}

//-------------------------------------
//--Registrador de métodos en LUA------
//-------------------------------------
void CGUIProcess::RegisterMethods()
{
	lua_State *state = CORE->GetScriptManager()->GetLuaState();

	module(state) [
		class_<CGUIProcess, CEngineProcess>("CGUIProcess")
			.def("change_process", &CGUIProcess::ChangeProcess)
			.def("exit_game", &CGUIProcess::ExitGame)
	];
}