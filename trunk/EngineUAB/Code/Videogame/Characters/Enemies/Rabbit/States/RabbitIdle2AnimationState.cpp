#include "RabbitIdle2AnimationState.h"
#include "Characters\StatesDefs.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitIdle2AnimationState::CRabbitIdle2AnimationState( void )
	: CState	("CRabbitIdle2AnimationState")
	, m_pRabbit	( NULL )
{
}

CRabbitIdle2AnimationState::CRabbitIdle2AnimationState( const std::string &_Name )
	: CState	(_Name)
	, m_pRabbit	( NULL )
{}


CRabbitIdle2AnimationState::~CRabbitIdle2AnimationState(void)
{
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitIdle2AnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitIdle2AnimationState::OnEnter( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_Character);

		int l_Num = _Character->GetAnimationID(IDLE2_STATE);
		m_pRabbit->GetAnimatedModel()->BlendCycle( l_Num, 0.3f );
	}
}

void CRabbitIdle2AnimationState::OnExit( CCharacter* _Character )
{
	if ( !_Character == NULL ) 
	{
		int l_Num = _Character->GetAnimationID(IDLE2_STATE);
		m_pRabbit->GetAnimatedModel()->ClearCycle( l_Num, 0.3f );
	}
}

bool CRabbitIdle2AnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
