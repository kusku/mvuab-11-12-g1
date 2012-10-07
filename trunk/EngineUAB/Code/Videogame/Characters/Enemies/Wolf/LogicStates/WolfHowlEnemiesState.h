#pragma once

#ifndef _WOLF_HOWL_ENEMIES_STATE_H_
#define _WOLF_HOWL_ENEMIES_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "Callbacks\State\ActionStateCallback.h"

// --- Foward Declarations ---
class CWolf;
class CAnimationCallback;
// ---------------------------

class CWolfHowlEnemiesState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CWolfHowlEnemiesState	( CCharacter* );
					CWolfHowlEnemiesState	( CCharacter*, const std::string &_Name );
	virtual			~CWolfHowlEnemiesState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Methods ---------------------------------------------
	void			ShowEnemiesToHelp			( void );

	void			SetCurrentDynamicEnemyIndex	( uint32 _Index )			{ m_DynamicEnemyIndex = _Index; }
	inline uint32	GetCurrentDynamicEnemyIndex	( void ) const				{ return m_DynamicEnemyIndex; }  
	
	//----Members ---------------------------------------------
private:
	CWolf				  * m_pWolf;
	CActionStateCallback	m_ActionStateCallback;
	CAnimationCallback    *	m_pAnimationCallback;

	float					m_SoundDuration;
	uint32					m_DynamicEnemyIndex;
};


#endif _WOLF_HOWL_ENEMIES_STATE_H_