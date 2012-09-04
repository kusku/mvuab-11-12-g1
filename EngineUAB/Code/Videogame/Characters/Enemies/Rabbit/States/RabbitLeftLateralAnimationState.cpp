#include "RabbitLeftLateralAnimationState.h"
#include "Characters\StatesDefs.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitLeftLateralAnimationState::CRabbitLeftLateralAnimationState( void )
	: CState("CRabbitLeftLateralAnimationState")
{
}

CRabbitLeftLateralAnimationState::CRabbitLeftLateralAnimationState( const std::string &_Name )
	: CState(_Name)
{}


CRabbitLeftLateralAnimationState::~CRabbitLeftLateralAnimationState(void)
{
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitLeftLateralAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitLeftLateralAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(LEFT_LATERAL_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitLeftLateralAnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(LEFT_LATERAL_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitLeftLateralAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
