#include "RabbitJumpAnimationState.h"
#include "Characters\StatesDefs.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitJumpAnimationState::CRabbitJumpAnimationState( void )
	: CState()
{
}

CRabbitJumpAnimationState::CRabbitJumpAnimationState( const std::string &_Name )
	: CState(_Name)
{}


CRabbitJumpAnimationState::~CRabbitJumpAnimationState(void)
{
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitJumpAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitJumpAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(JUMP_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitJumpAnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(JUMP_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitJumpAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
