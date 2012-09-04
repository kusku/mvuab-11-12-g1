#include "RabbitWalkAnimationState.h"
#include "Characters\StatesDefs.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitWalkAnimationState::CRabbitWalkAnimationState( void )
	: CState ("CRabbitWalkAnimationState")
{
}

CRabbitWalkAnimationState::CRabbitWalkAnimationState( const std::string &_Name )
	: CState(_Name)
{
}

CRabbitWalkAnimationState::~CRabbitWalkAnimationState( void )
{
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitWalkAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitWalkAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(WALK_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitWalkAnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(WALK_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitWalkAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
