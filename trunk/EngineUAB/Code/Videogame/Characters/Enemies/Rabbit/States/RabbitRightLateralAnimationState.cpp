#include "RabbitRightLateralAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitRightLateralAnimationState::CRabbitRightLateralAnimationState( void )
	: CState	("CRabbitRightLateralAnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitRightLateralAnimationState::CRabbitRightLateralAnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CRabbitRightLateralAnimationState::~CRabbitRightLateralAnimationState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitRightLateralAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitRightLateralAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(RIGHT_LATERAL_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitRightLateralAnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(RIGHT_LATERAL_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitRightLateralAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
