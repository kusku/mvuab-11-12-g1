#pragma once

#ifndef __STEERING_BEHAVIORS_SETTINGS_MANAGER_CLASS_H__
#define __STEERING_BEHAVIORS_SETTINGS_MANAGER_CLASS_H__

#include <string>
#include "SteeringBehaviorsDefs.h"

// -- FOWARD Declarations -----
class CRenderManager;
// ----------------------------

class CSteeringBehaviorsSeetingsManager
{
public:
	//--- Init and End protocols------------------------------------------
					CSteeringBehaviorsSeetingsManager		( void );
	virtual			~CSteeringBehaviorsSeetingsManager		( void );

	//----Main Functions ------------------------------------------------
	void			Init					( void );
	void			Destroy					( void );
	bool			Load					( const std::string &_FileName );
	bool			Reload					( void );
	
	void			Update					( float _ElapsedTime );
	void			Render					( CRenderManager *_RM );
	
	//---- Public Interfaces ------------------------------------------------
	
	//---- Properties ( get & Set )---------------------------------------
	inline void		SetObstacleWallDetectionFeelerLength	( float _ObstacleWallDetectionFeelerLength )		{ m_ObstacleWallDetectionFeelerLength = _ObstacleWallDetectionFeelerLength; }
	inline float	GetObstacleWallDetectionFeelerLength	( void ) const										{ return m_ObstacleWallDetectionFeelerLength; }
	
	inline void		SetObstacleWallDetectionFeelerAngle		( float _ObstacleWallDetectionFeelerAngle )			{ m_ObstacleWallDetectionFeelerAngle = _ObstacleWallDetectionFeelerAngle; }
	inline float	GetObstacleWallDetectionFeelerAngle		( void ) const										{ return m_ObstacleWallDetectionFeelerAngle; }
	
	inline void		SetDetectionFeelerAngleIncrement		( float _DetectionFeelerAngleIncrement )			{ m_DetectionFeelerAngleIncrement = _DetectionFeelerAngleIncrement; }
	inline float	GetDetectionFeelerAngleIncrement		( void ) const										{ return m_DetectionFeelerAngleIncrement; }

	inline void		SetCollisionDetectionFeelerLength		( float _CollisionDetectionFeelerLength )			{ m_CollisionDetectionFeelerLength = _CollisionDetectionFeelerLength; }
	inline float	GetCollisionDetectionFeelerLength		( void ) const										{ return m_CollisionDetectionFeelerLength; }

	inline void		SetCollisionDetectionFeelerAngle		( float _CollisionDetectionFeelerAngle )			{ m_CollisionDetectionFeelerAngle = _CollisionDetectionFeelerAngle; }
	inline float	GetCollisionDetectionFeelerAngle		( void ) const										{ return m_CollisionDetectionFeelerAngle; }
	
	inline void		SetSeparationDetectionFeelerLength		( float _SeparationDetectionFeelerLength )			{ m_SeparationDetectionFeelerLength = _SeparationDetectionFeelerLength; }
	inline float	GetSeparationDetectionFeelerLength		( void ) const										{ return m_SeparationDetectionFeelerLength; }

	inline void		SetSeparationDecayCoefficient			( float _SeparationDecayCoefficient )				{ m_SeparationDecayCoefficient = _SeparationDecayCoefficient; }
	inline float	GetSeparationDecayCoefficient			( void ) const										{ return m_SeparationDecayCoefficient; }

	inline void		SetArriveDecelarationDistance			( float _ArriveDecelarationDistance )				{ m_ArriveDecelarationDistance = _ArriveDecelarationDistance; }
	inline float	GetArriveDecelarationDistance			( void ) const										{ return m_ArriveDecelarationDistance; }

	inline void		SetArriveDecelarationTweaker			( float _ArriveDecelarationTweaker )				{ m_ArriveDecelarationTweaker = _ArriveDecelarationTweaker; }
	inline float	GetArriveDecelarationTweaker			( void ) const										{ return m_ArriveDecelarationTweaker; }

	inline void				SetArriveDecelaration			( eDeceleration _ArriveDecelaration )				{ m_ArriveDecelaration = _ArriveDecelaration; }
	inline eDeceleration	GetArriveDecelaration			( void ) const										{ return m_ArriveDecelaration; }

	inline void		SetNeightbourRadius						( float _NeightbourRadius )							{ m_NeightbourRadius = _NeightbourRadius; }
	inline float	GetNeightbourRadius						( void ) const										{ return m_NeightbourRadius; }

	// --- Parámetros del comportamiento Wander ---
	inline void		SetWanderRadius							( float _WanderRadius )								{ m_WanderRadius = _WanderRadius; }
	inline float	GetWanderRadius							( void ) const										{ return m_WanderRadius; }

	inline void		SetWanderDistance						( float _WanderDistance )							{ m_WanderDistance = _WanderDistance; }
	inline float	GetWanderDistance						( void ) const										{ return m_WanderDistance; }

	inline void		SetWanderRefreshRate					( float _WanderRefreshRate )						{ m_WanderRefreshRate = _WanderRefreshRate; }
	inline float	GetWanderRefreshRate					( void ) const										{ return m_WanderRefreshRate; }

	inline void		SetWanderMinimumDistance				( float _WanderMinimumDistance )					{ m_WanderMinimumDistance = _WanderMinimumDistance; }
	inline float	GetWanderMinimumDistance				( void ) const										{ return m_WanderMinimumDistance; }

	// --- Parámetros de pesos ---
	inline void		SetSeparationWeight						( float _SeparationWeight )							{ m_SeparationWeight = _SeparationWeight; }
	inline float	GetSeparationWeight						( void ) const										{ return m_SeparationWeight; }

	inline void		SetAlignmentWeight						( float _AlignmentWeight )							{ m_AlignmentWeight = _AlignmentWeight; }
	inline float	GetAlignmentWeight						( void ) const										{ return m_AlignmentWeight; }

	inline void		SetCohesionWeight						( float _CohesionWeight )							{ m_CohesionWeight = _CohesionWeight; }
	inline float	GetCohesionWeight						( void ) const										{ return m_CohesionWeight; }

	inline void		SetCollisionAvoidanceWeight				( float _CollisionAvoidanceWeight )					{ m_CollisionAvoidanceWeight = _CollisionAvoidanceWeight; }
	inline float	GetCollisionAvoidanceWeight				( void ) const										{ return m_CollisionAvoidanceWeight; }

	inline void		SetObstacleObstacleWallAvoidanceWeight	( float _ObstacleWallAvoidanceWeight )				{ m_ObstacleObstacleWallAvoidanceWeight = _ObstacleWallAvoidanceWeight; }
	inline float	GetObstacleObstacleWallAvoidanceWeight	( void ) const										{ return m_ObstacleObstacleWallAvoidanceWeight; }

	inline void		SetWanderWeight							( float _WanderWeight )								{ m_WanderWeight = _WanderWeight; }
	inline float	GetWanderWeight							( void ) const										{ return m_WanderWeight; }

	inline void		SetSeekWeight							( float _SeekWeight )								{ m_SeekWeight = _SeekWeight; }
	inline float	GetSeekWeight							( void ) const										{ return m_SeekWeight; }

	inline void		SetFleeWeight							( float _FleeWeight )								{ m_FleeWeight = _FleeWeight; }
	inline float	GetFleeWeight							( void ) const										{ return m_FleeWeight; }

	inline void		SetArriveWeight							( float _ArriveWeight )								{ m_ArriveWeight = _ArriveWeight; }
	inline float	GetArriveWeight							( void ) const										{ return m_ArriveWeight; }

	inline void		SetPursuitWeight						( float _PursuitWeight )							{ m_PursuitWeight = _PursuitWeight; }
	inline float	GetPursuitWeight						( void ) const										{ return m_PursuitWeight; }

	inline void		SetOffsetPursuitWeight					( float _OffsetPursuitWeight )						{ m_OffsetPursuitWeight = _OffsetPursuitWeight; }
	inline float	GetOffsetPursuitWeight					( void ) const										{ return m_OffsetPursuitWeight; }

	inline void		SetInterposeWeight						( float _InterposeWeight )							{ m_InterposeWeight = _InterposeWeight; }
	inline float	GetInterposeWeight						( void ) const										{ return m_InterposeWeight; }

	inline void		SetHideWeight							( float _HideWeight )								{ m_HideWeight = _HideWeight; }
	inline float	GetHideWeight							( void ) const										{ return m_HideWeight; }

	inline void		SetEvadeWeight							( float _EvadeWeight )								{ m_EvadeWeight = _EvadeWeight; }
	inline float	GetEvadeWeight							( void ) const										{ return m_EvadeWeight; }

	inline void		SetFollowPathWeight						( float _FollowPathWeight )							{ m_FollowPathWeight = _FollowPathWeight; }
	inline float	GetFollowPathWeight						( void ) const										{ return m_FollowPathWeight; }
	
	inline void		SetNumberEnemiesToAttackAtSameTime		( int _NumberEnemiesToAttackAtSameTime )			{ m_NumberEnemiesToAttackAtSameTime = _NumberEnemiesToAttackAtSameTime; }
	inline int		GetNumberEnemiesToAttackAtSameTime		( void ) const										{ return m_NumberEnemiesToAttackAtSameTime; }
	
	inline void		SetMaxNumberEnemiesCanHelp				( int _MaxNumberEnemiesCanHelp )					{ m_MaxNumberEnemiesCanHelp = _MaxNumberEnemiesCanHelp; }
	inline int		GetMaxNumberEnemiesCanHelp				( void ) const										{ return m_MaxNumberEnemiesCanHelp; }
	
	inline void		SetMinNumberEnemiesCanHelp				( int _MinNumberEnemiesCanHelp )					{ m_MinNumberEnemiesCanHelp = _MinNumberEnemiesCanHelp; }
	inline int		GetMinNumberEnemiesCanHelp				( void ) const										{ return m_MinNumberEnemiesCanHelp; }
	
	inline void		SetCamaraRangeAngleForPrepared			( float _CamaraRangeAngleForPrepared )				{ m_CamaraRangeAngleForPrepared = _CamaraRangeAngleForPrepared; }
	inline float	GetCamaraRangeAngleForPrepared			( void ) const										{ return m_CamaraRangeAngleForPrepared; }
	
	inline void		SetCamaraRangeAngleForAttack			( float _CamaraRangeAngleForAttack )				{ m_CamaraRangeAngleForAttack = _CamaraRangeAngleForAttack; }
	inline float	GetCamaraRangeAngleForAttack			( void ) const										{ return m_CamaraRangeAngleForAttack; }
	
	//----Private Functions -----------------------------------------------------
private:
	bool			LoadXML					( void );

	//---- Members -------------------------------------------------------
private:
	std::string			m_szFilename;		// Fitxer xml on trobaré les propietats dels comportaments
	
	// longitud de colision de diferentes comportamientos 
	float			m_ObstacleWallDetectionFeelerLength;	// Longitud de los rayos de colisión con obstaculos y paredes
	float			m_ObstacleWallDetectionFeelerAngle;		// Ángulo de los rayos de colisión con obstaculos y paredes
	float			m_DetectionFeelerAngleIncrement;		// Este me permite crear un rayo cada X grados de incremento. Es decir, cada X grados dentro del angulo crea un rayo
	float			m_CollisionDetectionFeelerLength;		// Longitud de los rayos de colisión con otros caracteres
	float			m_CollisionDetectionFeelerAngle;		// Ángulo de los rayos de colisión con caracteres
	float			m_SeparationDetectionFeelerLength;		// 
	float			m_SeparationDecayCoefficient;
	float			m_ArriveDecelarationDistance;			// Distancia a partir de la cual desaceleramos
	float			m_ArriveDecelarationTweaker;			// Permite suavizar la desaceleración. Este paràmetro ayuda en la divisíon
	eDeceleration	m_ArriveDecelaration;					// Cantidad de desaceleración: rápida, normal, lenta
	float			m_NeightbourRadius;						// Radio de búsqueda de vecinos en el estado de Flocking : separation, cohesion, alignment
	float			m_WanderRadius;							// Medida del radio del círculo de vagaje aleatorio
	float			m_WanderDistance;						// Distancia del enemigo a la posició frontal donde ubicamos el círculo
	float			m_WanderRefreshRate;					// Times to refresh x second
	float			m_WanderMinimumDistance;				// Mínima distancia para volver a cambiar de sentido

	// usa estos valores para ajustar la cantidad de cada fuerza de comportamiento direccional del total de la fuerza steering 

	float			m_SeparationWeight;
	float			m_AlignmentWeight;
	float			m_CohesionWeight;
	float			m_CollisionAvoidanceWeight;
	float			m_ObstacleObstacleWallAvoidanceWeight;
	float			m_WanderWeight;
	float			m_SeekWeight;
	float			m_FleeWeight;
	float			m_ArriveWeight;
	float			m_PursuitWeight;
	float			m_OffsetPursuitWeight;
	float			m_InterposeWeight;
	float			m_HideWeight;
	float			m_EvadeWeight;
	float			m_FollowPathWeight;
					
	float			m_prWallAvoidance;
	float			m_prObstacleAvoidance;
	float			m_prSeparation;
	float			m_prAlignment;
	float			m_prCohesion;
	float			m_prWander;
	float			m_prSeek;
	float			m_prFlee;
	float			m_prEvade;
	float			m_prHide;
	float			m_prArrive;

	int				m_NumberEnemiesToAttackAtSameTime;
	int				m_MaxNumberEnemiesCanHelp;
	int				m_MinNumberEnemiesCanHelp;

	float			m_CamaraRangeAngleForPrepared;
	float			m_CamaraRangeAngleForAttack;
};	
	
#endif __STEERING_BEHAVIORS_SETTINGS_MANAGER_CLASS_H__