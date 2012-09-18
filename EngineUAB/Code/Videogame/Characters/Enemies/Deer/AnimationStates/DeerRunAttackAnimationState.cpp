#include "DeerRunAttackAnimationState.h"
#include "Characters\StatesDefs.h"
#include "StatesMachine\Telegram.h"
#include "Characters\Enemies\Deer\Deer.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CDeerRunAttackAnimationState::CDeerRunAttackAnimationState( CCharacter* _pCharacter )
	: CState	( _pCharacter, "CDeerRunAttackAnimationState")
	, m_pDeer	( NULL )
{
}

CDeerRunAttackAnimationState::CDeerRunAttackAnimationState( CCharacter* _pCharacter, const std::string &_Name )
	: CState	(_pCharacter, _Name)
	, m_pDeer	( NULL )
{
}

CDeerRunAttackAnimationState::~CDeerRunAttackAnimationState( void )
{
	if ( m_pDeer )
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_RUN_ATTACK_STATE);
		m_pDeer->GetAnimatedModel()->RemoveAction(l_Num);
	}

	m_pDeer = NULL;
}


// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------
void CDeerRunAttackAnimationState::Execute( CCharacter*, float _ElapsedTime )
{
	/*if ( m_Currentduration == m_AnimationDuration )
	{
		int l_iAnimID = m_pDeer->GetAnimationID(DEER_RUN_ATTACK_STATE);
		m_pDeer->GetAnimatedModel()->BlendCycle(l_iAnimID, 0.3f);
	}

	m_Currentduration += _ElapsedTime;*/
}

void CDeerRunAttackAnimationState::OnEnter( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	/*m_Currentduration = 0.f;
	m_AnimationDuration = m_pDeer->GetAnimatedModel()->GetCurrentAnimationDuration(DEER_RUN_ATTACK_STATE) / 2;*/

	int l_Num = _pCharacter->GetAnimationID(DEER_RUN_ATTACK_STATE);
	m_pDeer->GetAnimatedModel()->ExecuteAction( l_Num, 0.1f );
}

void CDeerRunAttackAnimationState::OnExit( CCharacter* _pCharacter )
{
	if (!m_pDeer) 
	{
		m_pDeer = dynamic_cast<CDeer*> (_pCharacter);
	}

	if ( m_pDeer )
	{
		int l_Num = m_pDeer->GetAnimationID(DEER_RUN_ATTACK_STATE);
		m_pDeer->GetAnimatedModel()->RemoveAction(l_Num);
	}
}

bool CDeerRunAttackAnimationState::OnMessage( CCharacter*, const STelegram& _Telegram )
{
	return false;
}
