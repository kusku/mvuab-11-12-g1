#include "DeerTiredState.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Enemies\Deer\Deer.h"

#include "DeerPursuitState.h"
#include "DeerHitState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerTiredState::CDeerTiredState( CCharacter* _pCharacter )
	: CState		(_pCharacter, "CDeerTiredState")
	, m_ActionTime	( 1.5f, 2.f )
	, m_pDeer		( NULL )
	, m_MinTime		( 1.f )
	, m_MaxTime		( 2.f )
{
}

CDeerTiredState::CDeerTiredState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_ActionTime	( 1.f, 2.f )
	, m_pDeer		( NULL )
	, m_MinTime		( 1.f )
	, m_MaxTime		( 2.f )
{
}


CDeerTiredState::~CDeerTiredState(void)
{
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerTiredState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	m_ActionTime.InitAction(m_MinTime, m_MaxTime);
	m_ActionTime.StartAction();
	int l_Valor = BoostRandomHelper::GetInt(1, 3);
	// Me gusta darle doble opción al idle 2... 
	/*if ( l_Valor == 1 || l_Valor == 2 ) 
	{
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdle2AnimationState());
	}
	else
	{*/
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
	//}

	//LOGGER->AddNewLog(ELL_INFORMATION, "Valor : %d", l_Valor);
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = "Fatigued";
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}


void CDeerTiredState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}
	
	//LOGGER->AddNewLog(ELL_INFORMATION, "CDeerTiredState::Execute-> %s Cansado durante %f segons", m_pDeer->GetName().c_str(), m_MaxTime );
	
	if ( m_ActionTime.IsActionFinished() ) 
	{
		// restauramos la variable que determina si está cansado
		m_pDeer->SetHitsDone(0);
		m_pDeer->GetLogicFSM()->RevertToPreviousState();
		//m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
	}

	m_ActionTime.Update(_ElapsedTime);

	// Mentre espero miro al player
	m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime);
}


void CDeerTiredState::OnExit( CCharacter* _pCharacter )
{
}

bool CDeerTiredState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	// Si venimos del hit volveremos a el i haremos el doble hit
	if ( m_pDeer->GetLogicFSM()->GetPreviousState()->GetName() == "CRabbitHitState" )
	{
		m_pDeer->GetHitState()->SetDoubleHit(true);
	}	
	return m_pDeer->CallHitState(m_pDeer, _Telegram);
}
