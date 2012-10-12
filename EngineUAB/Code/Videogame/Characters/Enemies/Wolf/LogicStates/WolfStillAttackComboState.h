#pragma once

#ifndef _WOLF_STILL_ATTACK_COMBO_STATE_H_
#define _WOLF_STILL_ATTACK_COMBO_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CWolf;
class CParticleEmitter;
class CAnimationCallback;
class CActionStateCallback;
// ---------------------------

class CWolfStillAttackComboState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfStillAttackComboState	( CCharacter* );
					CWolfStillAttackComboState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfStillAttackComboState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			UpdateParticlesPositions( CCharacter* _pCharacter );
	
	// Para tema de partículas de impacto
	void			GenerateImpact			( CCharacter* _pCharacter );
	void			UpdateImpact			( CCharacter* _pCharacter );

	//----Members ---------------------------------------------
private:
	CWolf					  * m_pWolf;
	CAnimationCallback		  *	m_pAnimationCallback;
	CActionStateCallback	  * m_pActionStateCallback;

	bool						m_SoundPlayed1;				// Indica si el sonido ya se inició
	bool						m_SoundPlayed2;				// Indica si el sonido ya se inició
	bool						m_SoundPlayed3;				// Indica si el sonido ya se inició
	bool						m_SoundPlayedScream1;		// Indica si el sonido de chillo ya se inició
	bool						m_SoundPlayedScream2;		// Indica si el sonido de chillo ya se inició
	bool						m_SoundPlayedScream3;		// Indica si el sonido de chillo ya se inició
	bool						m_FirstHitDone;				// Indica que ya se ha lanzado el golpeo
	bool						m_FirstHitReached;			// Indica si alcancé al player con el golpeo
	bool						m_FirstParticlesHitDone;	// Indiaca que ya se ha lanzado las partículas de impacto
	bool						m_SecondHitDone;			// Indica que ya se ha lanzado el golpeo
	bool						m_SecondHitReached;			// Indica si alcancé al player con el golpeo
	bool						m_SecondParticlesHitDone;	// Indiaca que ya se ha lanzado las partículas de impacto
	bool						m_ThirdHitDone;				// Indica que ya se ha lanzado el golpeo
	bool						m_ThirdHitReached;			// Indica si alcancé al player con el golpeo
	bool						m_ThirdParticlesHitDone;	// Indiaca que ya se ha lanzado las partículas de impacto

	// Tema Partículas
	CParticleEmitter	*m_pParticleEmitter;
};


#endif _WOLF_STILL_ATTACK_COMBO_STATE_H_