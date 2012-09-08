#include "Player.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "Callbacks\Animation\AnimationCallback.h"
#include "Characters\Player\LogicStates\PlayerAttackState.h"
#include "Characters\Player\LogicStates\PlayerAttack2State.h"
#include "Characters\Player\LogicStates\PlayerAttack3State.h"
#include "Characters\Player\LogicStates\PlayerHitState.h"
#include "Characters\Player\LogicStates\PlayerIdleState.h"
#include "Characters\Player\LogicStates\PlayerJumpState.h"
#include "Characters\Player\LogicStates\PlayerRunState.h"
#include "Characters\Player\LogicStates\PlayerDefenseState.h"
#include "Characters\Player\LogicStates\PlayerTargetAttackState.h"
#include "Characters\Player\LogicStates\PlayerTargetAttack2State.h"
#include "Characters\Player\LogicStates\PlayerTargetAttack3State.h"
#include "Characters\Player\LogicStates\PlayerTargetRunState.h"
#include "Characters\Player\AnimationStates\PlayerAnimationAttackState.h"
#include "Characters\Player\AnimationStates\PlayerAnimationAttack2State.h"
#include "Characters\Player\AnimationStates\PlayerAnimationAttack3State.h"
#include "Characters\Player\AnimationStates\PlayerAnimationDefenseState.h"
#include "Characters\Player\AnimationStates\PlayerAnimationHitState.h"
#include "Characters\Player\AnimationStates\PlayerAnimationIdleState.h"
#include "Characters\Player\AnimationStates\PlayerAnimationJumpState.h"
#include "Characters\Player\AnimationStates\PlayerAnimationRunState.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "Steering Behaviors\SteeringEntity.h"
#include "StatesMachine\StateMachine.h"
#include "Scripting\ScriptManager.h"
#include "Cameras\ThPSCharacterCamera.h"
#include "PhysicController.h"
#include "ActionToInput.h"
#include "EngineProcess.h"
#include "GameProcess.h"
#include "Math\MathTypes.h"
#include "Core.h"
#include "Base.h"

#if defined _DEBUG
#include "Memory\MemLeaks.h"
#endif

CPlayer::CPlayer()
	: m_bIsTargetFixed(false)
	, m_fMovementZoom(5.7f)
	, m_fStaticZoom(5.2f)
	, m_fVelocityAdaptativeZoom(1.5f)
	, m_fDownZoom(2.0f)
	, m_fDistanceToDetectEnemy(20.f)
	, m_fVisibilityAngle(FLOAT_PI_VALUE / 3.f)
	, m_pCamera(NULL)
{
	m_fYaw		= 0.0f;
	m_fPitch	= -FLOAT_PI_VALUE / 10.f;
	m_fRoll		= 0.0f;

	m_bLocked	= false;
}

CPlayer::~CPlayer()
{
	TStateMap::iterator l_It	= m_LogicStatesMap.begin();
	TStateMap::iterator l_End	= m_LogicStatesMap.end();
	for(; l_It != l_End; ++l_It)
	{
		CHECKED_DELETE(l_It->second);
	}

	l_It	= m_AnimationStatesMap.begin();
	l_End	= m_AnimationStatesMap.end();
	for(; l_It != l_End; ++l_It)
	{
		CHECKED_DELETE(l_It->second);
	}

	m_LogicStatesMap.clear();
	m_AnimationStatesMap.clear();
}

bool CPlayer::Init()
{
	m_pCamera = static_cast<CGameProcess*>(CORE->GetProcess())->CreatePlayerCamera(1.0f, 10000.0f, m_fStaticZoom, 1.05f, 1.05f, "Caperucita");

	CreateCallbacks();
	CreateStates();

	TState *l_pState = GetLogicState("idle"); 
	m_pLogicStateMachine->SetCurrentState( l_pState );
	m_pLogicStateMachine->ChangeState( l_pState );

	l_pState = GetAnimationState("animidle");
	m_pGraphicStateMachine->SetCurrentState( l_pState );
	m_pGraphicStateMachine->ChangeState( l_pState );

	m_pCurrentAnimatedModel->SetYaw( (m_fYaw * FLOAT_PI_VALUE / 180.f ) + 90.f );
	m_pCurrentAnimatedModel->SetPitch( 0.f );

	return true;
}

void CPlayer::Update( float _ElapsedTime )
{
	if( !m_bLocked )
	{
		CActionToInput *l_pInput = CORE->GetActionToInput();

		//Coge el desplazamiento que hace el mouse
		float l_fDeltaMouse = l_pInput->DoActionMouse("PitchPlayer");
		
		//Calcula el ángulo pitch
		m_fPitch = m_fPitch - l_fDeltaMouse;
		if( m_fPitch > FLOAT_PI_VALUE / 12.f )
		{
			m_fPitch = FLOAT_PI_VALUE / 12.f;
		}
		else if( m_fPitch < -FLOAT_PI_VALUE / 6.f )
		{
			m_fPitch = -FLOAT_PI_VALUE / 6.f;
		}

		//Modifica el zoom de la cámara
		/*float l_fZoom = m_pCamera->GetZoom();
		if( m_pLogicStateMachine->GetCurrentState()->GetName() != "idle" )
		{
			if( (m_fMovementZoom - l_fZoom) > 0.0001f )
			{
				l_fZoom = l_fZoom + m_fVelocityAdaptativeZoom * _ElapsedTime;
			}
			else
			{
				l_fZoom = m_fMovementZoom;
			}
		}
		else
		{
			if( m_fPitch > -FLOAT_PI_VALUE / 10.f )
			{
				if( (l_fZoom - m_fStaticZoom) > 0.0001f )
				{
					l_fZoom = l_fZoom - 2.f * m_fVelocityAdaptativeZoom * _ElapsedTime;
				}
				else
				{
					l_fZoom = m_fMovementZoom;
				}
			}
			else
			{
				if( (l_fZoom - m_fStaticZoom) > 0.0001f )
				{
					l_fZoom = l_fZoom - m_fVelocityAdaptativeZoom * _ElapsedTime;
				}
				else
				{
					if( (m_fStaticZoom - l_fZoom) > 0.1f )
					{
						l_fZoom = l_fZoom + m_fVelocityAdaptativeZoom * _ElapsedTime;
					}
					else
					{
						l_fZoom = m_fStaticZoom;
					}
				}
			}
		}

		m_pCamera->SetZoom( l_fZoom );*/

		//Mira si el player se pone en defensa
		if( l_pInput->DoAction("DefensePlayer") )
		{
			m_pLogicStateMachine->ChangeState( GetLogicState("defense") );
			m_pGraphicStateMachine->ChangeState( GetAnimationState("animdefense") );
		}

		//Mira si el personaje ha muerto
		if( m_pProperties->GetLife() <= 0 )
		{
			SCRIPT->RunCode("change_to_game_over_gui_process()");
		}

		//Actualizamos los estados en caso de cambiar
		m_pLogicStateMachine->Update(_ElapsedTime);
		m_pGraphicStateMachine->Update(_ElapsedTime);

		//Actualiza la posición del objeto 3D
		m_Position = m_pController->GetPosition();
		m_Position.y -= m_pController->GetHeight();

		//Actualiza la posición del modelo animado
		Vect3f l_Pos	= m_Position;
		l_Pos.y			= l_Pos.y - m_pController->GetHeight() + m_pProperties->GetAnimationOffset();
		
		m_pCurrentAnimatedModel->SetPosition( l_Pos );
		m_pSteeringEntity->SetPosition( l_Pos );

		if( m_pSteeringEntity->GetVelocity().SquaredLength() > 0.00000001f )
		{
			Vect3f l_Heading = m_pSteeringEntity->GetVelocity();
			l_Heading.Normalize(1.f);
			
			m_pSteeringEntity->SetHeading( l_Heading );
			m_pSteeringEntity->SetSide( l_Heading.GetPerpendicular() );
		}

		static_cast<CGameProcess*>( CORE->GetProcess() )->GetPlayerCamera()->Update(_ElapsedTime);
	}
}

void CPlayer::HitToPlayer()
{
	//TODO: Ajustar el daño
	RestLife( 10 );
}

CCharacter* CPlayer::DetectEnemy()
{
	CGameProcess *l_pProcess			= static_cast<CGameProcess*>( CORE->GetProcess() );
	CCharactersManager *l_pCharManager	= l_pProcess->GetCharactersManager();

	Vect3f l_FrontCamera				= l_pProcess->GetPlayerCamera()->GetDirection();
	CCharacter *l_pEnemy				= l_pCharManager->SearchTargetEnemy(m_fDistanceToDetectEnemy, m_fVisibilityAngle, l_FrontCamera);

	l_pCharManager->SetPreviewTargetEnemy( l_pEnemy );

	return l_pEnemy;
}

void CPlayer::CreateStates()
{
	//Logic States
	m_LogicStatesMap["attack1"]			= new CPlayerAttackState("attack1");
	m_LogicStatesMap["attack2"]			= new CPlayerAttack2State("attack2");
	m_LogicStatesMap["attack3"]			= new CPlayerAttack3State("attack3");
	m_LogicStatesMap["hit"]				= new CPlayerHitState("hit");
	m_LogicStatesMap["idle"]			= new CPlayerIdleState("idle");
	m_LogicStatesMap["jump"]			= new CPlayerJumpState("jump");
	m_LogicStatesMap["run"]				= new CPlayerRunState("run");
	m_LogicStatesMap["defense"]			= new CPlayerDefenseState("defense");
	m_LogicStatesMap["targetattack1"]	= new CPlayerTargetAttackState("targetattack1");
	m_LogicStatesMap["targetattack2"]	= new CPlayerTargetAttack2State("targetattack2");
	m_LogicStatesMap["targetattack3"]	= new CPlayerTargetAttack3State("targetattack3");
	m_LogicStatesMap["targetrun"]		= new CPlayerTargetRunState("targetrun");

	//Animation States
	m_AnimationStatesMap["animattack1"]	= new CPlayerAnimationAttackState("animattack1");
	m_AnimationStatesMap["animattack2"]	= new CPlayerAnimationAttack2State("animattack2");
	m_AnimationStatesMap["animattack3"]	= new CPlayerAnimationAttack3State("animattack3");
	m_AnimationStatesMap["animdefense"]	= new CPlayerAnimationDefenseState("animdefense");
	m_AnimationStatesMap["animhit"]		= new CPlayerAnimationHitState("animhit");
	m_AnimationStatesMap["animidle"]	= new CPlayerAnimationIdleState("animidle");
	m_AnimationStatesMap["animjump"]	= new CPlayerAnimationJumpState("animjump");
	m_AnimationStatesMap["animrun"]		= new CPlayerAnimationRunState("animrun");
}

void CPlayer::CreateCallbacks()
{
	CAnimationCallbackManager *l_pCallbackManager = static_cast<CGameProcess*>(CORE->GetProcess())->GetAnimationCallbackManager();

	l_pCallbackManager->CreateCallback("attack1", m_pCurrentAnimatedModel);
	l_pCallbackManager->CreateCallback("attack2", m_pCurrentAnimatedModel);
	l_pCallbackManager->CreateCallback("attack3", m_pCurrentAnimatedModel);
	l_pCallbackManager->CreateCallback("hit", m_pCurrentAnimatedModel);
	l_pCallbackManager->CreateCallback("jump", m_pCurrentAnimatedModel);
}

// Jordi: Este método me permite hacer todo lo necesario cuando está muerto el caracter.
void CPlayer::BeDead()
{
	return;
}