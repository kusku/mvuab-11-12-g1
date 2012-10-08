#include "Deer.h"
#include "Characters\Character.h"
#include "Characters\StatesDefs.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "SoundManager.h"

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
#include "LogicStates\DeerDeathState.h"

#include "AnimationStates\DeerIdleAnimationState.h"
#include "AnimationStates\DeerRunAnimationState.h"
#include "AnimationStates\DeerDeathAnimationState.h"
#include "AnimationStates\DeerDefenseAnimationState.h"
#include "AnimationStates\DeerHitAnimationState.h"
#include "AnimationStates\DeerStillAttackAnimationState.h"
#include "AnimationStates\DeerRunAttackAnimationState.h"
#include "AnimationStates\DeerWalkAnimationState.h"

#include "RenderableObjects/AnimatedModel/AnimatedInstanceModel.h"

#include "SoundManager.h"
#include "Core.h"
#include "Base.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

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
	, m_pStillAttackState			( NULL )
	, m_pRunAttackState				( NULL )
	, m_pDeathState					( NULL )
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
	, m_pStillAttackState			( NULL )
	, m_pRunAttackState				( NULL )
	, m_pDeathState					( NULL )
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
	CORE->GetSoundManager()->PlayEvent(GetSpeakerName(), "Stop_All_EFX_Rabbit");

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

	m_SpeakerName = "Deer_Speaker_" + out.str();
	m_pSpeaker = CORE->GetSoundManager()->CreateSpeaker(m_SpeakerName);

	return true;
}

void CDeer::Update( float _ElapsedTime )
{
	CCharacter::Update(_ElapsedTime);

	m_pSpeaker->SetPosition( m_Position );
	m_pSpeaker->SetOrientation( m_pCurrentAnimatedModel->GetFront() );
}

void CDeer::CreateCallbacks(void)
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), DEER_HIT_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), DEER_STILL_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), DEER_RUN_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), DEER_DEATH_STATE, this->GetAnimatedModel());
}

void CDeer::LoadGraphicStates( void )
{
	m_pAnimationIdleState			= new CDeerIdleAnimationState			(this);
	m_pAnimationRunState			= new CDeerRunAnimationState			(this);
	m_pAnimationDeathState			= new CDeerDeathAnimationState			(this);
	m_pAnimationHitState			= new CDeerHitAnimationState			(this);
	m_pAnimationDefenseState		= new CDeerDefenseAnimationState		(this);
	m_pAnimationRunAttackState		= new CDeerRunAttackAnimationState		(this);
	m_pAnimationStillAttackState	= new CDeerStillAttackAnimationState	(this);
	m_pAnimationWalkState			= new CDeerWalkAnimationState			(this);
	
	return;
}

void CDeer::LoadLogicStates( void )
{
	m_pIdleState				= new CDeerIdleState				(this);
	m_pPursuitState				= new CDeerPursuitState				(this);
	m_pPreparedToAttackState	= new CDeerPreparedToAttackState	(this);
	m_pAttackState				= new CDeerAttackState				(this);
	m_pTiredState				= new CDeerTiredState				(this);
	m_pHitState					= new CDeerHitState					(this);
	m_pDefenseState				= new CDeerDefenseState				(this);
	m_pStillAttackState			= new CDeerStillAttackState			(this);
	m_pRunAttackState			= new CDeerRunAttackState			(this);
	m_pDeathState				= new CDeerDeathState				(this);
	
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
	this->GetLogicFSM()->ChangeState(GetDeathState());
}