#include "Rabbit.h"
#include "StatesMachine\StateMachine.h"
#include "StatesMachine\State.h"
#include "Characters\Character.h"

#include "States\RabbitIdleState.h"
#include "States\RabbitPursuitState.h"
#include "States\RabbitPreparedToAttackState.h"

#include "States\RabbitIdleAnimationState.h"
#include "States\RabbitIdle2AnimationState.h"
#include "States\RabbitRunAnimationState.h"
#include "States\RabbitDeathAnimationState.h"
#include "States\RabbitDefenseAnimationState.h"
#include "States\RabbitHitAnimationState.h"
#include "States\RabbitJumpAnimationState.h"
#include "States\RabbitLeftLateralAnimationState.h"
#include "States\RabbitRightLateralAnimationState.h"
#include "States\RabbitStillAttackAnimationState.h"
#include "States\RabbitRunAttackAnimationState.h"
#include "States\RabbitWalkAnimationState.h"


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbit::CRabbit( int _Id )
	: m_HitsDone					( 0 )
	, m_ReceivedHits_X_Minut		( 0 )
	, m_TotalReceivedHits_X_Minut	( 3 )
	, m_pIdleState					( NULL )
	, m_pPursuitState				( NULL )
	, m_pPreparedToAttackState		( NULL )
	, m_pAnimationIdleState 		( NULL )
	, m_pAnimationIdle2State 		( NULL )
	, m_pAnimationRunState 			( NULL )
	, m_pAnimationDeathState 		( NULL )
	, m_pAnimationDefenseState 		( NULL )
	, m_pAnimationHitState 			( NULL )
	, m_pAnimationJumpState 		( NULL )
	, m_pAnimationLeftLateralState 	( NULL )
	, m_pAnimationRightLateralState ( NULL )
	, m_pAnimationRunAttackState 	( NULL )
	, m_pAnimationStillAttackState 	( NULL )
	, m_pAnimationWalkState 		( NULL )
{
	LoadGraphicStates();
	LoadLogicStates();
}

CRabbit::CRabbit( int _Id, std::string _Name )
	: m_HitsDone					( 0 )
	, m_ReceivedHits_X_Minut		( 0 )
	, m_TotalReceivedHits_X_Minut	( 3 )
	, m_pIdleState					( NULL )
	, m_pPursuitState				( NULL )
	, m_pPreparedToAttackState		( NULL )
	, m_pAnimationIdleState 		( NULL )
	, m_pAnimationIdle2State 		( NULL )
	, m_pAnimationRunState 			( NULL )
	, m_pAnimationDeathState 		( NULL )
	, m_pAnimationDefenseState 		( NULL )
	, m_pAnimationHitState 			( NULL )
	, m_pAnimationJumpState 		( NULL )
	, m_pAnimationLeftLateralState 	( NULL )
	, m_pAnimationRightLateralState ( NULL )
	, m_pAnimationRunAttackState 	( NULL )
	, m_pAnimationStillAttackState 	( NULL )
	, m_pAnimationWalkState 		( NULL )
{
	LoadGraphicStates();
	LoadLogicStates();
}


CRabbit::~CRabbit(void)
{
	CHECKED_DELETE ( m_pIdleState );
	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pPreparedToAttackState );

	// Estados animados
	CHECKED_DELETE ( m_pAnimationIdleState );
	CHECKED_DELETE ( m_pAnimationIdle2State );
	CHECKED_DELETE ( m_pAnimationRunState );
	CHECKED_DELETE ( m_pAnimationDeathState );
	CHECKED_DELETE ( m_pAnimationHitState );
	CHECKED_DELETE ( m_pAnimationDefenseState );
	CHECKED_DELETE ( m_pAnimationJumpState );
	CHECKED_DELETE ( m_pAnimationLeftLateralState );
	CHECKED_DELETE ( m_pAnimationRightLateralState );
	CHECKED_DELETE ( m_pAnimationRunAttackState );
	CHECKED_DELETE ( m_pAnimationStillAttackState );
	CHECKED_DELETE ( m_pAnimationWalkState );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
bool CRabbit::Init( void )
{
	bool l_IsOk = false;

	//CState<CCharacter>* l = dynamic_cast<CState<CCharacter>> (m_pAnimationIdleState);

	// Coloco los estados iniciales
	/*this->GetGraphicFSM()->SetCurrentState( m_pAnimationIdleState );
	this->GetLogicFSM()->SetCurrentState(m_pIdleState);*/
	this->GetGraphicFSM()->ChangeState( m_pAnimationIdleState );
	this->GetLogicFSM()->ChangeState(m_pIdleState);

	this->MoveTo2( Vect3f(0,0,0), 0 );

	return true;
}

void CRabbit::LoadGraphicStates( void )
{
	m_pAnimationIdleState			= new CRabbitIdleAnimationState();
	m_pAnimationIdle2State			= new CRabbitIdle2AnimationState();
	m_pAnimationRunState			= new CRabbitRunAnimationState();
	m_pAnimationDeathState			= new CRabbitDeathAnimationState();
	m_pAnimationHitState			= new CRabbitHitAnimationState();
	m_pAnimationDefenseState		= new CRabbitDefenseAnimationState();
	m_pAnimationJumpState			= new CRabbitJumpAnimationState();
	m_pAnimationLeftLateralState	= new CRabbitLeftLateralAnimationState();
	m_pAnimationRightLateralState	= new CRabbitRightLateralAnimationState();
	m_pAnimationRunAttackState		= new CRabbitRunAttackAnimationState();
	m_pAnimationStillAttackState	= new CRabbitStillAttackAnimationState();
	m_pAnimationWalkState			= new CRabbitWalkAnimationState();
	
	return;
}

void CRabbit::LoadLogicStates( void )
{
	m_pIdleState				= new CRabbitIdleState();
	m_pPursuitState				= new CRabbitPursuitState();
	m_pPreparedToAttackState	= new CRabbitPreparedToAttackState();

	return;
}