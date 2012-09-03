#pragma once

#ifndef _CRABBIT_H_
#define _CRABBIT_H_


#include <string>
#include "Characters\Character.h"

// --- Foward Declarations ---

// Estats lógica
class CRabbitIdleState;
class CRabbitPursuitState;
class CRabbitPreparedToAttackState;

// Estats animació 
class CRabbitIdleAnimationState;
class CRabbitIdle2AnimationState;
class CRabbitRunAnimationState;
class CRabbitDeathAnimationState;
class CRabbitDefenseAnimationState;
class CRabbitHitAnimationState;
class CRabbitJumpAnimationState;
class CRabbitLeftLateralAnimationState;
class CRabbitRightLateralAnimationState;
class CRabbitRunAttackAnimationState;
class CRabbitStillAttackAnimationState;
class CRabbitWalkAnimationState;
// ---------------------------

class CRabbit :	public CCharacter
{
public:

	// ------------- Constructors i Destructors --------------
					CRabbit				( int _Id );
					CRabbit				( int _Id, std::string _Name );
	virtual			~CRabbit			(void);

	//----Main Functions --------------------------------------
	virtual bool	Init				( void );
	virtual void	LoadGraphicStates	( void ); 
	virtual void	LoadLogicStates		( void );

	//----Methods ---------------------------------------------

	//----Properties ------------------------------------------
	CRabbitIdleState *					GetIdleState					( void ) const		{ return m_pIdleState; }
	CRabbitPursuitState	*				GetPursuitState					( void ) const		{ return m_pPursuitState; }
	CRabbitPreparedToAttackState *		GetPreparedToAttack				( void ) const		{ return m_pPreparedToAttackState; }

	CRabbitIdleAnimationState *			GetIdleAnimationState			( void ) const		{ return m_pAnimationIdleState; }
	CRabbitIdle2AnimationState *		GetIdle2AnimationState			( void ) const		{ return m_pAnimationIdle2State; }
	CRabbitRunAnimationState *			GetRunAnimationState			( void ) const		{ return m_pAnimationRunState; }
	CRabbitDeathAnimationState *		GetDeathAnimationState			( void ) const		{ return m_pAnimationDeathState; }
	CRabbitDefenseAnimationState *		GetDefenseAnimationState		( void ) const		{ return m_pAnimationDefenseState; }
	CRabbitHitAnimationState *			GetHitAnimationState			( void ) const		{ return m_pAnimationHitState; }
	CRabbitJumpAnimationState *			GetJumpAnimationState			( void ) const		{ return m_pAnimationJumpState; }
	CRabbitLeftLateralAnimationState *	GetLefLateralAnimationState		( void ) const		{ return m_pAnimationLeftLateralState; }
	CRabbitRightLateralAnimationState *	GetRightLateralAnimationState	( void ) const		{ return m_pAnimationRightLateralState; }
	CRabbitRunAttackAnimationState *	GetRunAttackAnimationState		( void ) const		{ return m_pAnimationRunAttackState; }
	CRabbitStillAttackAnimationState *	GetStillAttackAnimationState	( void ) const		{ return m_pAnimationStillAttackState; }
	CRabbitWalkAnimationState *			GetWalkAnimationState			( void ) const		{ return m_pAnimationWalkState; }

	void								SetHitsDone						( int _Hits ) 		{ m_HitsDone = _Hits; }
	const int							GetHitsDone						( void ) const		{ return m_HitsDone; }

	//void								SetReceivedHitsXMinut			( int _Hits ) const	{ m_ReceivedHits_X_Minut = _Hits; }
	const int							GetReceivedHitsXMinut			( void ) const		{ return m_ReceivedHits_X_Minut; }

	const int 							GetTotalReceivedHitsXMinut		( void ) const		{ return m_TotalReceivedHits_X_Minut; }

	//----Members ---------------------------------------------
private:
	int		m_HitsDone;				// Nº de hits que el conejo ha realizado

	// nº de hits que se le permite recibir al enemigo antes de empezar a bloquear. cuando llega al valor se pasa a estado de defense
	// cada ciertos golpeos pasaré a un estado de defensa. estas variables me permitiràn controlarlo. 
	int		m_ReceivedHits_X_Minut;				// conteo de golpeos 
	int		m_TotalReceivedHits_X_Minut;		// Total permitidos antes de pasar a estado defense

	CRabbitIdleState					*	m_pIdleState;
	CRabbitPursuitState					*	m_pPursuitState;
	CRabbitPreparedToAttackState		*	m_pPreparedToAttackState;

	CRabbitIdleAnimationState			*	m_pAnimationIdleState;
	CRabbitIdle2AnimationState			*	m_pAnimationIdle2State;
	CRabbitRunAnimationState			*	m_pAnimationRunState;
	CRabbitDeathAnimationState			*	m_pAnimationDeathState;
	CRabbitDefenseAnimationState		*	m_pAnimationDefenseState;
	CRabbitHitAnimationState			*	m_pAnimationHitState;
	CRabbitJumpAnimationState			*	m_pAnimationJumpState;
	CRabbitLeftLateralAnimationState	*	m_pAnimationLeftLateralState;
	CRabbitRightLateralAnimationState	*	m_pAnimationRightLateralState;
	CRabbitRunAttackAnimationState		*	m_pAnimationRunAttackState;
	CRabbitStillAttackAnimationState	*	m_pAnimationStillAttackState;
	CRabbitWalkAnimationState			*	m_pAnimationWalkState;

};


#endif _CRABBIT_H_