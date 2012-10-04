#include "WolfHowlEnemiesState.h"
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

#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlEnemiesAnimationState.h"
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
CWolfHowlEnemiesState::CWolfHowlEnemiesState( CCharacter* _pCharacter )
	: CState				(_pCharacter, "CWolfHowlEnemiesState")
	, m_pWolf				( NULL )
	, m_ActionStateCallback	( 0,1 )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_HOWL_ENEMIES_STATE);
}

CWolfHowlEnemiesState::CWolfHowlEnemiesState( CCharacter* _pCharacter, const std::string &_Name )
	: CState				(_pCharacter, _Name)
	, m_pWolf				( NULL )
	, m_ActionStateCallback	( 0,1 )
	, m_pAnimationCallback	( NULL )
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	m_pAnimationCallback = l_Process->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),WOLF_HOWL_ENEMIES_STATE);
}


CWolfHowlEnemiesState::~CWolfHowlEnemiesState(void)
{
	m_pWolf = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CWolfHowlEnemiesState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	
	m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHowlEnemiesAnimationState());
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Wolf_Howl2" );

	m_pAnimationCallback->Init();
	m_pAnimationCallback->StartAnimation();

	//m_SoundDuration = m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_HOWL_ENEMIES_STATE);
	//m_ActionStateCallback.InitAction(0, m_SoundDuration);
	//m_ActionStateCallback.StartAction();

	#if defined _DEBUG
		if( CORE->IsDebugMode() )
		{
			std::string l_State = WOLF_HOWL_ENEMIES_STATE;
			CORE->GetDebugGUIManager()->GetDebugRender()->AddEnemyStateName(m_pWolf->GetName().c_str(), l_State );
		}
	#endif
}

void CWolfHowlEnemiesState::Execute( CCharacter* _pCharacter, float _ElapsedTime )
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
			CreateEnemiesToHelp();

			// Volvemos al reposo
			m_pWolf->GetLogicFSM()->ChangeState( m_pWolf->GetIdleState());
			m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetIdleAnimationState());
		}
	//}
	
	// Reseteamos la velocidad del enemigo
	m_pWolf->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	m_pWolf->MoveTo2(_pCharacter->GetSteeringEntity()->GetVelocity(), _ElapsedTime);
}


void CWolfHowlEnemiesState::OnExit( CCharacter* _pCharacter )
{
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Stop_EFX_Wolf_Howl" );
}

bool CWolfHowlEnemiesState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if ( _Telegram.Msg == Msg_Attack ) 
	{
		if (!m_pWolf) 
		{
			m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
		}

		m_pWolf->RestLife(50); 
		m_pWolf->GetLogicFSM()->ChangeState(m_pWolf->GetHitState());
		m_pWolf->GetGraphicFSM()->ChangeState(m_pWolf->GetHitAnimationState());
		return true;
	}
	return false;
}

void CWolfHowlEnemiesState::CreateEnemiesToHelp(void)
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	Vect3f l_InitialPosition = m_pWolf->GetPosition();							// Almaceno la posición del wolf que hará de lider a partir de la qual haré la formación
	
	uint16 l_TotalEnemies = BoostRandomHelper::GetInt(5, 10);					// Obtengo un nº aleatorio de enemigos 
	float	l_DegreesToSetEnemies = 180.f / l_TotalEnemies + 1;					// Con esto tengo los ángulos que tengo que aplicar para colocar cada enemigo
		
	// Por cada X grados vamos a meter un enemigo segun el random anterior
	for ( float i = l_DegreesToSetEnemies; i < 120.f; i += l_DegreesToSetEnemies ) 
	{
		float l_DistanceToSet = BoostRandomHelper::GetFloat(4.f, 7.f);		// Obtenemos una distancia que aplicaremos dependiendo del ángulo
		Vect3f l_Front  = m_pWolf->GetFront(); 
		l_Front.RotateY(mathUtils::Deg2Rad( l_DegreesToSetEnemies));

		Vect3f l_OriginalFront = l_Front;
		l_Front.Normalize();
		Vect3f l_FinalPosition  = Vect3f( l_InitialPosition.x + l_Front.x * l_DistanceToSet, l_InitialPosition.y, l_InitialPosition.z + l_Front.z * l_DistanceToSet);
	
		// Ahora que tenemos la posición metemos el enemigo
		/*for ( size_t i = 1; i <= 2; i++ )
		{*/

			CCharacter *l_Character = l_Process->GetCharactersManager()->CreateEnemy(l_FinalPosition);
			l_Character->Appearance();
			l_Process->GetCharactersManager()->SaveDynamicCharacterCreated(l_Character->GetName());
		
		//}
	}
}