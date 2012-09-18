#include "WolfIdleState.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
//#include "Characters\Enemies\Wolf\AnimationStates\WolfIdle2AnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfRunAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfIdleState::CWolfIdleState( CCharacter* _pCharacter )
	: CState		(_pCharacter, "CWolfIdleState")
	//, m_ActionTime  ( CActionStateCallback( 1.f, 2.f ) )
	, m_pWolf		( NULL )
{
}

CWolfIdleState::CWolfIdleState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	//, m_ActionTime	( CActionStateCallback( 1.f, 2.f ) )
	, m_pWolf		( NULL )
{
}


CWolfIdleState::~CWolfIdleState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfIdleState::OnEnter( CCharacter* _pCharacter )
{
	m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	//m_ActionTime.StartAction();
}

void CWolfIdleState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	if ( m_pWolf->IsPlayerDetected() ) 
	{
		m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetPursuitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetRunAnimationState());
	}
	else
	{
		// TODO: Esto comentado es para si existe otro idle para cargar
		/*if ( m_ActionTime.IsActionFinished() )
		{*/
			// pillamos la animación idle que deseemos
			//if ( BoostRandomHelper::GetInt(1, 4) == 1 ) 
			//{
				m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
				
				#if defined _DEBUG
					if( CORE->IsDebugMode() )
					{
						std::string l_State = RABBIT_IDLE_STATE;
						CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
					}
				#endif
			//}
			//else
			//{
			//	m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdle2AnimationState());
			//}

			// pillamos un tiempo de ejecución aleatorio
		/*	m_ActionTime.StartAction();
		}
		else
		{
			m_ActionTime.Update(_ElapsedTime);
		}*/
	}

	// Reseteamos la velocidad del enemigo
	_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	_pCharacter->MoveTo2(_pCharacter->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CWolfIdleState::OnExit( CCharacter* _pCharacter )
{
}

bool CWolfIdleState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		}

		m_pWolf->RestLife(1000); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		return true;
	}
	return false;
}