#include "WolfTiredState.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfPursuitState.h"
#include "WolfHitState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfTiredState::CWolfTiredState(  CCharacter* _pCharacter )
	: CState		( _pCharacter, "CWolfTiredState")
	, m_ActionTime	( CActionStateCallback( 0.2f, 0.5f ) )
	, m_pWolf		( NULL )
{
}

CWolfTiredState::CWolfTiredState(  CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_ActionTime	( CActionStateCallback( 0.2f, 0.5f ) )
	, m_pWolf		( NULL )
{
}


CWolfTiredState::~CWolfTiredState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfTiredState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}

	m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
	m_ActionTime.StartAction();

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Fatigued";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfTiredState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	if ( m_ActionTime.IsActionFinished() ) 
	{
		// restauramos la variable que determina si está cansado
		m_pWolf->SetHitsDone(0);
		m_pWolf->GetLogicFSM()->RevertToPreviousState();
	}

	m_ActionTime.Update(_ElapsedTime);

	// Mentre espero miro al player
	m_pWolf->FaceTo( m_pWolf->GetPlayer()->GetPosition(), _ElapsedTime);
}


void CWolfTiredState::OnExit( CCharacter* _pCharacter )
{
}

bool CWolfTiredState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
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
