#include "PlayerAnimationDeathState.h"
#include "Characters\StatesDefs.h"
#include "Characters\Player\Player.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CPlayerAnimationDeathState::CPlayerAnimationDeathState( CCharacter* _pCharacter )
	: CState	(_pCharacter, "CPlayerAnimationDeathState" )
	, m_pPlayer	( NULL )
{
}

CPlayerAnimationDeathState::CPlayerAnimationDeathState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter,_Name)
	, m_pPlayer	( NULL )
{}


CPlayerAnimationDeathState::~CPlayerAnimationDeathState(void)
{
	if ( m_pPlayer )
	{
		int l_Num = m_pPlayer->GetAnimationID(PLAYER_DEATH_STATE);
		m_pPlayer->GetAnimatedModel()->RemoveAction(l_Num);
	}

	m_pPlayer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CPlayerAnimationDeathState::Execute( CCharacter*, float _ElapsedTime )
{
}

void CPlayerAnimationDeathState::OnEnter( CCharacter* _pCharacter )
{
	if ( !m_pPlayer ) 
	{
		// Almacenamos el enemigo
		m_pPlayer = dynamic_cast<CPlayer*> (_pCharacter);
	}

	int l_Num = m_pPlayer->GetAnimationID(PLAYER_DEATH_STATE);
	m_pPlayer->GetAnimatedModel()->ExecuteAction( l_Num, 0.f, true );
}

void CPlayerAnimationDeathState::OnExit( CCharacter* _pCharacter )
{
	if ( !m_pPlayer ) 
	{
		m_pPlayer = dynamic_cast<CPlayer*> (_pCharacter);
	}

	if ( m_pPlayer )
	{
		int l_Num = m_pPlayer->GetAnimationID(PLAYER_DEATH_STATE);
		m_pPlayer->GetAnimatedModel()->RemoveAction(l_Num);
	}
}

bool CPlayerAnimationDeathState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
