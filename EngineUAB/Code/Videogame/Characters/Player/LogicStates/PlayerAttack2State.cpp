#include "PlayerAttack2State.h"
#include "StatesMachine\Telegram.h"
#include "ActionToInput.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "Particles\ParticleEmitter.h"
#include "Particles\ParticleEmitterManager.h"
#include "DebugInfo\DebugRender.h"
#include "DebugGUIManager.h"
#include "PhysicController.h"
#include "StatesMachine\MessageDispatcher.h"
#include "SoundManager.h"
#include "GameProcess.h"
#include "EngineProcess.h"
#include "Utils\Random.h"
#include "Characters\Player\Player.h"
#include "Characters\Player\LogicStates\PlayerHitState.h"

#include "Core.h"
#include "Base.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Characters\StatesDefs.h"
#include "Logger\Logger.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAttack2State::CPlayerAttack2State( CCharacter * _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
	, m_bFirstUpdate(true)
	, m_fMaxVelocityMovement( _pCharacter->GetProperties()->GetMaxSpeed() )
	, m_fCurrentVelocityMovement( _pCharacter->GetProperties()->GetMaxSpeed() )
	, m_fAccelerationMovement( _pCharacter->GetProperties()->GetAccelerationAttack2() )
	, m_fAttackDistance( _pCharacter->GetProperties()->GetAttackDistance() )
	, m_fDetectionDistance( _pCharacter->GetProperties()->GetDetectionDistance() )
	, m_fAttackAngle( mathUtils::Deg2Rad( _pCharacter->GetProperties()->GetAttackAngle() ) )
	, m_fDetectionAngle( mathUtils::Deg2Rad( _pCharacter->GetProperties()->GetDetectionAngle() ) )
	, m_fAttackYaw(0.f)
	, m_pTarget(NULL)
{
	m_pInput			= CORE->GetActionToInput();
	m_pProcess			= static_cast<CGameProcess*>(CORE->GetProcess());
	m_pCallback			= m_pProcess->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),"attack2");
	m_pParticleEmitter	= GetParticleEmitterInstance( "SwordMedium", _pCharacter->GetName() + "SwordMedium");
}

CPlayerAttack2State::~CPlayerAttack2State()
{
	m_pProcess			= NULL;
	m_pCallback			= NULL;
	m_pParticleEmitter	= NULL;
	m_pInput			= NULL;
	m_pTarget			= NULL;
}

void CPlayerAttack2State::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Attack 2");
	}
#endif

	//Lanza el sistema de part�culas
	SetParticlePosition(_pCharacter);
	m_pParticleEmitter->EjectParticles();

	float l_fYaw = static_cast<CPlayer*>(_pCharacter)->CalculateAttackYaw(m_fDetectionDistance, m_fDetectionAngle, m_pTarget);

	//Setea el �ngulo calculado
	_pCharacter->GetController()->SetYaw( l_fYaw );

	m_fAttackYaw = l_fYaw;

	l_fYaw = -mathUtils::Rad2Deg(l_fYaw + FLOAT_PI_VALUE/2.f) + 180.f;
	_pCharacter->GetAnimatedModel()->SetYaw( l_fYaw );

	//Inicia el callback
	m_pCallback->StartAnimation();

	//Establece los valores para la ejecuci�n
	m_fCurrentVelocityMovement = m_fMaxVelocityMovement;
	_pCharacter->GetSteeringEntity()->SetVelocity(m_fCurrentVelocityMovement);
	m_bFirstUpdate = true;

	//CORE->GetSoundManager()->PlayEvent( _pCharacter->GetSpeakerName(), "Play_EFX_Caperucita_attack1");
}

void CPlayerAttack2State::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( m_bFirstUpdate )
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_Sword");
		m_bFirstUpdate = false;
	}

	if( m_pCallback->IsAnimationFinished() )
	{
		if( m_pInput->DoAction("HardAttackPlayer") )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack6") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack6") );
		}
		else if( m_pInput->DoAction("AttackPlayer") )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack3") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack3") );
		}
		else
		{
			if( m_pProcess->GetTimeBetweenClicks() < 0.2f )
			{
				if( m_pInput->DoAction("HardPreparedAttackPlayer") )
				{
					_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack6") );
					_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack6") );
				}
				else
				{
					_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack3") );
					_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack3") );
				}
			}
			else
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("idle") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animidle") );
			}
		}
	}

	//Movimiento del player hacia adelante
	Vect3f l_Dir	= v3fZERO;

	if( !_pCharacter->GetLocked() )
	{
		if( m_pTarget != NULL )
		{
			if( m_pTarget->GetPosition().Distance( _pCharacter->GetPosition() ) > m_fAttackDistance )
			{
				l_Dir = Vect3f( mathUtils::Cos<float>( m_fAttackYaw ), 0.f, mathUtils::Sin<float>( m_fAttackYaw ) );
			}
		}
		else
		{
			l_Dir = Vect3f( mathUtils::Cos<float>( m_fAttackYaw ), 0.f, mathUtils::Sin<float>( m_fAttackYaw ) );
		}
	}

	//Aplica la velocidad al movimiento
	l_Dir = l_Dir * m_fCurrentVelocityMovement * _fElapsedTime;

	//Mueve el controller f�sico
	CPhysicController *l_pController = _pCharacter->GetController();
	l_pController->Move( l_Dir, _fElapsedTime );

	//Actualiza la velocidad de movimiento
	m_fCurrentVelocityMovement = m_fCurrentVelocityMovement + m_fAccelerationMovement * _fElapsedTime;
	if( m_fCurrentVelocityMovement < 0.f )
	{
		m_fCurrentVelocityMovement = 0.f;
	}

	//Actualiza las part�culas
	SetParticlePosition(_pCharacter);
}

void CPlayerAttack2State::OnExit( CCharacter* _pCharacter  )
{
	Vect3f l_Front			= _pCharacter->GetAnimatedModel()->GetFront();
	CCharacter *l_pEnemy	= m_pProcess->GetCharactersManager()->SearchTargetEnemy(m_fAttackDistance, m_fAttackAngle, l_Front);

	if( l_pEnemy != NULL )
	{
		CORE->GetMessageDispatcher()->DispatchStateMessage(SEND_MSG_IMMEDIATELY, _pCharacter->GetID(), l_pEnemy->GetID(), Msg_Attack, NO_ADDITIONAL_INFO);
	}
}

bool CPlayerAttack2State::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	CPlayer * l_pPlayer = dynamic_cast<CPlayer*> (_pCharacter);
	
	//if( _Message.Msg == Msg_Attack )
	//{
	//	CRandom	l_Randomize;

	//	CCharacter *l_pEnemy	= m_pProcess->GetCharactersManager()->GetCharacterById(_Message.Sender);
	//	float l_fReceivedPain	= l_Randomize.getRandFloat( (float)(l_pEnemy->GetProperties()->GetStrong() / 2), (float)l_pEnemy->GetProperties()->GetStrong());
	//	float l_fPainToHit		= l_pEnemy->GetProperties()->GetStrong() * 0.95f;

	//	if( l_fReceivedPain >= l_fPainToHit )
	//	{
	//		return l_pPlayer->CallHitState(_pCharacter, _Message);
	//	}

	//	return true;
	//}
	//else if( _Message.Msg == Msg_Push )
	//{
	//	/*CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
	//	
	//	sDireccion * l_Info = (struct sDireccion *) _Message.ExtraInfo;
	//	_pCharacter->MoveTo2(l_Info->Direccion * 1.2f, l_Info->ElapsedTime);
	//	_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
	//	LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerAttack2State::OnMessage -> PUSHED!!");*/
	//	return l_pPlayer->CallHitState(_pCharacter, _Message);
	//}

	//return false;
	return l_pPlayer->CallHitState(_pCharacter, _Message);
}

void CPlayerAttack2State::SetParticlePosition( CCharacter* _pCharacter )
{	
	CAnimatedInstanceModel *l_pAnimatedModel = _pCharacter->GetAnimatedModel();

	Mat44f l_TransformMatrix		= m44fIDENTITY;
	Mat44f l_RotationMatrix			= m44fIDENTITY;
	Vect4f l_Rotation				= v3fZERO;
	Vect3f l_Translation			= v3fZERO;
	Mat44f l_AnimatedModelTransform = l_pAnimatedModel->GetTransform();

	l_pAnimatedModel->GetBonePosition("CHR_CAP L Hand", l_Translation);
	l_pAnimatedModel->GetBoneRotation("CHR_CAP L Hand", l_Rotation);

	l_TransformMatrix.Translate(l_Translation);
	l_RotationMatrix.SetFromQuaternion(l_Rotation);

	l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;

	m_pParticleEmitter->SetPosition( l_TransformMatrix.GetPos() );
}

void CPlayerAttack2State::GenerateImpact( void )
{
	//GetParticleEmitterFireSwordBlur")->EjectParticles();
	//GetParticleEmitterFireSwordSmoke")->EjectParticles();
}

void CPlayerAttack2State::UpdateImpact( CCharacter* _pCharacter )
{
	//SetParticlePosition(_pCharacter, "FireSwordBlur", "CHR_CAP L Hand" );
	//SetParticlePosition(_pCharacter, "FireSwordSmoke", "CHR_CAP L Hand" );

	//Vect3f l_Pos = _pCharacter->GetPosition() + _pCharacter->GetFront();
	//l_Pos.y += _pCharacter->GetProperties()->GetHeightController();
	//SetParticlePosition(_pCharacter, "BloodSplash", "", l_Pos );
	//SetParticlePosition(_pCharacter, "Impact", "", l_Pos );
	//SetParticlePosition(_pCharacter, "Streaks", "", l_Pos );

	////l_Pos.z -= 10.f;
	//SetParticlePosition(_pCharacter, "ExpandWave", "", l_Pos );
	
}
