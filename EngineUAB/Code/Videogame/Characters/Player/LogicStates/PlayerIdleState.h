#pragma once

#ifndef _PLAYER_IDLE_STATE_H_
#define _PLAYER_IDLE_STATE_H_

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include <string>

struct STelegram;
class CPlayer;
class CGameProcess;
class CActionToInput;

class CPlayerIdleState : public CState< CCharacter >
{
public:
	CPlayerIdleState( CCharacter * _pCharacter, const std::string &_Name );
	~CPlayerIdleState();

	void	OnEnter		(  CCharacter* _pCharacter );
	void	Execute		(  CCharacter* _pCharacter, float _fElapsedTime );
	void	OnExit		(  CCharacter* _pCharacter );

	bool	OnMessage	(  CCharacter* _pCharacter,  const STelegram& _Message );

	void			UpdateParticlesPositions( CCharacter* _pCharacter );

	// Para tema de partículas de impacto
	void			GenerateImpact			( CCharacter* _pCharacter );
	void			UpdateImpact			( CCharacter* _pCharacter );

private:
	CGameProcess	*m_pProcess;
	CActionToInput	*m_pInput;
	CPlayer			*m_pPlayer;
};

#endif //_PLAYER_IDLE_STATE_H_