#include "Wolf.h"
#include "Characters\Character.h"
#include "Characters\StatesDefs.h"
#include "GameProcess.h"
#include "SoundManager.h"
#include "HUD\HUD.h"
#include "PhysicController.h"

#include "Callbacks\Animation\AnimationCallbackManager.h"

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

#include "LogicStates\WolfIdleState.h"
#include "LogicStates\WolfPursuitState.h"
#include "LogicStates\WolfPreparedToAttackState.h"
#include "LogicStates\WolfAttackState.h"
#include "LogicStates\WolfHitState.h"
#include "LogicStates\WolfTiredState.h"
#include "LogicStates\WolfDefenseState.h"
#include "LogicStates\WolfStillAttackState.h"
#include "LogicStates\WolfStillAttackComboState.h"
#include "LogicStates\WolfRunAttackState.h"
#include "LogicStates\WolfDeathState.h"
#include "LogicStates\WolfHowlEnemiesState.h"
#include "LogicStates\WolfHowlLifeState.h"
#include "LogicStates\WolfHowlEnemiesState.h"

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
#include "AnimationStates\WolfStillAttackComboAnimationState.h"
#include "AnimationStates\WolfRunRunAttackAnimationState.h"
#include "AnimationStates\WolfPreparedRunAttackAnimationState.h"
#include "AnimationStates\WolfImpactRunAttackAnimationState.h"
#include "AnimationStates\WolfWalkAnimationState.h"
#include "AnimationStates\WolfHowlEnemiesAnimationState.h"
#include "AnimationStates\WolfHowlLifeAnimationState.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolf::CWolf( int _Id )
	: m_HitsDone							( 0 )
	, m_ReceivedHits_X_Minut				( 0 )
	, m_TotalReceivedHits_X_Minut			( 3 )
	, m_TotalHitsDoneToTired				( 2 ) 
	, m_pIdleState							( NULL )
	, m_pPursuitState						( NULL )
	, m_pPreparedToAttackState				( NULL )
	, m_pAttackState						( NULL )
	, m_pTiredState							( NULL )
	, m_pHitState							( NULL )
	, m_pDefenseState						( NULL )
	, m_pStillAttackState					( NULL )
	, m_pRunAttackState						( NULL )
	, m_pDeathState							( NULL )
	, m_pHowlLifeState						( NULL )
	, m_pHowlEnemiesState					( NULL )
	, m_pAnimationIdleState 				( NULL )
	//, m_pAnimationIdle2State 				( NULL )
	, m_pAnimationRunState 					( NULL )
	, m_pAnimationDeathState 				( NULL )
	, m_pAnimationDefenseState 				( NULL )
	, m_pAnimationHitState 					( NULL )
	//, m_pAnimationJumpState 				( NULL )
	/*, m_pAnimationLeftLateralState 		( NULL )
	, m_pAnimationRightLateralState			( NULL )*/
	//, m_pAnimationRunAttackState 			( NULL )
	, m_pAnimationRunRunAttackState 		( NULL )
	, m_pAnimationPreparedRunAttackState 	( NULL )
	, m_pAnimationImpactRunAttackState 		( NULL )
	, m_pAnimationStillAttackState 			( NULL )
	, m_pAnimationStillAttackComboState 	( NULL )
	, m_pAnimationWalkState 				( NULL )
	, m_pAnimationHowlLifeState				( NULL )
	, m_pAnimationHowlEnemiesState			( NULL )
	, m_CanHowlForLife						( false )
	, m_CanHowlForEnemies					( true )
	, m_NumEnemyCalls						( 2 )
	, m_NumLifeCalls						( 4 ) 	
{
}

CWolf::CWolf( int _Id, std::string _Name )
	: m_HitsDone							( 0 )
	, m_ReceivedHits_X_Minut				( 0 )
	, m_TotalReceivedHits_X_Minut			( 3 )
	, m_TotalHitsDoneToTired				( 2 ) 
	, m_pIdleState							( NULL )
	, m_pPursuitState						( NULL )
	, m_pPreparedToAttackState				( NULL )
	, m_pAttackState						( NULL )
	, m_pTiredState							( NULL )
	, m_pHitState							( NULL )
	, m_pDefenseState						( NULL )
	, m_pStillAttackState					( NULL )
	, m_pRunAttackState						( NULL )
	, m_pDeathState							( NULL )
	, m_pAnimationIdleState 				( NULL )
	//, m_pAnimationIdle2State 				( NULL )
	, m_pAnimationRunState 					( NULL )
	, m_pAnimationDeathState 				( NULL )
	, m_pAnimationDefenseState 				( NULL )
	, m_pAnimationHitState 					( NULL )
	//, m_pAnimationJumpState 				( NULL )
	/*, m_pAnimationLeftLateralState 		( NULL )
	, m_pAnimationRightLateralState			( NULL )*/
	//, m_pAnimationRunAttackState 			( NULL )
	, m_pAnimationRunRunAttackState 		( NULL )
	, m_pAnimationPreparedRunAttackState 	( NULL )
	, m_pAnimationImpactRunAttackState 		( NULL )
	, m_pAnimationStillAttackState 			( NULL )
	, m_pAnimationStillAttackComboState 	( NULL )
	, m_pAnimationWalkState 				( NULL )
	, m_pAnimationHowlLifeState				( NULL )
	, m_pAnimationHowlEnemiesState			( NULL )
	, m_CanHowlForLife						( false )
	, m_CanHowlForEnemies					( true )
	, m_NumEnemyCalls						( 2 )
	, m_NumLifeCalls						( 4 ) 	
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
	CHECKED_DELETE ( m_pStillAttackComboState );
	CHECKED_DELETE ( m_pRunAttackState );
	CHECKED_DELETE ( m_pDeathState );
	CHECKED_DELETE ( m_pHowlLifeState );
	CHECKED_DELETE ( m_pHowlEnemiesState );

	// Estados animados
	CHECKED_DELETE ( m_pAnimationIdleState );
	//CHECKED_DELETE ( m_pAnimationIdle2State );
	CHECKED_DELETE ( m_pAnimationRunState );
	CHECKED_DELETE ( m_pAnimationDeathState );
	CHECKED_DELETE ( m_pAnimationHitState );
	CHECKED_DELETE ( m_pAnimationDefenseState );
	//CHECKED_DELETE ( m_pAnimationJumpState );
	/*CHECKED_DELETE ( m_pAnimationLeftLateralState );
	CHECKED_DELETE ( m_pAnimationRightLateralState );*/
	CHECKED_DELETE ( m_pAnimationRunRunAttackState );
	CHECKED_DELETE ( m_pAnimationPreparedRunAttackState );
	CHECKED_DELETE ( m_pAnimationImpactRunAttackState );
	CHECKED_DELETE ( m_pAnimationStillAttackState );
	CHECKED_DELETE ( m_pAnimationStillAttackComboState );
	CHECKED_DELETE ( m_pAnimationWalkState );
	CHECKED_DELETE ( m_pAnimationHowlLifeState );
	CHECKED_DELETE ( m_pAnimationHowlEnemiesState );
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

	// Metemos la barra de vida desactivada
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	l_Process->GetHUD()->InitWolf(m_pProperties->GetLife());
	l_Process->GetHUD()->SetActiveWolfBar(false);

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

	CreateSkeaker();
	
	return true;
}

void CWolf::CreateCallbacks(void)
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());

	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_HIT_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_STILL_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_STILL_ATTACK_COMBO_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_ONLY_IMPACT_RUN_ATTACK_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_DEATH_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_HOWL_LIFE_STATE, this->GetAnimatedModel());
	l_Process->GetAnimationCallbackManager()->CreateCallback(GetName(), WOLF_HOWL_ENEMIES_STATE, this->GetAnimatedModel());
}

void CWolf::LoadGraphicStates( void )
{
	m_pAnimationIdleState				= new CWolfIdleAnimationState				(this);
	//m_pAnimationIdle2State				= new CWolfIdle2AnimationState			(this);
	m_pAnimationRunState				= new CWolfRunAnimationState				(this);
	m_pAnimationDeathState				= new CWolfDeathAnimationState				(this);
	m_pAnimationHitState				= new CWolfHitAnimationState				(this);
	m_pAnimationDefenseState			= new CWolfDefenseAnimationState			(this);
	//m_pAnimationJumpState				= new CWolfJumpAnimationState				(this);
	//m_pAnimationLeftLateralState		= new CWolfLeftLateralAnimationState		(this);
	//m_pAnimationRightLateralState		= new CWolfRightLateralAnimationState		(this);
	//m_pAnimationRunAttackState		= new CWolfRunAttackAnimationState			(this);
	m_pAnimationRunRunAttackState		= new CWolfRunRunAttackAnimationState		(this);
	m_pAnimationPreparedRunAttackState	= new CWolfPreparedRunAttackAnimationState	(this);
	m_pAnimationImpactRunAttackState	= new CWolfImpactRunAttackAnimationState	(this);
	m_pAnimationStillAttackState		= new CWolfStillAttackAnimationState		(this);
	m_pAnimationStillAttackComboState	= new CWolfStillAttackComboAnimationState	(this);
	m_pAnimationWalkState				= new CWolfWalkAnimationState				(this);
	m_pAnimationHowlLifeState			= new CWolfHowlLifeAnimationState			(this);
	m_pAnimationHowlEnemiesState		= new CWolfHowlEnemiesAnimationState		(this);

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
	m_pStillAttackComboState	= new CWolfStillAttackComboState			(this);
	m_pRunAttackState			= new CWolfRunAttackState					(this);
	m_pDeathState				= new CWolfDeathState						(this);
	m_pHowlLifeState			= new CWolfHowlLifeState					(this);
	m_pHowlEnemiesState			= new CWolfHowlEnemiesState					(this);
	return;
}

void CWolf::Update ( float _ElapsedTime )			
{ 
	if( !m_pProperties->GetActive() ) 
		return;
	
	m_pLogicStateMachine->Update( _ElapsedTime );
	m_pGraphicStateMachine->Update( _ElapsedTime );
	
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	l_Process->GetHUD()->SetWolfLife(m_pProperties->GetCurrentLife());
}

void CWolf::CreateSkeaker()
{
	//Crea el speaker de audio correspondiente
	uint16 index = CORE->GetSoundManager()->GetSpeakerCount();
	std::stringstream out;
	out << "_";
	out << index;

	m_SpeakerName = "Wolf_Speaker_" + out.str();
	m_pSpeaker = CORE->GetSoundManager()->CreateSpeaker(m_SpeakerName);

	CORE->GetSoundManager()->PlayEvent(m_SpeakerName, "Stop_All_EFX_Wolf");
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


// ------------------------------------------------------------------------------------------------------------------
//  TestIfCanHowlForLife: Solo aumentamos la vida en caso de estar a la mitad o casi muerto. Esta función comprueba
//						si es estamos en un nivel de vida para activar el estado
// ------------------------------------------------------------------------------------------------------------------
bool CWolf::TestIfCanHowlForLife( void )
{
	// Si estoy entre el 75% y el 70%
	if ( ( m_NumLifeCalls > 0 )  && ( m_pProperties->GetCurrentLife() <= m_pProperties->GetLife() * 0.75f ) && ( m_pProperties->GetCurrentLife() >= m_pProperties->GetLife() * 0.70f ) ) 
	{
		return true;
	}

	// Si estoy entre el 50% y el 33%
	if ( ( m_NumLifeCalls > 0 )  && ( m_pProperties->GetCurrentLife() <= m_pProperties->GetLife() * 0.5f ) && ( m_pProperties->GetCurrentLife() >= m_pProperties->GetLife() * 0.33f  ) ) 
	{
		return true;
	}
	
	// Si estoy entre el 30% y 10% 
	if ( ( m_NumLifeCalls > 0 )  && ( m_pProperties->GetCurrentLife() <= m_pProperties->GetLife() * 0.30f ) && ( m_pProperties->GetCurrentLife() >= m_pProperties->GetLife() * 0.10 ) ) 
	{
		return true;
	}
	
	return false;
}

// ------------------------------------------------------------------------------------------------------------------
//  TestIfCanHowlForEnemies: Hacemos aparecer enemigos en el momento de aparición del lobo y en el momento que la vida
//							esté a la mitad
// ------------------------------------------------------------------------------------------------------------------
bool CWolf::TestIfCanHowlForEnemies( void )
{
	// Si tenemos máxima vida o la mitad o menos tenemos que aullar
	if ( ( m_NumEnemyCalls > 0 )  && ( ( m_pProperties->GetCurrentLife() == m_pProperties->GetLife() ) || ( m_pProperties->GetCurrentLife() <= m_pProperties->GetLife()/2 ) ) )
	{
		return true;
	}

	return false;
}

bool CWolf::CallHitState( CCharacter* _pCharacter, const STelegram& _Message )
{
	if( _Message.Msg == Msg_Attack )
	{
		/*CRandom	l_Randomize;

		CCharacter *l_pEnemy	= m_pProcess->GetCharactersManager()->GetCharacterById(_Message.Sender);
		float l_fReceivedPain	= l_Randomize.getRandFloat( (float)(l_pEnemy->GetProperties()->GetStrong() / 2), (float)l_pEnemy->GetProperties()->GetStrong());
		float l_fPainToHit		= l_pEnemy->GetProperties()->GetStrong() * 0.95f;*/

		/*if( l_fReceivedPain >= l_fPainToHit )
		{*/

		STelegram l_Message = _Message;
		CWolfHitState *l_HitState = dynamic_cast<CWolfHitState*> ( GetHitState() );
		l_HitState->UpdateParameters(l_Message);
		_pCharacter->GetLogicFSM()->ChangeState( GetHitState() );
		_pCharacter->GetGraphicFSM()->ChangeState( GetHitAnimationState() );
		//}

		return true;
	}

	return false;
}

void CWolf::SetSoundsOff( void )
{
	CORE->GetSoundManager()->PlayEvent(GetSpeakerName(), "Stop_All_EFX_Wolf");
}

void CWolf::SetSoundsPauseResume( bool _Active )
{
	if ( _Active )
	{
		CORE->GetSoundManager()->PlayEvent(GetSpeakerName(), "Resume_All_EFX_Game");
		return;
	}
	CORE->GetSoundManager()->PlayEvent(GetSpeakerName(), "Pause_All_EFX_Wolf");
}

void CWolf::RemoveActions()
{
	m_pCurrentAnimatedModel->RemoveAction( GetAnimationID(WOLF_DEATH_STATE) );
}
