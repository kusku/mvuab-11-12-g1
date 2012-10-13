#include "Rabbit.h"
#include "Characters\Character.h"
#include "Characters\StatesDefs.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "SoundManager.h"

#include "StatesMachine\StateMachine.h"
#include "StatesMachine\State.h"

#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"
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

#include "Characters\Enemies\Rabbit\LogicStates\RabbitIdleState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitPursuitState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitPreparedToAttackState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitAttackState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitHitState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitTiredState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitDefenseState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitStillAttackState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitRunAttackState.h"
#include "Characters\Enemies\Rabbit\LogicStates\RabbitDeathState.h"

#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitIdleAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitIdle2AnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitRunAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitDeathAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitDefenseAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitHitAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitJumpAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitLeftLateralAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitRightLateralAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitStillAttackAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitRunAttackAnimationState.h"
#include "Characters\Enemies\Rabbit\AnimationStates\\RabbitWalkAnimationState.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "SoundManager.h"
#include "Speaker.h"
#include <sstream>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbit::CRabbit( int _Id )
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
	, m_pDeathState					( NULL )
	, m_pAnimationIdleState 		( NULL )
	, m_pAnimationIdle2State 		( NULL )
	, m_pAnimationRunState 			( NULL )
	, m_pAnimationDeathState 		( NULL )
	, m_pAnimationDefenseState 		( NULL )
	, m_pAnimationHitState 			( NULL )
	/*, m_pAnimationJumpState 		( NULL )*/
	, m_pAnimationLeftLateralState 	( NULL )
	, m_pAnimationRightLateralState ( NULL )
	, m_pAnimationRunAttackState 	( NULL )
	, m_pAnimationStillAttackState 	( NULL )
	, m_pAnimationWalkState 		( NULL )
{
}

CRabbit::CRabbit( int _Id, std::string _Name )
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
	, m_pDeathState					( NULL )
	, m_pAnimationIdleState 		( NULL )
	, m_pAnimationIdle2State 		( NULL )
	, m_pAnimationRunState 			( NULL )
	, m_pAnimationDeathState 		( NULL )
	, m_pAnimationDefenseState 		( NULL )
	, m_pAnimationHitState 			( NULL )
	//, m_pAnimationJumpState 		( NULL )
	, m_pAnimationLeftLateralState 	( NULL )
	, m_pAnimationRightLateralState ( NULL )
	, m_pAnimationRunAttackState 	( NULL )
	, m_pAnimationStillAttackState 	( NULL )
	, m_pAnimationWalkState 		( NULL )
{
}


CRabbit::~CRabbit(void)
{
	CORE->GetSoundManager()->PlayEvent(GetSpeakerName(), "Stop_All_EFX_Rabbit");

	CHECKED_DELETE ( m_pIdleState );
	CHECKED_DELETE ( m_pPursuitState );
	CHECKED_DELETE ( m_pPreparedToAttackState );
	CHECKED_DELETE ( m_pAttackState	);
	CHECKED_DELETE ( m_pTiredState );
	CHECKED_DELETE ( m_pHitState );
	CHECKED_DELETE ( m_pDefenseState );
	CHECKED_DELETE ( m_pStillAttack );
	CHECKED_DELETE ( m_pRunAttack );
	CHECKED_DELETE ( m_pDeathState );
	
	// Estados animados
	CHECKED_DELETE ( m_pAnimationIdleState );
	CHECKED_DELETE ( m_pAnimationIdle2State );
	CHECKED_DELETE ( m_pAnimationRunState );
	CHECKED_DELETE ( m_pAnimationDeathState );
	CHECKED_DELETE ( m_pAnimationHitState );
	CHECKED_DELETE ( m_pAnimationDefenseState );
	//CHECKED_DELETE ( m_pAnimationJumpState );
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
	this->GetBehaviors()->AddBehavior( new CObstacleWallAvoidance( CORE->GetSteeringBehaviourSettingsManager()->GetObstacleWallDetectionFeelerLength() ) );

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

	m_SpeakerName = "Rabbit_Speaker_" + out.str();
	m_pSpeaker = CORE->GetSoundManager()->CreateSpeaker(m_SpeakerName);

	return true;
}

void CRabbit::CreateCallbacks(void)
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), RABBIT_HIT_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), RABBIT_STILL_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), RABBIT_RUN_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), RABBIT_DEATH_STATE, this->GetAnimatedModel());
}

void CRabbit::Update( float _ElapsedTime )
{
	CCharacter::Update(_ElapsedTime);

	m_pSpeaker->SetPosition( m_Position );
	m_pSpeaker->SetOrientation( m_pCurrentAnimatedModel->GetFront() );
}

void CRabbit::LoadGraphicStates( void )
{
	m_pAnimationIdleState			= new CRabbitIdleAnimationState			(this);
	m_pAnimationIdle2State			= new CRabbitIdle2AnimationState		(this);
	m_pAnimationRunState			= new CRabbitRunAnimationState			(this);
	m_pAnimationDeathState			= new CRabbitDeathAnimationState		(this);
	m_pAnimationHitState			= new CRabbitHitAnimationState			(this);
	m_pAnimationDefenseState		= new CRabbitDefenseAnimationState		(this);
	//m_pAnimationJumpState			= new CRabbitJumpAnimationState			(this);
	m_pAnimationLeftLateralState	= new CRabbitLeftLateralAnimationState	(this);
	m_pAnimationRightLateralState	= new CRabbitRightLateralAnimationState	(this);
	m_pAnimationRunAttackState		= new CRabbitRunAttackAnimationState	(this);
	m_pAnimationStillAttackState	= new CRabbitStillAttackAnimationState	(this);
	m_pAnimationWalkState			= new CRabbitWalkAnimationState			(this);
	
	return;
}

void CRabbit::LoadLogicStates( void )
{
	m_pIdleState				= new CRabbitIdleState						(this);
	m_pPursuitState				= new CRabbitPursuitState					(this);
	m_pPreparedToAttackState	= new CRabbitPreparedToAttackState			(this);
	m_pAttackState				= new CRabbitAttackState					(this);
	m_pTiredState				= new CRabbitTiredState						(this);
	m_pHitState					= new CRabbitHitState						(this);
	m_pDefenseState				= new CRabbitDefenseState					(this);
	m_pStillAttack				= new CRabbitStillAttackState				(this);
	m_pRunAttack				= new CRabbitRunAttackState					(this);
	m_pDeathState				= new CRabbitDeathState						(this);
	return;
}

// -----------------------------------------
//				  METHODS
// -----------------------------------------
bool CRabbit::IsFatigued( void )
{
	return m_HitsDone == m_TotalHitsDoneToTired;
}

void CRabbit::BeDead( void )
{
	this->GetLogicFSM()->ChangeState(GetDeathState());
}

bool CRabbit::CallHitState( CCharacter* _pCharacter, const STelegram& _Message )
{
	if( _Message.Msg == Msg_Attack )
	{
		STelegram l_Message = _Message;
		CRabbitHitState *l_HitState = dynamic_cast<CRabbitHitState*> ( GetHitState() );
		l_HitState->UpdateParameters(l_Message);
		_pCharacter->GetLogicFSM()->ChangeState( GetHitState() );
		_pCharacter->GetGraphicFSM()->ChangeState( GetHitAnimationState() );
		
		return true;
	}

	return false;
}