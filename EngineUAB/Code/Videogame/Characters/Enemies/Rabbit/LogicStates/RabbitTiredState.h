#pragma once

#ifndef _RABBIT_TIRED_STATE_H_
#define _RABBIT_TIRED_STATE_H_


#include <string>
#include "StatesMachine\State.h"
#include "Callbacks\State\ActionStateCallback.h"
#include "Characters\Character.h"

// --- Foward Declarations ---
class CRabbit;
// ---------------------------

class CRabbitTiredState : public CState<CCharacter> 
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbitTiredState	( CCharacter* );
					CRabbitTiredState	( CCharacter* _pCharacter, const std::string &_Name );
	virtual			~CRabbitTiredState	( void );

	//----Main Functions --------------------------------------
	virtual void	Execute		( CCharacter*, float _ElapsedTime );

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( CCharacter* );

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( CCharacter* );

	// Se ejecuta cuando otro caracter envía un mensaje a este
	virtual bool	OnMessage	( CCharacter*, const STelegram& );

	//----Properties ------------------------------------------
	const float		GetMinTiredTime ( void ) const				{ return m_MinTime; }
	const float		GetMaxTiredTime ( void ) const				{ return m_MaxTime; }
	
	//----Methods ---------------------------------------------
	void			SetTiredTime	( float _MinValue, float _MaxValue )	{ m_MinTime = _MinValue; m_MaxTime = _MaxValue; }

	//----Members ---------------------------------------------
private:
	CActionStateCallback		m_ActionTime;
	CRabbit					  * m_pRabbit;
	
	std::string					m_ActiveActionState;		// Me indica qué acción o estado vamos a ejecutar después de hacer un random de todas ellas
	float						m_MinTime, m_MaxTime;
};


#endif _RABBIT_TIRED_STATE_H_