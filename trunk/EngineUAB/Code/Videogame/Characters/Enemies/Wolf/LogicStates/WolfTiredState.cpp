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
	, m_ActionTime	( 1.f, 2.f )
	, m_pWolf		( NULL )
	, m_MinTime		( 1.f )
	, m_MaxTime		( 2.f )
{
}

CWolfTiredState::CWolfTiredState(  CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_ActionTime	( 1.f, 2.f )
	, m_pWolf		( NULL )
	, m_MinTime		( 1.f )
	, m_MaxTime		( 2.f )
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

	m_ActionTime.InitAction(m_MinTime, m_MaxTime);
	m_ActionTime.StartAction();
	int l_Valor = BoostRandomHelper::GetInt(1, 3);
	// Me gusta darle doble opción al idle 2... 
	/*if ( l_Valor == 1 || l_Valor == 2 ) 
	{
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdle2AnimationState());
	}
	else
	{*/
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
	//}

	//LOGGER->AddNewLog(ELL_INFORMATION, "Valor : %d", l_Valor);
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
	
	//LOGGER->AddNewLog(ELL_INFORMATION, "CWolfTiredState::Execute-> %s Cansado durante %f segons", m_pWolf->GetName().c_str(), m_MaxTime );
	
	if ( m_ActionTime.IsActionFinished() ) 
	{
		// restauramos la variable que determina si está cansado
		m_pWolf->SetHitsDone(0);
		m_pWolf->GetLogicFSM()->RevertToPreviousState();
		//m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
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
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	return m_pWolf->CallHitState(_pCharacter, _Telegram);
}
