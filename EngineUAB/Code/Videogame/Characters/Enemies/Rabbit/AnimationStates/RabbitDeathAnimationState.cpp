#include "RabbitDeathAnimationState.h"
#include "Characters\StatesDefs.h"
#include "StatesMachine\Telegram.h"
#include "Characters\Enemies\Rabbit\Rabbit.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CRabbitDeathAnimationState::CRabbitDeathAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CRabbitDeathAnimationState" )
	, m_pRabbit	( NULL )
{
}

CRabbitDeathAnimationState::CRabbitDeathAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pRabbit	( NULL )
{}


CRabbitDeathAnimationState::~CRabbitDeathAnimationState(void)
{
	if (m_pRabbit)
	{
		int l_Num = m_pRabbit->GetAnimationID(DEER_DEATH_STATE);
		m_pRabbit->GetAnimatedModel()->RemoveAction( l_Num );
	}
	m_pRabbit = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CRabbitDeathAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CRabbitDeathAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		// Almacenamos el enemigo
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	int l_Num = m_pRabbit->GetAnimationID(RABBIT_DEATH_STATE);
	m_pRabbit->GetAnimatedModel()->ExecuteAction( l_Num, 0.f, true );
}

void CRabbitDeathAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pRabbit ) 
	{
		m_pRabbit = dynamic_cast<CRabbit*> (_pCharacter);
	}

	if ( m_pRabbit )
	{
		int l_Num = m_pRabbit->GetAnimationID(RABBIT_DEATH_STATE);
		m_pRabbit->GetAnimatedModel()->RemoveAction(l_Num);
	}
}

bool CRabbitDeathAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
