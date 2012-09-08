#include "Deer.h"
#include "Characters\Character.h"
#include "Characters\StatesDefs.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "StatesMachine\StateMachine.h"
#include "StatesMachine\State.h"

#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"
#include "Steering Behaviors\Flee.h"
#include "Steering Behaviors\Pursuit.h"
#include "Steering Behaviors\Evade.h"
#include "Steering Behaviors\Arrive.h"
#include "Steering Behaviors\Wander.h"
#include "Steering Behaviors\CollisionAvoidance.h"
#include "Steering Behaviors\ObstacleWallAvoidance.h"
#include "Steering Behaviors\Separation.h"
#include "Steering Behaviors\Cohesion.h"
#include "Steering Behaviors\Alignment.h"

#include "LogicStates\DeerIdleState.h"
#include "LogicStates\DeerPursuitState.h"
#include "LogicStates\DeerPreparedToAttackState.h"
#include "LogicStates\DeerAttackState.h"
#include "LogicStates\DeerHitState.h"
#include "LogicStates\DeerTiredState.h"
#include "LogicStates\DeerDefenseState.h"
#include "LogicStates\DeerStillAttackState.h"
#include "LogicStates\DeerRunAttackState.h"

#include "AnimationStates\DeerIdleAnimationState.h"
#include "AnimationStates\DeerRunAnimationState.h"
#include "AnimationStates\DeerDeathAnimationState.h"
#include "AnimationStates\DeerDefenseAnimationState.h"
#include "AnimationStates\DeerHitAnimationState.h"
#include "AnimationStates\DeerStillAttackAnimationState.h"
#include "AnimationStates\DeerRunAttackAnimationState.h"
#include "AnimationStates\DeerWalkAnimationState.h"


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeer::CDeer( int _Id )
	: m_HitsDone					( 0 )
	, m_ReceivedHits_X_Minut		( 0 )
	, m_TotalReceivedHits_X_Minut	( 3 )
	, m_TotalHitsDoneToTired		( 2 ) 
	, m_pIdleState					( NULL )
	, m_pPursuitState				( NULL )
	, m_pPreparedToAttackState		( NULL )
	, m_pAttackState				( NULL )
	, m_pTiredState					( NULL )
	, m_pHitState					( NULL )
	, m_pDefenseState				( NULL )
	, m_pStillAttack				( NULL )
	, m_pRunAttack					( NULL )
	, m_pAnimationIdleState 		( NULL )
	, m_pAnimationRunState 			( NULL )
	, m_pAnimationDeathState 		( NULL )
	, m_pAnimationDefenseState 		( NULL )
	, m_pAnimationHitState 			( NULL )
	, m_pAnimationRunAttackState 	( NULL )
	, m_pAnimationStillAttackState 	( NULL )
	, m_pAnimationWalkState 		( NULL )
{
}

CDeer::CDeer( int _Id, std::string _Name )
	: m_HitsDone					( 0 )
	, m_ReceivedHits_X_Minut		( 0 )
	, m_TotalReceivedHits_X_Minut	( 3 )
	, m_TotalHitsDoneToTired		( 2 ) 
	, m_pIdleState					( NULL )
	, m_pPursuitState				( NULL )
	, m_pPreparedToAttackState		( NULL )
	, m_pAttackState				( NULL )
	, m_pTiredState					( NULL )
	, m_pHitState					( NULL )
	, m_pDefenseState				( NULL )
	, m_pStillAttack				( NULL )
	, m_pRunAttack					( NULL )
	, m_pAnimationIdleState 		( NULL )
	, m_pAnimationRunState 			( NULL )
	, m_pAnimationDeathState 		( NULL )
	, m_pAnimationDefenseState 		( NULL )
	, m_pAnimationHitState 			( NULL )
	, m_pAnimationRunAttackState 	( NULL )
	, m_pAnimationStillAttackState 	( NULL )
	, m_pAnimationWalkState 		( NULL )
{
}


CDeer::~CDeer(void)
{
	CHECKED_DELETE ( m_pIdleState );
	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pPreparedToAttackState );
	CHECKED_DELETE ( m_pAttackState	);
	CHECKED_DELETE ( m_pTiredState );
	CHECKED_DELETE ( m_pHitState );
	CHECKED_DELETE ( m_pDefenseState );
	CHECKED_DELETE ( m_pStillAttack );
	CHECKED_DELETE ( m_pRunAttack );

	// Estados animados
	CHECKED_DELETE ( m_pAnimationIdleState );
	CHECKED_DELETE ( m_pAnimationRunState );
	CHECKED_DELETE ( m_pAnimationDeathState );
	CHECKED_DELETE ( m_pAnimationHitState );
	CHECKED_DELETE ( m_pAnimationDefenseState );
	CHECKED_DELETE ( m_pAnimationRunAttackState );
	CHECKED_DELETE ( m_pAnimationStillAttackState );
	CHECKED_DELETE ( m_pAnimationWalkState );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
bool CDeer::Init( void )
{ 
	bool l_IsOk = false;

	//CState<CCharacter>* l = dynamic_cast<CState<CCharacter>> (m_pAnimationIdleState);
	CreateCallbacks();
	LoadGraphicStates();
	LoadLogicStates();

	// Coloco los estados iniciales
	/*this->GetGraphicFSM()->SetCurrentState( m_pAnimationIdleState );
	this->GetLogicFSM()->SetCurrentState(m_pIdleState);*/
	this->GetGraphicFSM()->ChangeState( m_pAnimationIdleState );
	this->GetLogicFSM()->ChangeState(m_pIdleState);


	this->GetBehaviors()->AddBehavior( new CSeek() );
	this->GetBehaviors()->AddBehavior( new CFlee( this->GetProperties()->GetPanicDistance()) );
	this->GetBehaviors()->AddBehavior( new CPursuit() );
	this->GetBehaviors()->AddBehavior( new CEvade(this->GetProperties()->GetPanicDistance()) );
	this->GetBehaviors()->AddBehavior( new CArrive() );
	this->GetBehaviors()->AddBehavior( new CWander() );
	this->GetBehaviors()->AddBehavior( new CCollisionAvoidance(this->GetProperties()->GetPanicDistance()) );
	this->GetBehaviors()->AddBehavior( new CObstacleWallAvoidance(this->GetProperties()->GetDetectionDistance()) );

	// --- Flocking ---
	this->GetBehaviors()->AddBehavior( new CSeparation() );
	this->GetBehaviors()->AddBehavior( new CAlignment() );
	this->GetBehaviors()->AddBehavior( new CCohesion() );
	

	this->GetBehaviors()->SeekOff();
	this->GetBehaviors()->FleeOff();
	this->GetBehaviors()->ArriveOff();
	this->GetBehaviors()->PursuitOff();
	this->GetBehaviors()->EvadeOff();
	this->GetBehaviors()->WanderOff();
	this->GetBehaviors()->SeparationOff();
	this->GetBehaviors()->AlignmentOff();
	this->GetBehaviors()->CohesionOff();
	this->GetBehaviors()->CollisionAvoidanceOff();
	this->GetBehaviors()->ObstacleWallAvoidanceOff();

	this->MoveTo2( Vect3f(0,0,0), 0 );

	return true;
}

void CDeer::CreateCallbacks(void)
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	l_Process->GetAnimationCallbackManager()->CreateCallback(HIT_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(STILL_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(RUN_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(DEATH_STATE, this->GetAnimatedModel());
}

void CDeer::LoadGraphicStates( void )
{
	m_pAnimationIdleState			= new CDeerIdleAnimationState();
	m_pAnimationRunState			= new CDeerRunAnimationState();
	m_pAnimationDeathState			= new CDeerDeathAnimationState();
	m_pAnimationHitState			= new CDeerHitAnimationState();
	m_pAnimationDefenseState		= new CDeerDefenseAnimationState();
	m_pAnimationRunAttackState		= new CDeerRunAttackAnimationState();
	m_pAnimationStillAttackState	= new CDeerStillAttackAnimationState();
	m_pAnimationWalkState			= new CDeerWalkAnimationState();
	
	return;
}

void CDeer::LoadLogicStates( void )
{
	m_pIdleState				= new CDeerIdleState();
	m_pPursuitState				= new CDeerPursuitState();
	m_pPreparedToAttackState	= new CDeerPreparedToAttackState();
	m_pAttackState				= new CDeerAttackState();
	m_pTiredState				= new CDeerTiredState();
	m_pHitState					= new CDeerHitState();
	m_pDefenseState				= new CDeerDefenseState();
	m_pStillAttack				= new CDeerStillAttackState();
	m_pRunAttack				= new CDeerRunAttackState();

	return;
}

// -----------------------------------------
//				  METHODS
// -----------------------------------------
bool CDeer::IsFatigued( void )
{
	return m_HitsDone == m_TotalHitsDoneToTired;
}

void CDeer::BeDead( void )
{
	//this->GetLogicFSM()->ChangeState(GetDeathState());
}