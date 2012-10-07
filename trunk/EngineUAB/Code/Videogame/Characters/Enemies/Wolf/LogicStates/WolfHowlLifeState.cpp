#include "WolfHowlLifeState.h"
#include "GameProcess.h"
#include "SoundManager.h"
#include "Utils\BoostRandomHelper.h"

// --- Per pintar l'estat enemic ---
#include "DebugGUIManager.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Core.h"
// ---------------------------------

#include "Characters\Character.h"
#include "Characters\CharacterManager.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Wolf\Wolf.h"

#include "WolfHitState.h"
#include "WolfIdleState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlLifeAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "Steering Behaviors\SteeringEntity.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CWolfHowlLifeState::CWolfHowlLifeState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CWolfHowlLifeState")
	, m_pWolf				( NULL )
	, m_ActionStateCallback	( 0,1 )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_HOWL_LIFE_STATE);
}

CWolfHowlLifeState::CWolfHowlLifeState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pWolf				( NULL )
	, m_ActionStateCallback	( 0,1 )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_HOWL_LIFE_STATE);
}


CWolfHowlLifeState::~CWolfHowlLifeState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfHowlLifeState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHowlLifeAnimationState());
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_Howl1" );

	m_pAnimationCallback->Init();
	m_pAnimationCallback->StartAnimation();

	//m_SoundDuration = m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_HOWL_LIFE_STATE);
	//m_ActionStateCallback.InitAction(0, m_SoundDuration);
	//m_ActionStateCallback.StartAction();

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = WOLF_HOWL_LIFE_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfHowlLifeState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	// Si se ha iniciado la animación
	/*if ( m_pAnimationCallback->IsAnimationStarted() ) 
	{*/
		// Compruebo si la animación a finalizado
		if ( m_pAnimationCallback->IsAnimationFinished() )
		{
			IncreaseLife();

			// Volvemos al reposo
			m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetIdleState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
		}
	//}
	
	// Reseteamos la velocidad del enemigo
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pWolf->MoveTo2(_pCharacter->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CWolfHowlLifeState::OnExit( CCharacter* _pCharacter )
{
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Stop_EFX_Wolf_Howl" );
}

bool CWolfHowlLifeState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		}

		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}
	return false;
}

void CWolfHowlLifeState::IncreaseLife(void)
{
	if (!m_pWolf)
		return;

	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	int l_CurrentLife = m_pWolf->GetProperties()->GetCurrentLife();
	l_CurrentLife += 50;
	m_pWolf->GetProperties()->SetCurrentLife(l_CurrentLife);
	
	// TODO : Falta augmentar la barra gràfica

}