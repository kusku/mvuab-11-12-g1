#include "FreeCamera.h"
#include "RenderManager.h"
#include "ActionToInput.h"
#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Core.h"
#include "Base.h"
#include "Characters\PlayerDef.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CFreeCamera::CFreeCamera()
	: m_Dir(0.0f, 0.0f, 0.0f)
{
}

CFreeCamera::~CFreeCamera()
{
}

void CFreeCamera::Update(float _ElapsedTime, CCamera *camera)
{
	float d = 0.0f;
	CActionToInput *action2Input = CORE->GetActionToInput();
	CThPSCamera* l_ThPSCamera = static_cast<CThPSCamera*>(camera);
	if ( action2Input->DoAction("YawViewerCam", d) )
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

	if( action2Input->DoAction( "MovePlayerUp" ) )
	{
		if( action2Input->DoAction( "MovePlayerLeft" ) )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw + ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw + ePIf/4.f));
			m_Position += m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
		else if( action2Input->DoAction( "MovePlayerRight" ) )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw - ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw - ePIf/4.f));
			m_Position += m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
		else
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
			m_Position += m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}

		if ( action2Input->DoAction( "MoveCameraUp" ) )
		{
			m_Dir = Vect3f ( 0 , 1, 0 );
			m_Position -= m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
		else if( action2Input->DoAction( "MoveCameraDown" ) )
		{
			m_Dir = Vect3f ( 0, 1, 0 );
			m_Position += m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}

	}
	else if( action2Input->DoAction( "MovePlayerDown" ) )
	{
		if( action2Input->DoAction( "MovePlayerLeft" ) )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw - ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw - ePIf/4.f));
			m_Position -= m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
		else if( action2Input->DoAction( "MovePlayerRight" ) )
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw + ePIf/4.f), m_fPitch, mathUtils::Sin<float>(m_fYaw + ePIf/4.f));
			m_Position -= m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
		else
		{
			m_Dir = Vect3f(mathUtils::Cos<float>(m_fYaw), m_fPitch, mathUtils::Sin<float>(m_fYaw));
			m_Position -= m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}

		if ( action2Input->DoAction( "MoveCameraUp" ) )
		{
			m_Dir = Vect3f ( 0 , 1, 0 );
			m_Position -= m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
		else if( action2Input->DoAction( "MoveCameraDown" ) )
		{
			m_Dir = Vect3f ( 0, 1, 0 );
			m_Position += m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
	}
	else
	{
		if( action2Input->DoAction( "MovePlayerLeft" ) )
		{
			m_Dir = Vect3f(cosf(m_fYaw + ePIf/2.f), 0.0f, sinf(m_fYaw + ePIf/2.f));
			m_Position += m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}

		if( action2Input->DoAction( "MovePlayerRight" ) )
		{
			m_Dir = Vect3f(cosf(m_fYaw + ePIf/2.f), 0.0f, sinf(m_fYaw + ePIf/2.f));
			m_Position -= m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
		
		if ( action2Input->DoAction( "MoveCameraUp") )
		{
			m_Dir = Vect3f ( 0 , 1, 0 );
			m_Position -= m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
		
		if( action2Input->DoAction( "MoveCameraDown" ) )
		{
			m_Dir = Vect3f ( 0, 1, 0 );
			m_Position += m_Dir * VELOCITY_FREE_CAMERA * _ElapsedTime;
		}
	}
}

void CFreeCamera::Render(CRenderManager *_RM)
{
}