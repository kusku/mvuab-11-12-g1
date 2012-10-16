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
class CRabbitAttackState;
class CRabbitTiredState;
class CRabbitHitState;
class CRabbitDefenseState;
class CRabbitStillAttackState;
class CRabbitRunAttackState;
class CRabbitDeathState;

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
	void			Update				(  float _ElapsedTime );
	void			CreateCallbacks		( void );

	virtual void	LoadGraphicStates	( void ); 
	virtual void	LoadLogicStates		( void );

	//----Methods ---------------------------------------------
	bool			IsFatigued			( void );
	void			BeDead				( void );

	bool			CallHitState		( CCharacter* _pCharacter, const STelegram& _Message );
	void			CreateSpeaker		( void );
	void			SetSoundsOff		( void );

	//----Properties ------------------------------------------
	CRabbitIdleState *					GetIdleState					( void ) const			{ return m_pIdleState; }
	CRabbitPursuitState	*				GetPursuitState					( void ) const			{ return m_pPursuitState; }
	CRabbitPreparedToAttackState *		GetPreparedToAttack				( void ) const			{ return m_pPreparedToAttackState; }
	CRabbitAttackState *				GetAttackState					( void ) const			{ return m_pAttackState; }
	CRabbitTiredState *					GetTiredState					( void ) const			{ return m_pTiredState; }
	CRabbitHitState *					GetHitState						( void ) const			{ return m_pHitState; }
	CRabbitDefenseState *				GetDefenseState					( void ) const			{ return m_pDefenseState; }
	CRabbitStillAttackState *			GetStillAttackState				( void ) const			{ return m_pStillAttack; }
	CRabbitRunAttackState *				GetRunAttackState				( void ) const			{ return m_pRunAttack; }
	CRabbitDeathState *					GetDeathState					( void ) const			{ return m_pDeathState; }

	CRabbitIdleAnimationState *			GetIdleAnimationState			( void ) const			{ return m_pAnimationIdleState; }
	CRabbitIdle2AnimationState *		GetIdle2AnimationState			( void ) const			{ return m_pAnimationIdle2State; }
	CRabbitRunAnimationState *			GetRunAnimationState			( void ) const			{ return m_pAnimationRunState; }
	CRabbitDeathAnimationState *		GetDeathAnimationState			( void ) const			{ return m_pAnimationDeathState; }
	CRabbitDefenseAnimationState *		GetDefenseAnimationState		( void ) const			{ return m_pAnimationDefenseState; }
	CRabbitHitAnimationState *			GetHitAnimationState			( void ) const			{ return m_pAnimationHitState; }
	//CRabbitJumpAnimationState *			GetJumpAnimationState			( void ) const			{ return m_pAnimationJumpState; }
	CRabbitLeftLateralAnimationState *	GetLefLateralAnimationState		( void ) const			{ return m_pAnimationLeftLateralState; }
	CRabbitRightLateralAnimationState *	GetRightLateralAnimationState	( void ) const			{ return m_pAnimationRightLateralState; }
	CRabbitRunAttackAnimationState *	GetRunAttackAnimationState		( void ) const			{ return m_pAnimationRunAttackState; }
	CRabbitStillAttackAnimationState *	GetStillAttackAnimationState	( void ) const			{ return m_pAnimationStillAttackState; }
	CRabbitWalkAnimationState *			GetWalkAnimationState			( void ) const			{ return m_pAnimationWalkState; }

	inline void							SetHitsDone						( int _Hits ) 			{ m_HitsDone = _Hits; }
	inline const int					GetHitsDone						( void ) const			{ return m_HitsDone; }

	inline void							SetReceivedHitsXMinut			( int _Hits ) 			{ m_ReceivedHits_X_Minut = _Hits; }
	inline const int					GetReceivedHitsXMinut			( void ) const			{ return m_ReceivedHits_X_Minut; }

	inline const int 					GetTotalReceivedHitsXMinut		( void ) const			{ return m_TotalReceivedHits_X_Minut; }

	inline void							SetTotalHitsToBeTired			( int _HitsToBeTired ) 	{ m_TotalHitsDoneToTired = _HitsToBeTired; }
	inline const int					GetTotalHitsToBeTired			( void ) const			{ return m_TotalHitsDoneToTired; }

	//----Members ---------------------------------------------
private:
	int		m_HitsDone;							// Nº de hits que el conejo ha realizado
	int		m_TotalHitsDoneToTired;				// Total de golpes antes de sentirse cansado

	// nº de hits que se le permite recibir al enemigo antes de empezar a bloquear. cuando llega al valor se pasa a estado de defense
	// cada ciertos golpeos pasaré a un estado de defensa. Estas variables me permitiràn controlarlo. 
	int		m_ReceivedHits_X_Minut;				// Conteo de golpeos recibidos
	int		m_TotalReceivedHits_X_Minut;		// Total permitidos antes de pasar a estado defense

	CRabbitIdleState					*	m_pIdleState;
	CRabbitPursuitState					*	m_pPursuitState;
	CRabbitPreparedToAttackState		*	m_pPreparedToAttackState;
	CRabbitAttackState					*	m_pAttackState;
	CRabbitTiredState					*	m_pTiredState;
	CRabbitHitState						*	m_pHitState;
	CRabbitDefenseState					*	m_pDefenseState;
	CRabbitStillAttackState				*	m_pStillAttack;
	CRabbitRunAttackState				*	m_pRunAttack; 
	CRabbitDeathState					*	m_pDeathState;

	CRabbitIdleAnimationState			*	m_pAnimationIdleState;
	CRabbitIdle2AnimationState			*	m_pAnimationIdle2State;
	CRabbitRunAnimationState			*	m_pAnimationRunState;
	CRabbitDeathAnimationState			*	m_pAnimationDeathState;
	CRabbitDefenseAnimationState		*	m_pAnimationDefenseState;
	CRabbitHitAnimationState			*	m_pAnimationHitState;
	//CRabbitJumpAnimationState			*	m_pAnimationJumpState;
	CRabbitLeftLateralAnimationState	*	m_pAnimationLeftLateralState;
	CRabbitRightLateralAnimationState	*	m_pAnimationRightLateralState;
	CRabbitRunAttackAnimationState		*	m_pAnimationRunAttackState;
	CRabbitStillAttackAnimationState	*	m_pAnimationStillAttackState;
	CRabbitWalkAnimationState			*	m_pAnimationWalkState;

};


#endif _CRABBIT_H_