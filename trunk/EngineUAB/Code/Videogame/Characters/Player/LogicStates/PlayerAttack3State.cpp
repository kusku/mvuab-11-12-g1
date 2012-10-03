#include "PlayerAttack3State.h"
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
#include "Core.h"
#include "Base.h"

#include "Steering Behaviors\SteeringEntity.h"
#include "Characters\StatesDefs.h"
#include "Logger\Logger.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayerAttack3State::CPlayerAttack3State( CCharacter * _pCharacter, const std::string &_Name )
	: CState(_pCharacter, _Name)
	, m_bFirstUpdate(true)
	, m_fMaxVelocityMovement( _pCharacter->GetProperties()->GetMaxSpeed() )
	, m_fCurrentVelocityMovement( _pCharacter->GetProperties()->GetMaxSpeed() )
	, m_fAccelerationMovement( _pCharacter->GetProperties()->GetAccelerationAttack3() )
	, m_fAttackDistance( _pCharacter->GetProperties()->GetAttackDistance() )
	, m_fAttackAngle( mathUtils::Deg2Rad( _pCharacter->GetProperties()->GetAttackAngle() ) )
	, m_fAttackYaw(0.f)
{
	m_pCallback				= static_cast<CGameProcess*>(CORE->GetProcess())->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),"attack3");
	m_pParticleEmitter	= GetParticleEmitterInstance( "SwordFinal", _pCharacter->GetName() + "SwordFinal");
}

CPlayerAttack3State::~CPlayerAttack3State()
{

}

void CPlayerAttack3State::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Attack 3");
	}
#endif

	//Lanza el sistema de partículas
	SetParticlePosition(_pCharacter);
	m_pParticleEmitter->EjectParticles();

	//Calcula el ángulo a moverse
	CAnimatedInstanceModel *l_pAnimatedModel = _pCharacter->GetAnimatedModel();

	float l_fYaw = _pCharacter->GetController()->GetYaw();

	if( !_pCharacter->GetLocked() )
	{
		CCharactersManager *l_pCharManager	= NULL;
		CCharacter *l_pEnemy				= NULL;
		float l_fAngle						= 0.f;

		l_pCharManager	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager();

		//Calcula el ángulo de a ir
		Vect3f l_Front	= l_pAnimatedModel->GetFront();
		bool l_bMovement= CalculateAngleMovement( _pCharacter, l_fYaw );

		if( l_bMovement )
		{
			//Calcula un ángulo de correción según la dirección marcada
			l_pEnemy = l_pCharManager->GetPlayerAngleCorrection(m_fAttackDistance, m_fAttackAngle, l_fAngle);
		}
		else
		{
			//Calcula el ángulo de correción para enfocar hacia un enemigo cercano
			l_pEnemy = l_pCharManager->IsPlayerNearEnemy(m_fAttackDistance);
		}

		//Si se ataca a un enemigo, calculamos el nuevo ángulo con la asistencia
		if( l_pEnemy != NULL )
		{
			if( !l_bMovement )
			{
				//Calcula el ángulo de giro
				Vect3f l_EnemyDir = l_pEnemy->GetPosition() - _pCharacter->GetPosition();
				l_EnemyDir.Normalize();
				l_EnemyDir.y	= 0.f;

				l_fAngle		= l_EnemyDir.Dot( l_Front );
				l_fAngle		= mathUtils::ACos<float>( l_fAngle );
			}

			//Mira como tiene que girar el player
			bool l_bInside	= _pCharacter->IsPointAtLeft( l_pEnemy->GetPosition(), l_Front );

			if( l_bInside )
			{
				l_fYaw	-= l_fAngle;
			}	
			else
			{
				l_fYaw	+= l_fAngle;
			}
		}
	}

	//Setea el ángulo calculado
	_pCharacter->GetController()->SetYaw( l_fYaw );

	m_fAttackYaw = l_fYaw;

	l_fYaw = -mathUtils::Rad2Deg(l_fYaw + FLOAT_PI_VALUE/2.f) + 180.f;
	l_pAnimatedModel->SetYaw( l_fYaw );

	//Inicia el callback
	m_pCallback->StartAnimation();

	//Establece los valores para la ejecución
	m_fCurrentVelocityMovement = m_fMaxVelocityMovement;
	m_bFirstUpdate = true;
}

void CPlayerAttack3State::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( m_bFirstUpdate )
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_Sword");
		m_bFirstUpdate = false;
	}

	if( m_pCallback->IsAnimationFinished() )
	{
		CActionToInput *l_pInput = CORE->GetActionToInput();

		if( l_pInput->DoAction("HardAttackPlayer") )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack4") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack4") );
		}
		else if( l_pInput->DoAction("AttackPlayer") )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack1") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack1") );
		}
		else
		{
			if( static_cast<CGameProcess*>(CORE->GetProcess())->GetTimeBetweenClicks() < 0.2f )
			{
				if( l_pInput->DoAction("HardPreparedAttackPlayer") )
				{
					_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack4") );
					_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack4") );
				}
				else
				{
					_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack1") );
					_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack1") );
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
		l_Dir		= Vect3f( mathUtils::Cos<float>( m_fAttackYaw ), 0.f, mathUtils::Sin<float>( m_fAttackYaw ) );
	}

	//Aplica la velocidad al movimiento
	l_Dir = l_Dir * m_fCurrentVelocityMovement * _fElapsedTime;

	//Mueve el controller físico
	CPhysicController *l_pController = _pCharacter->GetController();
	l_pController->Move( l_Dir, _fElapsedTime );

	//Actualiza la velocidad de movimiento
	m_fCurrentVelocityMovement = m_fCurrentVelocityMovement + m_fAccelerationMovement * _fElapsedTime;
	if( m_fCurrentVelocityMovement < 0.f )
	{
		m_fCurrentVelocityMovement = 0.f;
	}

	//Actualiza las partículas
	SetParticlePosition(_pCharacter);
}

void CPlayerAttack3State::OnExit( CCharacter* _pCharacter )
{
	Vect3f l_Front			= _pCharacter->GetAnimatedModel()->GetFront();
	CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->SearchTargetEnemy(3.0f, FLOAT_PI_VALUE / 6.f, l_Front);

	if( l_pEnemy != NULL )
	{
		CORE->GetMessageDispatcher()->DispatchStateMessage(SEND_MSG_IMMEDIATELY, _pCharacter->GetID(), l_pEnemy->GetID(), Msg_Attack, NO_ADDITIONAL_INFO);
	}
}

bool CPlayerAttack3State::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
{
	if( _Message.Msg == Msg_Attack )
	{
		CRandom	l_Randomize;

		CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
		float l_fReceivedPain	= l_Randomize.getRandFloat( (float)(l_pEnemy->GetProperties()->GetStrong() / 2), (float)l_pEnemy->GetProperties()->GetStrong());
		float l_fPainToHit		= l_pEnemy->GetProperties()->GetStrong() * 0.95f;

		if( l_fReceivedPain >= l_fPainToHit )
		{
			static_cast<CPlayer*>(_pCharacter)->HitToPlayer();
		}

		return true;
	}
	else if( _Message.Msg == Msg_Push )
	{
		/*CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
		
		sDireccion * l_Info = (struct sDireccion *) _Message.ExtraInfo;
		_pCharacter->MoveTo2(l_Info->Direccion * 1.2f, l_Info->ElapsedTime);
		_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));*/
		LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerAttack3State::OnMessage -> PUSHED!!");
		return true;
	}

	return false;
}

bool CPlayerAttack3State::CalculateAngleMovement( CCharacter *_pCharacter, float &_fAngle )
{
	CActionToInput *l_pInput = CORE->GetActionToInput();
	bool l_bMove = false;			


	if( l_pInput->DoAction("MovePlayerUp") )
	{
		_fAngle = _pCharacter->GetYaw();
		if( l_pInput->DoAction("MovePlayerLeft") )
		{
			_fAngle += FLOAT_PI_VALUE / 4.f;
		}
		else if( l_pInput->DoAction("MovePlayerRight") )
		{
			_fAngle -= FLOAT_PI_VALUE / 4.f;
		}

		l_bMove = true;
	}
	else if( l_pInput->DoAction("MovePlayerDown") )
	{
		_fAngle = _pCharacter->GetYaw();
		_fAngle -= FLOAT_PI_VALUE;
		if( l_pInput->DoAction("MovePlayerLeft") )
		{
			_fAngle -= FLOAT_PI_VALUE / 4.f;
		}
		else if( l_pInput->DoAction("MovePlayerRight") )
		{
			_fAngle += FLOAT_PI_VALUE / 4.f;
		}

		l_bMove = true;
	}
	else if( l_pInput->DoAction("MovePlayerLeft") )
	{
		_fAngle = _pCharacter->GetYaw();
		_fAngle += FLOAT_PI_VALUE / 2.f;

		l_bMove = true;
	}
	else if( l_pInput->DoAction("MovePlayerRight") )
	{
		_fAngle = _pCharacter->GetYaw();
		_fAngle -= FLOAT_PI_VALUE / 2.f;

		l_bMove = true;
	}

	return l_bMove;		
}

void CPlayerAttack3State::SetParticlePosition( CCharacter* _pCharacter )
{
	Vect3f l_Pos	= _pCharacter->GetPosition();
	l_Pos.y			+= 1.f;
	l_Pos			+= _pCharacter->GetAnimatedModel()->GetFront() * 1.7f;

	m_pParticleEmitter->SetPosition( l_Pos );
}