#pragma once

#ifndef __STEERING_BEHAVIORS_CLASS_H__
#define __STEERING_BEHAVIORS_CLASS_H__

#include <list>
#include <vector>

#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Math\Vector3.h"

// --- Foward Declarations ---
class CSteeringEntity;
class CSeek;
class CFlee;
class CPursuit;
class CEvade;
class COffsetPursuit;
class CArrive;
class CWander;
class CCollisionAvoidance;
class CObstacleWallAvoidance;
class CSeparation;
class CAlignment;
class CCohesion;
class CPhysicUserData;
// ---------------------------

class CSteeringBehaviors
{
public:
	//--- Init and End protocols------------------------------------------
								CSteeringBehaviors				( float _MaxForce );
	virtual						~CSteeringBehaviors				( void );

	//---- Main Functions ------------------------------------------------
	void						Initialize						( void );
	void						Destroy							( void );
	const Vect3f&				Update							( float _ElapsedTime, CSteeringEntity * _pEntity);
	const Vect3f&				CalculateWeightedSum			( void ); 
	const Vect3f&				CalculatePrioritized			( void ); 
	const Vect3f&				CalculateDithered				( void ); 

	bool						AccumulateForce					( Vect3f & _CurrentSteeringForce, Vect3f _ForceToAdd );

	//---- Functions -----------------------------------------------------
	void						AddBehavior						( CSeek *_pSteering );
	void						AddBehavior						( CFlee *_pSteering );
	void						AddBehavior						( CPursuit *_pSteering );
	void						AddBehavior						( CEvade *_pSteering );
	void						AddBehavior						( CArrive *_pSteering );
	void						AddBehavior						( CCollisionAvoidance *_pSteering );
	void						AddBehavior						( CObstacleWallAvoidance *_pSteering );
	void						AddBehavior						( CWander *_pSteering );
	void						AddBehavior						( CSeparation *_pSteering );
	void						AddBehavior						( CAlignment *_pSteering );
	void						AddBehavior						( CCohesion *_pSteering );
	
	int							HasBehavior						( eBehaviorType type );

	//this function tests if a specific bit of m_iFlags is set
	bool						On								( eBehaviorType _bt )		{ 
																								return (m_iFlags & _bt) == _bt; 
																							}

	inline Vect2f				Truncate						(  Vect2f v, float _Max );

	void						CalculateNeighbors				( const Vect3f & _Position, float _ViewDistance );
	void						ClearNeighbors					( void );

	//---- Properties ( get & Set )---------------------------------------
	// Fuerza del steering creada por la combinación en la ejecución de todos los steerings
	const Vect3f&				GetSteeringForce				( void ) const 										{ return m_SteeringForce; }
	const float					GetMaxForce						( void ) const 										{ return m_MaxForce; }

	inline void					SetSeek							( CSeek * _Seek )									{ m_pSeek = _Seek; }
	CSeek*						GetSeek							( void );
	
	inline void					SetFlee							( CFlee * _Flee )									{ m_pFlee = _Flee; }
	CFlee*						GetFlee							( void );

	inline void					SetPursuit						( CPursuit * _Pursuit )								{ m_pPursuit = _Pursuit; }
	CPursuit*					GetPursuit						( void );

	inline void					SetEvade						( CEvade * _Evade )									{ m_pEvade = _Evade; }
	CEvade*						GetEvade						( void );

	inline void					SetArrive						( CArrive * _pArrive )								{ m_pArrive = _pArrive; }
	CArrive*					GetArrive						( void );

	inline void					SetWander						( CWander * _pWander )								{ m_pWander = _pWander; }
	CWander*					GetWander						( void );

	inline void					SetCollisionAvoidance			( CCollisionAvoidance * _pObstacleAvoidance )		{ m_pCollisionAvoidance = _pObstacleAvoidance; }
	CCollisionAvoidance*		GetCollisionAvoidance			( void );

	inline void					SetObstacleWallAvoidance		( CObstacleWallAvoidance * _pWallAvoidance )		{ m_pObstacleWallAvoidance = _pWallAvoidance; }
	CObstacleWallAvoidance*		GetObstacleWallAvoidance		( void );

	inline void					SetSeparation					( CSeparation * _pSeparation )						{ m_pSeparation = _pSeparation; }
	CSeparation*				GetSeparation					( void );
	
	inline void					SetAlignment					( CAlignment * _pAlignment )						{ m_pAlignment = _pAlignment; }
	CAlignment*					GetAlignment					( void );
	
	inline void					SetCohesion						( CCohesion * _pCohesion )							{ m_pCohesion= _pCohesion; }
	CCohesion*					GetCohesion						( void );

	void						FleeOn							( void )											{ m_iFlags |= flee; }
	void						SeekOn							( void )											{ m_iFlags |= seek; }
	void						ArriveOn						( void )											{ m_iFlags |= arrive; }
	void						WanderOn						( void )											{ m_iFlags |= wander; }
	void						PursuitOn						( void )											{ m_iFlags |= pursuit; /*m_pTargetAgent1 = v;*/ }
	void						EvadeOn							( void )											{ m_iFlags |= evade; /*m_pTargetAgent1 = v;*/ }
	void						CohesionOn						( void )											{ m_iFlags |= cohesion; }
	void						SeparationOn					( void )											{ m_iFlags |= separation; }
	void						AlignmentOn						( void )											{ m_iFlags |= alignment; }
	void						CollisionAvoidanceOn			( void )											{ m_iFlags |= collision_avoidance; }
	void						ObstacleWallAvoidanceOn			( void )											{ m_iFlags |= obstacle_wall_avoidance; }
	void						FollowPathOn					( void )											{ m_iFlags |= follow_path; }
	/*void						InterposeOn						( Vehicle* v1, Vehicle* v2 )						{ m_iFlags |= interpose; m_pTargetAgent1 = v1; m_pTargetAgent2 = v2; }
	void						HideOn							( Vehicle* v)										{ m_iFlags |= hide; m_pTargetAgent1 = v; }*/
	//void						OffsetPursuitOn					( Vehicle* v1, const Vector2D offset )				{ m_iFlags |= offset_pursuit; m_vOffset = offset; m_pTargetAgent1 = v1; }  
	//void						OffsetPursuitOn					( void )											{ m_iFlags |= offset_pursuit; m_vOffset = _Offset; } //m_pTargetAgent1 = v1; }  
	void						FlockingOn						( void )											{ CohesionOn(); AlignmentOn(); SeparationOn(); WanderOn(); }

	void						FleeOff							( void )											{ if(On(flee))   m_iFlags ^=flee; }
	void						SeekOff							( void )											{ if(On(seek))   m_iFlags ^=seek; }
	void						ArriveOff						( void )											{ if(On(arrive)) m_iFlags ^=arrive; }
	void						WanderOff						( void )											{ if(On(wander)) m_iFlags ^=wander; }
	void						PursuitOff						( void )											{ if(On(pursuit)) m_iFlags ^=pursuit; }
	void						EvadeOff						( void )											{ if(On(evade)) m_iFlags ^=evade; }
	void						CohesionOff						( void )											{ if(On(cohesion)) m_iFlags ^=cohesion; }
	void						SeparationOff					( void )											{ if(On(separation)) m_iFlags ^=separation; }
	void						AlignmentOff					( void )											{ if(On(alignment)) m_iFlags ^=alignment; }
	void						CollisionAvoidanceOff			( void )											{ if(On(collision_avoidance)) m_iFlags ^=collision_avoidance; }
	void						ObstacleWallAvoidanceOff		( void )											{ if(On(obstacle_wall_avoidance)) m_iFlags ^=obstacle_wall_avoidance; }
	void						FollowPathOff					( void )											{ if(On(follow_path)) m_iFlags ^=follow_path; }
	void						InterposeOff					( void )											{ if(On(interpose)) m_iFlags ^=interpose; }
	void						HideOff							( void )											{ if(On(hide)) m_iFlags ^=hide; }
	void						OffsetPursuitOff				( void )											{ if(On(offset_pursuit)) m_iFlags ^=offset_pursuit; }
	void						FlockingOff						( void )											{ CohesionOff(); AlignmentOff(); SeparationOff(); WanderOff(); }

	bool						isFleeOn						( void )											{ return On(flee); }
	bool						isSeekOn						( void )											{ return On(seek); }
	bool						isArriveOn						( void )											{ return On(arrive); }
	bool						isWanderOn						( void )											{ return On(wander); }
	bool						isPursuitOn						( void )											{ return On(pursuit); }
	bool						isEvadeOn						( void )											{ return On(evade); }
	bool						isCohesionOn					( void )											{ return On(cohesion); }
	bool						isSeparationOn					( void )											{ return On(separation); }
	bool						isAlignmentOn					( void )											{ return On(alignment); } 
	bool						isCollisionAvoidanceOn			( void )											{ return On(collision_avoidance); }
	bool						isObstacleWallAvoidanceOn		( void )											{ return On(obstacle_wall_avoidance); }
	bool						isFollowPathOn					( void )											{ return On(follow_path); }
	bool						isInterposeOn					( void )											{ return On(interpose); }
	bool						isHideOn						( void )											{ return On(hide); }
	bool						isOffsetPursuitOn				( void )											{ return On(offset_pursuit); }
    
//---- Members -------------------------------------------------------

protected:
	typedef std::list<eBehaviorType> tBehaviourType;

	/// Behaviors que van a ser ejecutados en el proceso update
    tBehaviourType		  * m_Behaviors;
	std::vector<CPhysicUserData*> m_UserDatas;


	CSteeringEntity			* m_pEntity;				// La entidad tratada

	eSummingMethod			  m_SummingMethod;			// Tipo de mètodo usado para el sumatorio de comportamientos activos
  
	Vect3f					  m_SteeringForce;			// Resultado de la suma de todas las fuerzas segun los steerings
	float					  m_MaxForce;				// Màxima fuerza que puede llegar a alcanzar el behaviour

	CSeek					* m_pSeek;					// Comportamiento de acercarse al objetivo de forma directa
	CFlee					* m_pFlee;					// Comportamiento de alejarse del objetivo de forma directa
	CPursuit				* m_pPursuit;				// Comportamiento de acercarse al objetivo de forma inteligente
	CEvade					* m_pEvade;					// Comportamiento de alejarse del objetivo de forma inteligente
	COffsetPursuit			* m_pOffsetPursuit;			// Comportamiento de acercarse al objetivo de forma inteligente pero manteniendo un offset entre ellos
	
	CArrive					* m_pArrive;				// Comportamiento de acercarse al objetivo de forma suave y frenando
	CCollisionAvoidance		* m_pCollisionAvoidance;	// Comportamiento al colisionar contra cualquier carácter del mundo
	CObstacleWallAvoidance	* m_pObstacleWallAvoidance;	// Comportamiento al colisionar contra cualquier muro u objecto del mundo
	CWander					* m_pWander;				// Comportamiento errático/aleatorio
	
	CSeparation				* m_pSeparation;			// Comportamiento para separarse de agentes vecinos relativa a la distancia
	CAlignment				* m_pAlignment;				// Comportamiento para alinear agentes vecinos relativa al heading
	CCohesion				* m_pCohesion;				// Comportamiento para cohesionar agentes vecinos relativa al centro de massas

	
	// Multiplicadores modificables para efectuar la fuerza del comportamiento adecuado.
	float			        m_WeightSeparation;
	float			        m_WeightCohesion;
	float			        m_WeightAlignment;
	float			        m_WeightWander;
	float			        m_WeightObstacleAvoidance;
	float			        m_WeightWallAvoidance;
	float			        m_WeightSeek;
	float			        m_WeightFlee;
	float			        m_WeightArrive;
	float			        m_WeightPursuit;
	float			        m_WeightOffsetPursuit;
	float			        m_WeightInterpose;
	float			        m_WeightHide;
	float			        m_WeightEvade;
	float			        m_WeightFollowPath;

	int						m_iFlags;				// Flag binario que indica si o no un comportamiento es activo
};	

#endif __STEERING_BEHAVIORS_CLASS_H__
