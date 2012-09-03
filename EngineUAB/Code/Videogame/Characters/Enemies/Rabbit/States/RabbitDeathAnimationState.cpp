#include "RabbitDeathAnimationState.h"
#include "Characters\StatesDefs.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitDeathAnimationState::CRabbitDeathAnimationState( void )
	: CState()
{
}

CRabbitDeathAnimationState::CRabbitDeathAnimationState( const std::string &_Name )
	: CState(_Name)
{}


CRabbitDeathAnimationState::~CRabbitDeathAnimationState(void)
{
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitDeathAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitDeathAnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(DEATH_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitDeathAnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(DEATH_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitDeathAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
