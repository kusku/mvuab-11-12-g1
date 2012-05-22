#include "GameProcess.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"

#include "Characters\CharacterManager.h"
#include "Characters\Character.h"
#include "StatesMachine\ScriptedStateMachine.h"


#include "Core.h"
#include "Base.h"
#include "TestBehaviourDefs.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

CGameProcess::CGameProcess(HWND hWnd)
	: m_hWnd(hWnd)
{
}

CGameProcess::~CGameProcess(void)
{
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

	if( INIT_GUI )
	{
		CORE->GetScriptManager()->RunCode("load_data()");
	}
	else
	{
		CORE->GetScriptManager()->RunCode("load_all()");
	}

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