#include "Player.h"
#include "ActionToInput.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"

CPlayer::CPlayer()
	: m_Dir(0.0f, 0.0f, 0.0f)
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Update(float elapsedTime, CCamera *camera)
{
	UpdateInputActions(elapsedTime, camera);	
}

void CPlayer::UpdateInputActions(float elapsedTime, CCamera *camera)
{
	float d = 0.0f;
	CActionToInput *action2Input = CORE->GetActionToInput();
	CThPSCamera* l_ThPSCamera = static_cast<CThPSCamera*>(camera);
	if( action2Input->DoAction("YawViewerCam", d) )
	{
		m_fYaw += d;
		if( m_fYaw > e2PIf )
		{
			m_fYaw -= e2PIf;
		}
		else if( m_fYaw < -e2PIf )
		{
			m_fYaw += e2PIf;
		}
	}

	if( action2Input->DoAction("PitchViewerCam", d) )
	{
		float l_Pitch = m_fPitch + d;
		if( abs(l_Pitch) < ePIf/2 )
			m_fPitch = l_Pitch;
	}

	/*if( action2Input->DoAction("MoveZViewerCam", d) )
	{
		m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
		if( d > 0 )
		{
			m_Position -= m_Dir * 50.f * elapsedTime;
		}
		else
		{
			m_Position += m_Dir * 50.f * elapsedTime;
		}
	}

	if( action2Input->DoAction("SlowZoomViewerCam", d) )
	{
		float zoom = l_ThPSCamera->GetZoom() + d;
		l_ThPSCamera->SetZoom(zoom);
	}
	else if( action2Input->DoAction("ZoomViewerCam", d) )
	{
		float zoom = l_ThPSCamera->GetZoom() + d;
		l_ThPSCamera->SetZoom(zoom);
	}*/


	if( action2Input->DoAction("MovePlayerUp") )
	{
		if( action2Input->DoAction("MovePlayerLeft") )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw + ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw + ePIf/4.f));
			m_Position += m_Dir * 15.f * elapsedTime;
		}
		else if( action2Input->DoAction("MovePlayerRight") )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw - ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw - ePIf/4.f));
			m_Position += m_Dir * 15.f * elapsedTime;
		}
		else
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
			m_Position += m_Dir * 15.f * elapsedTime;
		}
	}
	else if( action2Input->DoAction("MovePlayerDown") )
	{
		if( action2Input->DoAction("MovePlayerLeft") )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw - ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw - ePIf/4.f));
			m_Position -= m_Dir * 15.f * elapsedTime;
		}
		else if( action2Input->DoAction("MovePlayerRight") )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw + ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw + ePIf/4.f));
			m_Position -= m_Dir * 15.f * elapsedTime;
		}
		else
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
			m_Position -= m_Dir * 15.f * elapsedTime;
		}
	}
	else
	{
		if( action2Input->DoAction("MovePlayerLeft") )
		{
			m_Dir = Vect3f(cosf(m_fYaw + ePIf/2.f), 0.0f, sinf(m_fYaw + ePIf/2.f));
			m_Position += m_Dir * 15.f * elapsedTime;
		}

		if( action2Input->DoAction("MovePlayerRight") )
		{
			m_Dir = Vect3f(cosf(m_fYaw + ePIf/2.f), 0.0f, sinf(m_fYaw + ePIf/2.f));
			m_Position -= m_Dir * 15.f * elapsedTime;
		}
	}
}

void CPlayer::Render(CRenderManager *RM)
{
	Mat44f mat, trans, rot, rotPitch;
	mat.SetIdentity();
	trans.SetIdentity();
	rot.SetIdentity();
	rotPitch.SetIdentity();
	trans.Translate(m_Position);
	rot.SetRotByAngleY(-m_fYaw);
	rotPitch.SetRotByAngleZ(m_fPitch);

	mat = trans * rot * rotPitch;
	RM->SetTransform(mat);

	RM->DrawSphere(0.5f, 7, colWHITE);
}