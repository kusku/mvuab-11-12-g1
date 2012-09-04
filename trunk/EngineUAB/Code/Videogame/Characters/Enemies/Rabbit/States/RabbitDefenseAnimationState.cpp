#include "RabbitDefenseAnimationState.h"
#include "Characters\StatesDefs.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitDefenseAnimationState::CRabbitDefenseAnimationState( void )
	: CState("CRabbitDefenseAnimationState")
{
}

CRabbitDefenseAnimationState::CRabbitDefenseAnimationState( const std::string &_Name )
	: CState(_Name)
{}


CRabbitDefenseAnimationState::~CRabbitDefenseAnimationState(void)
{
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitDefenseAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitDefenseAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(DEFENSE_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitDefenseAnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(DEFENSE_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitDefenseAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
