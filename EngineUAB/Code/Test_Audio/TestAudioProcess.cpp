#include "TestAudioProcess.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Commands\SceneRendererCommandManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "ActionToInput.h"
#include "Scripting\ScriptManager.h"
#include "Wwise\WwiseSoundManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CTestAudioProcess::CTestAudioProcess( void )
	: m_pThPSCamera(NULL)
	, pos(0,0)
	, screen(800,600)
{
}

CTestAudioProcess::~CTestAudioProcess( void )
{
	Release();
}

void CTestAudioProcess::Done ( void )
{
}

void CTestAudioProcess::Release ( void )
{
	m_pWwiseSoundManager->Terminate();
	
	CHECKED_DELETE(m_pWwiseSoundManager);
	CHECKED_DELETE( m_pThPSCamera );
	m_pCamera = NULL;	
}

bool CTestAudioProcess::Init( void )
{
	CORE->GetScriptManager()->RunCode("load_basics()");
	CORE->GetScriptManager()->RunCode("load_data()");

	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	m_Player.SetPosition(Vect3f(0.f,1.f,0.f));
	m_Player.SetPitch(0.0f);
	m_Player.SetYaw(0.0f);
	m_Player.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 10.0f);
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);

	m_pWwiseSoundManager = new CWwiseSoundManager();
	m_pWwiseSoundManager->Init();
	m_pWwiseSoundManager->Load("./data/xml/soundbanks.xml", "./data/xml/speakers.xml");

	m_pWwiseSoundManager->SetSwitch("Ground_Materials", "Wood");
	m_pWwiseSoundManager->PlayEvent("Play_Ground");

	return true;
}

void CTestAudioProcess::Update( float _ElapsedTime )
{
	CORE->SetCamera( m_pCamera );

	UpdateInputs();

	m_Player.Update( _ElapsedTime, m_pCamera );

	CORE->GetRenderableObjectsLayersManager()->Update( _ElapsedTime );

	m_pWwiseSoundManager->Update();
}

void CTestAudioProcess::Render( CRenderManager &_RM )
{
}

void CTestAudioProcess::UpdateInputs()
{
	if( CORE->GetActionToInput()->DoAction("PlaySound") )
	{
		m_pWwiseSoundManager->PlayEvent("Play_Explosions");
	}

	if( CORE->GetActionToInput()->DoAction("PlayGravel") )
	{
		m_pWwiseSoundManager->SetSwitch("Ground_Materials", "Gravel");
		m_pWwiseSoundManager->PlayEvent("Play_Ground");
	}

	if( CORE->GetActionToInput()->DoAction("PlayWood") )
	{
		m_pWwiseSoundManager->SetSwitch("Ground_Materials", "Wood");
		m_pWwiseSoundManager->PlayEvent("Play_Ground");
	}
}

void CTestAudioProcess::RegisterMethods()
{
}