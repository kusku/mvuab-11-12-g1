#include "DeerHitState.h"
#include "Utils\BoostRandomHelper.h"
#include "GameProcess.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "SoundManager.h"
#include "Core.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\Telegram.h"

#include "Characters\Enemies\Deer\Deer.h"
#include "Characters\StatesDefs.h"

#include "DeerIdleState.h"
#include "DeerTiredState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeeridleAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviors.h"
#include "Steering Behaviors\Seek.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"


#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerHitState::CDeerHitState( void )
	: CState						("CDeerHitState")
	, m_pDeer						( NULL )
	, m_pActionStateCallback		( 0.f, 1.f )
	, m_pActionRecoilStateCallback	( 0.f, 1.f )
	, m_pAnimationCallback			( NULL )
	, m_IsCommingFromTired			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_HIT_STATE);
}

CDeerHitState::CDeerHitState( const std::string &_Name )
	: CState						(_Name)
	, m_pDeer						( NULL )
	, m_pActionStateCallback		( 0.f, 1.f )
	, m_pActionRecoilStateCallback	( 0.f, 1.f )
	, m_pAnimationCallback			( NULL )
	, m_IsCommingFromTired			( false )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(DEER_HIT_STATE);
}


CDeerHitState::~CDeerHitState(void)
{
	m_pDeer = NULL;
	m_pAnimationCallback = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerHitState::OnEnter( CCharacter* _Character )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	// Si volvemos de haber recibido y después de estar cansados nos salimos.
	if ( m_IsCommingFromTired ) 
	{
		m_pDeer->GetTiredState()->SetTiredTime(m_RecoverMinTiredTime, m_RecoverMaxTiredTime);	// Recuperamos el tiempo que teneniamos por defecto asignado al estado TIRED
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetIdleState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
		m_IsCommingFromTired = false;
	}
	// Si entramos por primera vez ejecutaremos el hit normal
	else 
	{
		m_pAnimationCallback->Init();
		m_pAnimationCallback->StartAnimation();
	
		PlayRandomSound();
		m_pActionStateCallback.InitAction(0, m_SoundDuration);
		m_pActionStateCallback.StartAction();

		//m_pActionStateCallback.InitAction(0.f, m_pDeer->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_HIT_STATE));
		//m_pActionStateCallback.StartAction();

		// --- Para la gestión del retroceso ---
		m_OldMaxSpeed = m_pDeer->GetProperties()->GetMaxSpeed();
		m_pDeer->GetProperties()->SetMaxSpeed(3.f);

		m_HitDirection = m_pDeer->GetSteeringEntity()->GetFront();
		m_HitDirection.Normalize();
		m_HitDirection = m_HitDirection.RotateY(mathUtils::PiTimes(1.f));		
		/*m_pDeer->GetBehaviors()->SeekOn();
		m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));*/
		//m_pDeer->GetBehaviors()->GetSeek()->SetTarget(m_HitDirection);
		
		m_MaxHitDistance = m_pDeer->GetProperties()->GetImpactDistance() * 2;
		// ---------------------------------------
	}

	
	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = DEER_HIT_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}

void CDeerHitState::Execute( CCharacter* _Character, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_Character);
	}

	/*if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{
		if ( m_pAnimationCallback->IsAnimationFinished() ) 
		{
			m_pDeer->GetLogicFSM()->RevertToPreviousState();
			m_pDeer->GetGraphicFSM()->RevertToPreviousState();
		}
	}*/
	/*else
	{
		m_pAnimationCallback->StartAnimation();
	}*/

	if ( m_pActionStateCallback.IsActionFinished() )
	{
		// restablecemos la velocidad máxima
		m_pDeer->GetProperties()->SetMaxSpeed(m_OldMaxSpeed);

		if ( m_pDeer->IsAlive() ) 
		{
			// Obligo a descansar entre unos segundosw
			float l_MaxTimeInTired = BoostRandomHelper::GetFloat(3, 5);
			m_RecoverMinTiredTime = m_pDeer->GetTiredState()->GetMinTiredTime();
			m_RecoverMaxTiredTime = m_pDeer->GetTiredState()->GetMaxTiredTime();
			m_pDeer->GetTiredState()->SetTiredTime(0.f, l_MaxTimeInTired);
			m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetTiredState());
			m_IsCommingFromTired = true;
		}
	}
	else
	{
		m_pActionStateCallback.Update(_ElapsedTime);
		
		// Gestiono el retroceso del hit
		float l_Distance = m_pDeer->GetDistanceToPlayer();
		if ( l_Distance > m_MaxHitDistance ) 
		{
			m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			m_pDeer->MoveTo2( m_pDeer->GetSteeringEntity()->GetVelocity(), _ElapsedTime );
		} 
		else
		{
			//m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
			//m_pDeer->GetBehaviors()->GetSeek()->SetTarget(l_Front);
			//m_pDeer->FaceTo( m_pDeer->GetPlayer()->GetPosition(), _ElapsedTime );
			m_pDeer->MoveTo2(m_HitDirection, _ElapsedTime );
		}
	}
}

void CDeerHitState::OnExit( CCharacter* _Character )
{
	//if (!m_pDeer) 
	//{
	//	m_pDeer = dynamic_cast<CDeer*> (_Character);
	//}


}

bool CDeerHitState::OnMessage( CCharacter* _Character, const STelegram& _Telegram )
{
	return false;
}

// Devuelve el tiempo, la duración
void CDeerHitState::PlayRandomSound( void )
{
	int l_Num = BoostRandomHelper::GetInt(1,4);
	if ( l_Num == 1 )
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerPain1");
		m_SoundDuration = 1.2f;
	}
	else if ( l_Num == 2)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerPain2");
		m_SoundDuration = 0.56f;
	}
	else if ( l_Num == 3)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerPain3");
		m_SoundDuration = 2.0f;
	}
	else if ( l_Num == 4)
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_DeerPain4");
		m_SoundDuration = 1.4f;
	}
}