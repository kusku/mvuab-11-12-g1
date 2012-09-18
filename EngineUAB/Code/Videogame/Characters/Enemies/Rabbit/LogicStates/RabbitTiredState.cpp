#include "RabbitTiredState.h"
#include "RabbitPursuitState.h"
#include "RabbitHitState.h"

#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdleAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitIdle2AnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\RabbitHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitTiredState::CRabbitTiredState( CCharacter* _pCharacter )
	: CState		(_pCharacter, "CRabbitTiredState")
	, m_ActionTime	( 1.5f, 2.f )
	, m_MinTime		( 1.f )
	, m_MaxTime		( 2.f )
	, m_pRabbit		( NULL )
{
}

CRabbitTiredState::CRabbitTiredState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_ActionTime	( 1.5f, 2.f )
	, m_MinTime		( 1.f )
	, m_MaxTime		( 2.f )
	, m_pRabbit		( NULL )
{
}


CRabbitTiredState::~CRabbitTiredState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitTiredState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	m_ActionTime.InitAction(m_MinTime, m_MaxTime);
	m_ActionTime.StartAction();
	int l_Valor = BoostRandomHelper::GetInt(1, 3);
	// Me gusta darle doble opción al idle 2... 
	/*if ( l_Valor == 1 || l_Valor == 2 ) 
	{
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdle2AnimationState());
	}
	else
	{*/
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetIdleAnimationState());
	//}

	//LOGGER->AddNewLog(ELL_INFORMATION, "Valor : %d", l_Valor);
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Fatigued";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pRabbit->GetName().c_str(), l_State );
		}
	#endif
}


void CRabbitTiredState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pRabbit) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}
	
	if ( m_ActionTime.IsActionFinished() ) 
	{
		// restauramos la variable que determina si está cansado
		m_pRabbit->SetHitsDone(0);
		m_pRabbit->GetLogicFSM()->RevertToPreviousState();
	}

	m_ActionTime.Update(_ElapsedTime);

	// Mentre espero miro al player
	m_pRabbit->FaceTo( m_pRabbit->GetPlayer()->GetPosition(), _ElapsedTime);
}


void CRabbitTiredState::OnExit( CCharacter* _pCharacter )
{
}

bool CRabbitTiredState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pRabbit) 
		{
			m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
		}

		m_pRabbit->RestLife(50); 
		m_pRabbit->GetLogicFSM()->ChangeState(m_pRabbit->GetHitState());
		m_pRabbit->GetGraphicFSM()->ChangeState(m_pRabbit->GetHitAnimationState());
		return true;
	}

	return false;
}
