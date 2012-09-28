#include "DeerIdleState.h"
#include "Utils\BoostRandomHelper.h"
#include "SoundManager.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"

#include "DeerPursuitState.h"
#include "DeerHitState.h"

#include "Characters\Enemies\Deer\AnimationStates\DeerIdleAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerRunAnimationState.h"
#include "Characters\Enemies\Deer\AnimationStates\DeerHitAnimationState.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerIdleState::CDeerIdleState( CCharacter* _pCharacter )
	: CState			(_pCharacter, "CDeerIdleState")
	, m_pDeer			( NULL )
	, m_AlreadyDetected	( false )
	, m_AlreadyChased	( false )
	, m_ActionStateCallback	( 0,1 )
{	  
}

CDeerIdleState::CDeerIdleState( CCharacter* _pCharacter, const std::string &_Name )
	: CState		(_pCharacter, _Name)
	, m_pDeer		( NULL )
	, m_AlreadyDetected	( false )
	, m_AlreadyChased	( false )
	, m_ActionStateCallback	( 0,1 )
{
}


CDeerIdleState::~CDeerIdleState(void)
{
	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerIdleState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}
	
	m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetIdleAnimationState());
	m_AlreadyDetected	= false;
	m_AlreadyChased		= false;

	// Esta clase la uso como gestor de sonido para ver cuando debo enviar otro sonido
	float l_Tiempo = BoostRandomHelper::GetFloat(2.f, 4.f);
	m_ActionStateCallback.InitAction(0.f, l_Tiempo );
	m_ActionStateCallback.StartAction();


	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = DEER_IDLE_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pDeer->GetName().c_str(), l_State );
		}
	#endif
}

void CDeerIdleState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	// Si debo perseguir al player
	if ( m_pDeer->IsPlayerChased() ) 
	{
		CORE->GetSoundManager()->PlayEvent( "Stop_EFX_Deer_Enemy_Detected" );
		
		m_pDeer->GetLogicFSM()->ChangeState( m_pDeer->GetPursuitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetRunAnimationState());
		return;
	}

	// Gestiono sonidos si lo detecto
	if ( m_pDeer->IsPlayerDetected() )
	{
		if ( m_ActionStateCallback.IsActionStarted() )
		{
			// Miramos si ya está lanzado el sonido
			if ( !m_AlreadyDetected && m_pDeer->IsPlayerDetected() ) 
			{
				//if ( m_IdleWarningSounds )
					CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Deer_Enemy_Detected" );
				/*else
					CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_thread" );*/

				m_AlreadyDetected = true;
			}

			// Si finalizó el tiempo --> finaliza el sonido
			if ( m_ActionStateCallback.IsActionFinished() )
			{
				m_ActionStateCallback.InitAction();
			}
			else
			{
				// Actualizo el gestor de sonido de este estado
				m_ActionStateCallback.Update(_ElapsedTime);
			}
		}
		else
		{
			m_ActionStateCallback.StartAction();
			m_AlreadyDetected = false;
			//m_IdleWarningSounds = !m_IdleWarningSounds;
		}
	}
	else
	{
		CORE->GetSoundManager()->PlayEvent(_pCharacter->GetSpeakerName(), "Stop_EFX_Deer_Enemy_Detected");
		m_AlreadyDetected = false;
	}

	// Reseteamos la velocidad del enemigo
	m_pDeer->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pDeer->MoveTo2(_pCharacter->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CDeerIdleState::OnExit( CCharacter* _pCharacter )
{
	if (!_pCharacter) 
		return;

	CORE->GetSoundManager()->PlayEvent("Stop_EFX_Deer_Enemy_Detected");
}

bool CDeerIdleState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pDeer) 
		{
			m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
		}

		m_pDeer->RestLife(50); 
		m_pDeer->GetLogicFSM()->ChangeState(m_pDeer->GetHitState());
		m_pDeer->GetGraphicFSM()->ChangeState(m_pDeer->GetHitAnimationState());
		return true;
	}

	return false;
}