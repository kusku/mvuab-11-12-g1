#include "Player.h"
#include "ActionToInput.h"
#include "RenderManager.h"
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

void CPlayer::Update(float elapsedTime)
{
	UpdateInputActions(elapsedTime);	
}

void CPlayer::UpdateInputActions(float elapsedTime)
{
	float d = 0.0f;
	if( CORE->GetActionToInput()->DoAction("MoveXCam", d) )
	{
		m_fYaw += d;
	}

	if( CORE->GetActionToInput()->DoAction("MoveYCam", d) )
	{
		m_fPitch += d;
	}

	if( CORE->GetActionToInput()->DoAction("MovePlayerUp") )
	{
		if( CORE->GetActionToInput()->DoAction("MovePlayerLeft") )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw + D3DX_PI/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw + D3DX_PI/4.f));
			m_Position += m_Dir * 5.f * elapsedTime;
		}
		else if( CORE->GetActionToInput()->DoAction("MovePlayerRight") )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw - D3DX_PI/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw - D3DX_PI/4.f));
			m_Position += m_Dir * 5.f * elapsedTime;
		}
		else
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
			m_Position += m_Dir * 5.f * elapsedTime;
		}
	}
	else if( CORE->GetActionToInput()->DoAction("MovePlayerDown") )
	{
		if( CORE->GetActionToInput()->DoAction("MovePlayerLeft") )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw - D3DX_PI/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw - D3DX_PI/4.f));
			m_Position -= m_Dir * 5.f * elapsedTime;
		}
		else if( CORE->GetActionToInput()->DoAction("MovePlayerRight") )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw + D3DX_PI/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw + D3DX_PI/4.f));
			m_Position -= m_Dir * 5.f * elapsedTime;
		}
		else
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
			m_Position -= m_Dir * 5.f * elapsedTime;
		}
	}
	else
	{
		if( CORE->GetActionToInput()->DoAction("MovePlayerLeft") )
		{
			m_Dir = Vect3f(cosf(m_fYaw + D3DX_PI/2.f), 0.0f, sinf(m_fYaw + D3DX_PI/2.f));
			m_Position += m_Dir * 5.f * elapsedTime;
		}

		if( CORE->GetActionToInput()->DoAction("MovePlayerRight") )
		{
			m_Dir = Vect3f(cosf(m_fYaw + D3DX_PI/2.f), 0.0f, sinf(m_fYaw + D3DX_PI/2.f));
			m_Position -= m_Dir * 5.f * elapsedTime;
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

	RM->DrawCube( Vect3f(0.5f, 0.5f, 0.5f) );
}