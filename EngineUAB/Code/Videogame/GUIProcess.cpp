#include "GUIProcess.h"
#include "XML\XMLControl.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"
#include "SoundManager.h"
#include "Listener.h"
#include "Cameras\FPSCamera.h"
#include "Cameras\Camera.h"
#include "Math\Vector2.h"
#include "Options\Options.h"
#include "Core.h"
#include "Base.h"
#include "VideogameDefs.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CGUIProcess::CGUIProcess(HWND hWnd)
	: m_hWnd(hWnd)
	, m_pFPSCamera(NULL)
{
}

CGUIProcess::~CGUIProcess()
{
	Done();
}

void CGUIProcess::Release()
{
	CHECKED_DELETE( m_pFPSCamera );
	m_pCamera = NULL;
}

bool CGUIProcess::Init()
{
	Vect2i pos;
	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	//Establece la cámara
	/*m_StaticCamera.SetPosition(Vect3f(0.f,1.f,0.f));
	m_StaticCamera.SetPitch(0.0f);
	m_StaticCamera.SetYaw(0.0f);
	m_StaticCamera.SetRoll(0.0f);*/

	m_StaticCamera.SetPosition(Vect3f(130.4898f, -3.45527f, -160.052f));
	m_StaticCamera.SetPitch(3.5111432f);
	m_StaticCamera.SetYaw(5.8668766f);
	m_StaticCamera.SetRoll(FLOAT_PI_VALUE);
	m_StaticCamera.SetScale(v3fUNIT);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pFPSCamera = new CFPSCamera(1.f, 10000.f, 45.f * D3DX_PI / 180.f, aspect,  &m_StaticCamera);
	m_pCamera = static_cast<CCamera*>(m_pFPSCamera);
	CORE->SetCamera(m_pCamera);

	//Registra los métodos de él mismo
	//RegisterMethods();

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

void CGUIProcess::SetCameraToCore()
{
	CORE->SetCamera(m_pCamera);
}

void CGUIProcess::SetCameraToAudio()
{
	CORE->GetSoundManager()->GetListener()->SetCamera(m_pCamera);
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
	PostMessage(m_hWnd, WM_EXIT_GAME, 0, 0);
}

void CGUIProcess::LoadGameData()
{
	//m_pThread = new boost::thread(&CGUIProcess::ChangeProcess, this);
	ChangeProcess();
}

void CGUIProcess::Reload( void )
{
}


void CGUIProcess::PreReload( void )
{
	CORE->GetSoundManager()->StopAllEvents();
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
			.def("load_game_data", &CGUIProcess::LoadGameData)
	];
}
