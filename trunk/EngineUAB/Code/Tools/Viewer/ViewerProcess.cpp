#include "ViewerProcess.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Periphericals\Mouse.h"
#include "Base.h"
#include "Core.h"
#include "Math\Color.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Math\Matrix44.h"
#include "Cameras\Camera.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CViewerProcess::CViewerProcess()
	: pos(0,0)
	, screen(800,600)
	, m_RotTerra(0.0f)
	, m_RotTMateixa(0.0f)
	, m_RotLluna(0.0f)
	, yaw(0.0f)
	, m_pThPSCamera(NULL)
	, m_pFPSCamera(NULL)
{
}

CViewerProcess::~CViewerProcess()
{
	CHECKED_DELETE( m_pThPSCamera );
	CHECKED_DELETE( m_pFPSCamera );
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
	m_pFPSCamera = new CFPSCamera(1.0f, 100.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player);
	m_Camera = m_pThPSCamera;

}

void CViewerProcess::Update(float elapsedTime)
{
	//m_RotTMateixa = m_RotTMateixa + 1.5f * elapsedTime;
	//m_RotTerra = m_RotTerra + 0.2f * elapsedTime;
	//m_RotLluna = m_RotLluna + 3.5f * elapsedTime;
	if( CORE->GetActionToInput()->DoAction("AssignFPSCamera") )
	{
		m_Camera = m_pFPSCamera;
	}
	else if( CORE->GetActionToInput()->DoAction("AssignThPSCamera") )
	{
		m_Camera = m_pThPSCamera;
	}

	m_Player.Update(elapsedTime);
}

void CViewerProcess::Render(CRenderManager *RM)
{
	Mat44f mat;
	mat.SetIdentity();
	RM->SetTransform(mat);
	RM->DrawAxis(1.0f);
	RM->DrawGrid(100.0f, 100.0f, 30, colBLACK);
	m_Player.Render(RM);
	/*Mat44f trans, trans2, rot, rot2;
	Mat44f matMovement;

	RM->DrawAxis(2.0f);
	RM->DrawSphere(1.f, 15, colYELLOW);

	mat.SetIdentity();
	matMovement .SetIdentity();
	rot.SetIdentity();
	rot2.SetIdentity();
	trans.SetIdentity();
	rot2.RotByAngleY(m_RotTMateixa);
	rot.RotByAngleY(m_RotTerra);
	trans.Translate(Vect3f(3.5f, 0.0f, 0.0f));
	matMovement = rot * trans;
	mat = matMovement * rot2;
	RM->SetTransform(mat);
	RM->DrawSphere(1.f, 15, colCYAN);

	mat.SetIdentity();
	rot2.SetIdentity();
	trans2.SetIdentity();
	trans2.Translate(Vect3f(2.0f, 0.0f, 0.0f));
	rot2.RotByAngleY(m_RotLluna);
	matMovement = matMovement * rot2 * trans2;
	RM->SetTransform(matMovement);
	RM->DrawSphere(0.2f, 10, colBLACK);*/
}
