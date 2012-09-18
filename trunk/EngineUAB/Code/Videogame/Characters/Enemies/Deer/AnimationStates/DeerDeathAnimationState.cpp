#include "DeerDeathAnimationState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Enemies\Deer\Deer.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerDeathAnimationState::CDeerDeathAnimationState( CCharacter* _pCharacter )
	: CState	(_pCharacter, "CDeerDeathAnimationState" )
	, m_pDeer	( NULL )
{
}

CDeerDeathAnimationState::CDeerDeathAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter,_Name)
	, m_pDeer	( NULL )
{}


CDeerDeathAnimationState::~CDeerDeathAnimationState(void)
{
	if ( m_pDeer )
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_DEATH_STATE);
		m_pDeer->GetAnimatedModel()->RemoveAction(l_Num);
	}

	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerDeathAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CDeerDeathAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pDeer ) 
	{
		// Almacenamos el enemigo
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	int l_Num = m_pDeer->GetAnimationID(DEER_DEATH_STATE);
	m_pDeer->GetAnimatedModel()->ExecuteAction( l_Num, 0.3f );
}

void CDeerDeathAnimationState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pDeer ) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	if ( m_pDeer )
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_DEATH_STATE);
		m_pDeer->GetAnimatedModel()->RemoveAction(l_Num);
	}
}

bool CDeerDeathAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
