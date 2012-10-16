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
#include "Characters\Player\Player.h"

#include "WolfHitState.h"
#include "WolfIdleState.h"

#include "Characters\Enemies\Wolf\AnimationStates\WolfHowlEnemiesAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfIdleAnimationState.h"
#include "Characters\Enemies\Wolf\AnimationStates\WolfHitAnimationState.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#include "Callbacks\Animation\AnimationCallback.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Steering Behaviors\SteeringBehaviorsSeetingsManager.h"

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
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_wind1" );

	m_pAnimationCallback->Init();
	m_pAnimationCallback->StartAnimation();

	//m_SoundDuration = m_pWolf->GetAnimatedModel()->GetCurrentAnimationDuration(WOLF_HOWL_ENEMIES_STATE);
	//m_ActionStateCallback.InitAction(0, m_SoundDuration);
	//m_ActionStateCallback.StartAction();

	//CORE->GetParticleEmitterManager()->GetResource("DeepSnow")->GetParticleEmitterInstance(GetName() + "_DeepSnow")->SetActive(true);
	CORE->GetParticleEmitterManager()->GetResource("DeepSnow")->GetParticleEmitterInstance(GetName() + "_DeepSnow")->EjectParticles();
	CORE->GetParticleEmitterManager()->GetResource("DeepSnow")->GetParticleEmitterInstance(GetName() + "_DeepSnow")->SetPosition(m_pWolf->GetPosition());
	
	// Ya lo hemos ejecutado y ahora lo desactivamos
	m_pWolf->SetCanHowlForEnemies(false);

	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	CPlayer * l_Player = dynamic_cast<CPlayer *> (l_Process->GetCharactersManager()->GetPlayer());
	l_Player->SetLocked(true);
	l_Player->GetLogicFSM()->ChangeState(l_Player->GetLogicState("idle"));
	l_Player->GetGraphicFSM()->ChangeState(l_Player->GetAnimationState("animidle"));
	l_Process->GetCharactersManager()->SetAllEnemiesInIdle();
	l_Process->GetCharactersManager()->LockAllActiveEnemies();
	

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
			ShowEnemiesToHelp();

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
	CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Stop_EFX_wind1" );

	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	CPlayer * l_Player = dynamic_cast<CPlayer *> (l_Process->GetCharactersManager()->GetPlayer());
	l_Player->SetLocked(false);
	l_Process->GetCharactersManager()->UnlockAllActiveEnemies();

	CORE->GetParticleEmitterManager()->GetResource("DeepSnow")->GetParticleEmitterInstance(GetName() + "_DeepSnow")->StopEjectParticles();
	//CORE->GetParticleEmitterManager()->GetResource("DeepSnow")->GetParticleEmitterInstance(GetName() + "_DeepSnow")->SetActive(false);
}

bool CWolfHowlEnemiesState::OnMessage( CCharacter* _pCharacter, const STelegram& _Telegram )
{
	if (!m_pWolf) 
	{
		m_pWolf = dynamic_cast<CWolf*> (_pCharacter);
	}
	return m_pWolf->CallHitState(_pCharacter, _Telegram);
}

void CWolfHowlEnemiesState::ShowEnemiesToHelp(void)
{
	CGameProcess * l_Process = dynamic_cast<CGameProcess*> (CORE->GetProcess());
	CCharactersManager * l_CM = l_Process->GetCharactersManager();
	Vect3f l_InitialPosition = m_pWolf->GetPosition();							// Almaceno la posición del wolf que hará de lider a partir de la qual haré la formación
	
	int l_Min = CORE->GetSteeringBehaviourSettingsManager()->GetMinNumberEnemiesCanHelp();
	int l_Max = CORE->GetSteeringBehaviourSettingsManager()->GetMaxNumberEnemiesCanHelp();

	uint16 l_TotalEnemies = BoostRandomHelper::GetInt(l_Min, l_Max);			// Obtengo un nº aleatorio de enemigos 
	float	l_DegreesToSetEnemies = 120.f / l_TotalEnemies + 1;					// Con esto tengo los ángulos que tengo que aplicar para colocar cada enemigo
	
	Vect3f l_Front  = m_pWolf->GetFront(); 
	Vect3f l_OriginalFront = l_Front; 
	l_Front.Normalize();
	float l_Angle = l_OriginalFront.xzToAngle();
	l_Angle -= ePI2f;
	float l_AngleGraus = mathUtils::Rad2Deg(l_Angle);
	l_Front.RotateY(l_Angle);

	// Por cada X grados vamos a meter un enemigo segun el random anterior
	for ( float i = l_DegreesToSetEnemies; i < 120.f; i += l_DegreesToSetEnemies ) 
	{
		float l_DistanceToSet = BoostRandomHelper::GetFloat(2.f, 10.f);		// Obtenemos una distancia que aplicaremos dependiendo del ángulo
		//Vect3f l_Front  = m_pWolf->GetFront(); 
		l_Front.RotateY(mathUtils::Deg2Rad( l_DegreesToSetEnemies));

		Vect3f l_FinalPosition  = Vect3f( l_InitialPosition.x + l_Front.x * l_DistanceToSet, l_InitialPosition.y + m_pWolf->GetProperties()->GetHeightController() , l_InitialPosition.z + l_Front.z * l_DistanceToSet);
	
		CCharacter* l_Character = l_CM->GetCharacterById(m_DynamicEnemyIndex);
		l_Character->MoveCharacter(l_FinalPosition);
		l_Character->Appearance();
		l_Character->SetEnable(true);
		m_DynamicEnemyIndex++;
	}
}