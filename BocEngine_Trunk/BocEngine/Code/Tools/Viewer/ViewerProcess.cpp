#include "ViewerProcess.h"
#include "RenderManager.h"
#include "ActionToInput.h"
#include "Cameras\ThPSCamera.h"
#include "Base.h"
#include "Core.h"
#include "Math\Color.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Math\Matrix44.h"
#include "Cameras\Camera.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "Lights\LightManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CViewerProcess::CViewerProcess()
	: pos(0,0)
	, screen(800,600)
	, yaw(0.0f)
	, m_pThPSCamera(NULL)	
{
}

CViewerProcess::~CViewerProcess()
{
	CHECKED_DELETE( m_pThPSCamera );
	m_Camera = NULL;
}

void CViewerProcess::Init()
{
	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	m_Player.SetPosition(Vect3f(0.f,1.f,0.f));
	m_Player.SetPitch(0.0f);
	m_Player.SetYaw(0.0f);
	m_Player.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 100.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 10.0f);
	m_Camera = static_cast<CCamera*>(m_pThPSCamera);
}

void CViewerProcess::Update(float elapsedTime)
{
	m_Player.Update(elapsedTime, m_Camera);
	UpdateInputs(elapsedTime);

	CORE->GetRenderableObjectsManager()->Update(elapsedTime);
}

void CViewerProcess::UpdateInputs(float elapsedTime)
{
	CActionToInput *action2Input = CORE->GetActionToInput();
	if( action2Input->DoAction("ReloadTTFs") )
	{
		CORE->ReloadTTFs();
	}

	if( action2Input->DoAction("ReloadLanguageXMLs") )
	{
		CORE->ReloadLanguages();
	}

	if( action2Input->DoAction("ReloadActions") )
	{
		CORE->ReloadInputs();
	}

	if( action2Input->DoAction("ReloadRenderableObjects") )
	{
		CORE->ReloadRenderableObjects();
	}
}

void CViewerProcess::Render(CRenderManager *RM)
{
	Mat44f mat;
	mat.SetIdentity();
	RM->SetTransform(mat);
	RM->DrawAxis(1.0f);
	RM->DrawGrid(100.0f, 100.0f, 30, colBLACK);

	m_Player.Render(RM);

	CEffectTechnique* tech = CORE->GetEffectManager()->GetEffectTechnique("technique0");
	
	CEffect* effect = tech->GetEffect();
	CORE->GetRenderableObjectsManager()->Render(RM, tech);

	//CORE->GetLightManager()->Render(RM);
}