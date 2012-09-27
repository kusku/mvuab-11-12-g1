#include "Wolf.h"
#include "Characters\Character.h"
#include "Characters\StatesDefs.h"
#include "GameProcess.h"
#include "SoundManager.h"

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

#include "LogicStates\WolfIdleState.h"
#include "LogicStates\WolfPursuitState.h"
#include "LogicStates\WolfPreparedToAttackState.h"
#include "LogicStates\WolfAttackState.h"
#include "LogicStates\WolfHitState.h"
#include "LogicStates\WolfTiredState.h"
#include "LogicStates\WolfDefenseState.h"
#include "LogicStates\WolfStillAttackState.h"
#include "LogicStates\WolfRunAttackState.h"
#include "LogicStates\WolfDeathState.h"

#include "AnimationStates\WolfIdleAnimationState.h"
//#include "AnimationStates\WolfIdle2AnimationState.h"
#include "AnimationStates\WolfRunAnimationState.h"
#include "AnimationStates\WolfDeathAnimationState.h"
#include "AnimationStates\WolfDefenseAnimationState.h"
#include "AnimationStates\WolfHitAnimationState.h"
#include "AnimationStates\WolfJumpAnimationState.h"
//#include "AnimationStates\WolfLeftLateralAnimationState.h"
//#include "AnimationStates\WolfRightLateralAnimationState.h"
#include "AnimationStates\WolfStillAttackAnimationState.h"
#include "AnimationStates\WolfRunAttackAnimationState.h"
#include "AnimationStates\WolfWalkAnimationState.h"


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolf::CWolf( int _Id )
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
	, m_pStillAttackState			( NULL )
	, m_pRunAttackState				( NULL )
	, m_pDeathState					( NULL )
	, m_pAnimationIdleState 		( NULL )
	//, m_pAnimationIdle2State 		( NULL )
	, m_pAnimationRunState 			( NULL )
	, m_pAnimationDeathState 		( NULL )
	, m_pAnimationDefenseState 		( NULL )
	, m_pAnimationHitState 			( NULL )
	, m_pAnimationJumpState 		( NULL )
	/*, m_pAnimationLeftLateralState 	( NULL )
	, m_pAnimationRightLateralState ( NULL )*/
	, m_pAnimationRunAttackState 	( NULL )
	, m_pAnimationStillAttackState 	( NULL )
	, m_pAnimationWalkState 		( NULL )
{
}

CWolf::CWolf( int _Id, std::string _Name )
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
	, m_pStillAttackState			( NULL )
	, m_pRunAttackState				( NULL )
	, m_pDeathState					( NULL )
	, m_pAnimationIdleState 		( NULL )
	//, m_pAnimationIdle2State 		( NULL )
	, m_pAnimationRunState 			( NULL )
	, m_pAnimationDeathState 		( NULL )
	, m_pAnimationDefenseState 		( NULL )
	, m_pAnimationHitState 			( NULL )
	, m_pAnimationJumpState 		( NULL )
	/*, m_pAnimationLeftLateralState 	( NULL )
	, m_pAnimationRightLateralState ( NULL )*/
	, m_pAnimationRunAttackState 	( NULL )
	, m_pAnimationStillAttackState 	( NULL )
	, m_pAnimationWalkState 		( NULL )
{
}


CWolf::~CWolf(void)
{
	CHECKED_DELETE ( m_pIdleState );
	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pPreparedToAttackState );
	CHECKED_DELETE ( m_pAttackState	);
	CHECKED_DELETE ( m_pTiredState );
	CHECKED_DELETE ( m_pHitState );
	CHECKED_DELETE ( m_pDefenseState );
	CHECKED_DELETE ( m_pStillAttackState );
	CHECKED_DELETE ( m_pRunAttackState );
	CHECKED_DELETE ( m_pDeathState );

	// Estados animados
	CHECKED_DELETE ( m_pAnimationIdleState );
	//CHECKED_DELETE ( m_pAnimationIdle2State );
	CHECKED_DELETE ( m_pAnimationRunState );
	CHECKED_DELETE ( m_pAnimationDeathState );
	CHECKED_DELETE ( m_pAnimationHitState );
	CHECKED_DELETE ( m_pAnimationDefenseState );
	CHECKED_DELETE ( m_pAnimationJumpState );
	/*CHECKED_DELETE ( m_pAnimationLeftLateralState );
	CHECKED_DELETE ( m_pAnimationRightLateralState );*/
	CHECKED_DELETE ( m_pAnimationRunAttackState );
	CHECKED_DELETE ( m_pAnimationStillAttackState );
	CHECKED_DELETE ( m_pAnimationWalkState );
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
bool CWolf::Init( void )
{
	bool l_IsOk = false;

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

	//Crea el speaker de audio correspondiente
	uint16 index = CORE->GetSoundManager()->GetSpeakerCount();
	std::stringstream out;
	out << "_";
	out << index;

	m_SpeakerName = "Wolf_Speaker_" + out.str();
	m_pSpeaker = CORE->GetSoundManager()->CreateSpeaker(m_SpeakerName);

	return true;
}

void CWolf::CreateCallbacks(void)
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());

	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_HIT_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_STILL_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_RUN_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_DEATH_STATE, this->GetAnimatedModel());
}

void CWolf::LoadGraphicStates( void )
{
	m_pAnimationIdleState			= new CWolfIdleAnimationState			(this);
	//m_pAnimationIdle2State			= new CWolfIdle2AnimationState		(this);
	m_pAnimationRunState			= new CWolfRunAnimationState			(this);
	m_pAnimationDeathState			= new CWolfDeathAnimationState			(this);
	m_pAnimationHitState			= new CWolfHitAnimationState			(this);
	m_pAnimationDefenseState		= new CWolfDefenseAnimationState		(this);
	m_pAnimationJumpState			= new CWolfJumpAnimationState			(this);
	//m_pAnimationLeftLateralState	= new CWolfLeftLateralAnimationState	(this);
	//m_pAnimationRightLateralState	= new CWolfRightLateralAnimationState	(this);
	m_pAnimationRunAttackState		= new CWolfRunAttackAnimationState		(this);
	m_pAnimationStillAttackState	= new CWolfStillAttackAnimationState	(this);
	m_pAnimationWalkState			= new CWolfWalkAnimationState			(this);

	return;
}

void CWolf::LoadLogicStates( void )
{
	m_pIdleState				= new CWolfIdleState						(this);
	m_pPursuitState				= new CWolfPursuitState						(this);
	m_pPreparedToAttackState	= new CWolfPreparedToAttackState			(this);
	m_pAttackState				= new CWolfAttackState						(this);
	m_pTiredState				= new CWolfTiredState						(this);
	m_pHitState					= new CWolfHitState							(this);
	m_pDefenseState				= new CWolfDefenseState						(this);
	m_pStillAttackState			= new CWolfStillAttackState					(this);
	m_pRunAttackState			= new CWolfRunAttackState					(this);
	m_pDeathState				= new CWolfDeathState						(this);

	return;
}

// -----------------------------------------
//				  METHODS
// -----------------------------------------
bool CWolf::IsFatigued( void )
{
	return m_HitsDone == m_TotalHitsDoneToTired;
}

void CWolf::BeDead( void )
{
	this->GetLogicFSM()->ChangeState(GetDeathState());
}