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
	, m_fMaxVelocityMovement(10.f)
	, m_fCurrentVelocityMovement(10.f)
	, m_fAccelerationMovement(-35.f)
	, m_fAttackYaw(0.f)
{
	m_pCallback			= static_cast<CGameProcess*>(CORE->GetProcess())->GetAnimationCallbackManager()->GetCallback(_pCharacter->GetName(),"attack2");
	m_pParticleEmitter	= CORE->GetParticleEmitterManager()->GetResource("SwordLeft");
}

CPlayerAttack2State::~CPlayerAttack2State()
{

}

void CPlayerAttack2State::OnEnter( CCharacter* _pCharacter )
{
#if defined _DEBUG
	if( CORE->IsDebugMode() )
	{
		CORE->GetDebugGUIManager()->GetDebugRender()->SetStateName("Attack 2");
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
		l_fYaw			= CalculateAngleMovement( _pCharacter, l_fYaw );

		//Calcula el ángulo de correción para enfocar hacia un enemigo cercano
		l_pEnemy		= l_pCharManager->IsPlayerNearEnemy(10.f);

		//Si se ataca a un enemigo, calculamos el nuevo ángulo con la asistencia
		if( l_pEnemy != NULL )
		{
			//Calcula el ángulo de giro
			Vect3f l_EnemyDir = l_pEnemy->GetPosition() - _pCharacter->GetPosition();
			l_EnemyDir.Normalize();
			l_EnemyDir.y = 0.f;

			l_fAngle	= l_EnemyDir.Dot( l_Front );
			l_fAngle		= mathUtils::ACos<float>( l_fAngle );

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

void CPlayerAttack2State::Execute( CCharacter* _pCharacter, float _fElapsedTime )
{
	if( m_bFirstUpdate )
	{
		CORE->GetSoundManager()->PlayEvent("Play_EFX_Sword");
		m_bFirstUpdate = false;
	}

	if( m_pCallback->IsAnimationFinished() )
	{
		if( CORE->GetActionToInput()->DoAction("AttackPlayer") )
		{
			_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack3") );
			_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack3") );
		}
		else
		{
			if( static_cast<CGameProcess*>(CORE->GetProcess())->GetTimeBetweenClicks() < 0.2f )
			{
				_pCharacter->GetLogicFSM()->ChangeState( _pCharacter->GetLogicState("attack3") );
				_pCharacter->GetGraphicFSM()->ChangeState( _pCharacter->GetAnimationState("animattack3") );
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

void CPlayerAttack2State::OnExit( CCharacter* _pCharacter  )
{
	Vect3f l_Front			= _pCharacter->GetAnimatedModel()->GetFront();
	CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->SearchTargetEnemy(3.0f, FLOAT_PI_VALUE / 6.f, l_Front);

	if( l_pEnemy != NULL )
	{
		CORE->GetMessageDispatcher()->DispatchStateMessage(SEND_MSG_IMMEDIATELY, _pCharacter->GetID(), l_pEnemy->GetID(), Msg_Attack, NO_ADDITIONAL_INFO);
	}
}

bool CPlayerAttack2State::OnMessage( CCharacter* _pCharacter, const STelegram& _Message )
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
		CCharacter *l_pEnemy	= static_cast<CGameProcess*>(CORE->GetProcess())->GetCharactersManager()->GetCharacterById(_Message.Sender);
		
		sDireccion * l_Info = (struct sDireccion *) _Message.ExtraInfo;
		_pCharacter->MoveTo2(l_Info->Direccion * 1.2f, l_Info->ElapsedTime);
		_pCharacter->GetSteeringEntity()->SetVelocity(Vect3f(0,0,0));
		LOGGER->AddNewLog(ELL_INFORMATION, "CPlayerAttack2State::OnMessage -> PUSHED!!");
		return true;
	}

	return false;
}

float CPlayerAttack2State::CalculateAngleMovement( CCharacter *_pCharacter, float _fAngle )
{
	CActionToInput *l_pInput = CORE->GetActionToInput();

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
	}
	else if( l_pInput->DoAction("MovePlayerLeft") )
	{
		_fAngle = _pCharacter->GetYaw();
		_fAngle += FLOAT_PI_VALUE / 2.f;
	}
	else if( l_pInput->DoAction("MovePlayerRight") )
	{
		_fAngle = _pCharacter->GetYaw();
		_fAngle -= FLOAT_PI_VALUE / 2.f;
	}

	return _fAngle;		
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