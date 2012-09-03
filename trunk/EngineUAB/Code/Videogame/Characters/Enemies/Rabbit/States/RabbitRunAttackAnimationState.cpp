#include "RabbitRunAttackAnimationState.h"
#include "Characters\StatesDefs.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitRunAttackAnimationState::CRabbitRunAttackAnimationState( void )
{
}

CRabbitRunAttackAnimationState::CRabbitRunAttackAnimationState( const std::string &_Name )
	: CState(_Name)
{
}

CRabbitRunAttackAnimationState::~CRabbitRunAttackAnimationState( void )
{
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitRunAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitRunAttackAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(RUN_ATTACK_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitRunAttackAnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(RUN_ATTACK_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitRunAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
