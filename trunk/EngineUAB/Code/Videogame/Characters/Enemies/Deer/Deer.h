#pragma once

#ifndef _DEER_H_
#define _DEER_H_


#include <string>
#include "Characters\Character.h"

// --- Foward Declarations ---

// Estats lógica
class CDeerIdleState;
class CDeerPursuitState;
class CDeerPreparedToAttackState;
class CDeerAttackState;
class CDeerTiredState;
class CDeerHitState;
class CDeerDefenseState;
class CDeerStillAttackState;
class CDeerRunAttackState;
class CDeerDeathState;

// Estats animació 
class CDeerIdleAnimationState;
class CDeerRunAnimationState;
class CDeerDeathAnimationState;
class CDeerDefenseAnimationState;
class CDeerHitAnimationState;
class CDeerRunAttackAnimationState;
class CDeerStillAttackAnimationState;
class CDeerWalkAnimationState;
// ---------------------------

class CDeer :	public CCharacter
{
public:

	// ------------- Constructors i Destructors --------------
					CDeer				( int _Id );
					CDeer				( int _Id, std::string _Name );
	virtual			~CDeer			(void);

	//----Main Functions --------------------------------------
	virtual bool	Init				( void );
	void			CreateCallbacks		( void );

	virtual void	LoadGraphicStates	( void ); 
	virtual void	LoadLogicStates		( void );

	//----Methods ---------------------------------------------
	bool			IsFatigued			( void );
	void			BeDead				( void );

	//----Properties ------------------------------------------
	CDeerIdleState *					GetIdleState					( void ) const			{ return m_pIdleState; }
	CDeerPursuitState	*				GetPursuitState					( void ) const			{ return m_pPursuitState; }
	CDeerPreparedToAttackState *		GetPreparedToAttack				( void ) const			{ return m_pPreparedToAttackState; }
	CDeerAttackState *					GetAttackState					( void ) const			{ return m_pAttackState; }
	CDeerTiredState *					GetTiredState					( void ) const			{ return m_pTiredState; }
	CDeerHitState *						GetHitState						( void ) const			{ return m_pHitState; }
	CDeerDefenseState *					GetDefenseState					( void ) const			{ return m_pDefenseState; }
	CDeerStillAttackState *				GetStillAttackState				( void ) const			{ return m_pStillAttackState; }
	CDeerRunAttackState *				GetRunAttackState				( void ) const			{ return m_pRunAttackState; }
	CDeerDeathState *					GetDeathState					( void ) const			{ return m_pDeathState; }
	
	CDeerIdleAnimationState *			GetIdleAnimationState			( void ) const			{ return m_pAnimationIdleState; }
	CDeerRunAnimationState *			GetRunAnimationState			( void ) const			{ return m_pAnimationRunState; }
	CDeerDeathAnimationState *			GetDeathAnimationState			( void ) const			{ return m_pAnimationDeathState; }
	CDeerDefenseAnimationState *		GetDefenseAnimationState		( void ) const			{ return m_pAnimationDefenseState; }
	CDeerHitAnimationState *			GetHitAnimationState			( void ) const			{ return m_pAnimationHitState; }
	CDeerRunAttackAnimationState *		GetRunAttackAnimationState		( void ) const			{ return m_pAnimationRunAttackState; }
	CDeerStillAttackAnimationState *	GetStillAttackAnimationState	( void ) const			{ return m_pAnimationStillAttackState; }
	CDeerWalkAnimationState *			GetWalkAnimationState			( void ) const			{ return m_pAnimationWalkState; }

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
	

	CDeerIdleState					*	m_pIdleState;
	CDeerPursuitState				*	m_pPursuitState;
	CDeerPreparedToAttackState		*	m_pPreparedToAttackState;
	CDeerAttackState				*	m_pAttackState;
	CDeerTiredState					*	m_pTiredState;
	CDeerHitState					*	m_pHitState;
	CDeerDefenseState				*	m_pDefenseState;
	CDeerStillAttackState			*	m_pStillAttackState;
	CDeerRunAttackState				*	m_pRunAttackState; 
	CDeerDeathState					*	m_pDeathState; 


	CDeerIdleAnimationState			*	m_pAnimationIdleState;
	CDeerRunAnimationState			*	m_pAnimationRunState;
	CDeerDeathAnimationState		*	m_pAnimationDeathState;
	CDeerDefenseAnimationState		*	m_pAnimationDefenseState;
	CDeerHitAnimationState			*	m_pAnimationHitState;
	CDeerRunAttackAnimationState	*	m_pAnimationRunAttackState;
	CDeerStillAttackAnimationState	*	m_pAnimationStillAttackState;
	CDeerWalkAnimationState			*	m_pAnimationWalkState;

};


#endif _DEER_H_