#pragma once

#ifndef _CWOLF_H_
#define _CWOLF_H_


#include <string>
#include "Characters\Character.h"

// --- Foward Declarations ---

// Estats lógica
class CWolfIdleState;
class CWolfPursuitState;
class CWolfPreparedToAttackState;
class CWolfAttackState;
class CWolfTiredState;
class CWolfHitState;
class CWolfDefenseState;
class CWolfStillAttackState;
class CWolfRunAttackState;
class CWolfDeathState;

// Estats animació 
class CWolfIdleAnimationState;
//class CWolfIdle2AnimationState;
class CWolfRunAnimationState;
class CWolfDeathAnimationState;
class CWolfDefenseAnimationState;
class CWolfHitAnimationState;
class CWolfJumpAnimationState;
//class CWolfLeftLateralAnimationState;
//class CWolfRightLateralAnimationState;
class CWolfRunAttackAnimationState;
class CWolfStillAttackAnimationState;
class CWolfWalkAnimationState;
// ---------------------------

class CWolf :	public CCharacter
{
public:

	// ------------- Constructors i Destructors --------------
					CWolf				( int _Id );
					CWolf				( int _Id, std::string _Name );
	virtual			~CWolf			(void);

	//----Main Functions --------------------------------------
	virtual bool	Init				( void );
	void			CreateCallbacks		( void );

	virtual void	LoadGraphicStates	( void ); 
	virtual void	LoadLogicStates		( void );

	//----Methods ---------------------------------------------
	bool			IsFatigued			( void );
	void			BeDead				( void );

	//----Properties ------------------------------------------
	CWolfIdleState *					GetIdleState					( void ) const			{ return m_pIdleState; }
	CWolfPursuitState	*				GetPursuitState					( void ) const			{ return m_pPursuitState; }
	CWolfPreparedToAttackState *		GetPreparedToAttack				( void ) const			{ return m_pPreparedToAttackState; }
	CWolfAttackState *					GetAttackState					( void ) const			{ return m_pAttackState; }
	CWolfTiredState *					GetTiredState					( void ) const			{ return m_pTiredState; }
	CWolfHitState *						GetHitState						( void ) const			{ return m_pHitState; }
	CWolfDefenseState *					GetDefenseState					( void ) const			{ return m_pDefenseState; }
	CWolfStillAttackState *				GetStillAttackState				( void ) const			{ return m_pStillAttackState; }
	CWolfRunAttackState *				GetRunAttackState				( void ) const			{ return m_pRunAttackState; }
	CWolfDeathState *					GetDeathState					( void ) const			{ return m_pDeathState; }
	
	CWolfIdleAnimationState *			GetIdleAnimationState			( void ) const			{ return m_pAnimationIdleState; }
	//CWolfIdle2AnimationState *			GetIdle2AnimationState			( void ) const			{ return m_pAnimationIdle2State; }
	CWolfRunAnimationState *			GetRunAnimationState			( void ) const			{ return m_pAnimationRunState; }
	CWolfDeathAnimationState *			GetDeathAnimationState			( void ) const			{ return m_pAnimationDeathState; }
	CWolfDefenseAnimationState *		GetDefenseAnimationState		( void ) const			{ return m_pAnimationDefenseState; }
	CWolfHitAnimationState *			GetHitAnimationState			( void ) const			{ return m_pAnimationHitState; }
	CWolfJumpAnimationState *			GetJumpAnimationState			( void ) const			{ return m_pAnimationJumpState; }
	//CWolfLeftLateralAnimationState *	GetLefLateralAnimationState		( void ) const			{ return m_pAnimationLeftLateralState; }
	//CWolfRightLateralAnimationState *	GetRightLateralAnimationState	( void ) const			{ return m_pAnimationRightLateralState; }
	CWolfRunAttackAnimationState *		GetRunAttackAnimationState		( void ) const			{ return m_pAnimationRunAttackState; }
	CWolfStillAttackAnimationState *	GetStillAttackAnimationState	( void ) const			{ return m_pAnimationStillAttackState; }
	CWolfWalkAnimationState *			GetWalkAnimationState			( void ) const			{ return m_pAnimationWalkState; }

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
	

	CWolfIdleState					*	m_pIdleState;
	CWolfPursuitState				*	m_pPursuitState;
	CWolfPreparedToAttackState		*	m_pPreparedToAttackState;
	CWolfAttackState				*	m_pAttackState;
	CWolfTiredState					*	m_pTiredState;
	CWolfHitState					*	m_pHitState;
	CWolfDefenseState				*	m_pDefenseState;
	CWolfStillAttackState			*	m_pStillAttackState;
	CWolfRunAttackState				*	m_pRunAttackState; 
	CWolfDeathState					*	m_pDeathState; 

	CWolfIdleAnimationState			*	m_pAnimationIdleState;
	//CWolfIdle2AnimationState		*	m_pAnimationIdle2State;
	CWolfRunAnimationState			*	m_pAnimationRunState;
	CWolfDeathAnimationState		*	m_pAnimationDeathState;
	CWolfDefenseAnimationState		*	m_pAnimationDefenseState;
	CWolfHitAnimationState			*	m_pAnimationHitState;
	CWolfJumpAnimationState			*	m_pAnimationJumpState;
	/*CWolfLeftLateralAnimationState	*	m_pAnimationLeftLateralState;
	CWolfRightLateralAnimationState	*	m_pAnimationRightLateralState;*/
	CWolfRunAttackAnimationState	*	m_pAnimationRunAttackState;
	CWolfStillAttackAnimationState	*	m_pAnimationStillAttackState;
	CWolfWalkAnimationState			*	m_pAnimationWalkState;

};


#endif _CWOLF_H_