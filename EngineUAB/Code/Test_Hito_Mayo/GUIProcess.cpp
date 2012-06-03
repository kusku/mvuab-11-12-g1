#include "GUIProcess.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"
#include "SoundManager.h"
#include "ActionToInput.h"
#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Base.h"
#include "Core.h"
#include "TestDef.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CGUIProcess::CGUIProcess(HWND hWnd)
	: m_hWnd(hWnd)
	, pos(Vect2i(0,0))
	, screen(Vect2i(800,600))
	, m_IsFirstSound(true)
{
}

CGUIProcess::~CGUIProcess(void)
{
	CHECKED_DELETE( m_pThPSCamera );
	m_pCamera = NULL;
}

bool CGUIProcess::Init()
{
	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	m_StaticCamera.SetPosition(Vect3f(0.f,1.f,0.f));
	m_StaticCamera.SetPitch(0.0f);
	m_StaticCamera.SetYaw(0.0f);
	m_StaticCamera.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_StaticCamera, 10.0f, 0.f, 0.f, "Static");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);

	RegisterMethods();

	CORE->GetScriptManager()->RunCode("load_basics()");

	return true;
}

void CGUIProcess::Update(float _ElapsedTime)
{
	if( m_IsFirstSound )
	{
		uint32 id = CORE->GetSoundManager()->CreateSource();
		CORE->GetSoundManager()->PlaySource3D(id, "initGame", true);
		m_IsFirstSound = false;
	}

	if( CORE->GetActionToInput()->DoAction("ChangeRenderCommands") )
	{
		CORE->GetScriptManager()->RunCode("change_to_game_process()");
		CORE->SetGameMode( !CORE->IsGameMode() );
		/*PostMessage(m_hWnd, WM_GAME_PROCESS, 0, 0);*/
	}
}

void CGUIProcess::Render(CRenderManager &_RM)
{
}

void CGUIProcess::ChangeProcess()
{
	PostMessage(m_hWnd, WM_GAME_PROCESS, 0, 0);
}

void CGUIProcess::ExitGame()
{
	PostMessage(m_hWnd, WM_KEYDOWN, (WPARAM)VK_ESCAPE, 0);
}

void CGUIProcess::RegisterMethods()
{
	lua_State *state = CORE->GetScriptManager()->GetLuaState();

	module(state) [
		class_<CGUIProcess, CEngineProcess>("CGUIProcess")
			.def("change_process", &CGUIProcess::ChangeProcess)
			.def("exit_game", &CGUIProcess::ExitGame)
	];
}