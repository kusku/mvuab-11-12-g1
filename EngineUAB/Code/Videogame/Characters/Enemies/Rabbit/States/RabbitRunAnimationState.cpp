#include "RabbitRunAnimationState.h"
#include "Characters\StatesDefs.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitRunAnimationState::CRabbitRunAnimationState( void )
{
}

CRabbitRunAnimationState::CRabbitRunAnimationState( const std::string &_Name )
	: CState(_Name)
{
}

CRabbitRunAnimationState::~CRabbitRunAnimationState( void )
{
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitRunAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitRunAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(RUN_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitRunAnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(RUN_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitRunAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
