#pragma once

#ifndef __STEERING_BEHAVIOURS_CLASS_H__
#define __STEERING_BEHAVIOURS_CLASS_H__

#include <list>
#include "SteeringBehavioursDefs.h"
#include "Math\Vector2.h"

// --- Foward Declarations ---
class CSteeringEntity;
class CSeek;
class CPursuit;
class CArrive;
// ---------------------------

class CSteeringBehaviours
{
public:
	//--- Init and End protocols------------------------------------------
							CSteeringBehaviours			( float _MaxForce );
	virtual					~CSteeringBehaviours		( void );

	//---- Main Functions ------------------------------------------------
	void					Initialize					( void );
	void					Destroy						( void );
	const Vect2f&			Update						( float _ElapsedTime, CSteeringEntity * _pEntity);

	//---- Functions -----------------------------------------------------
	void					AddBehavior					( CSeek *_pSteering );
	void					AddBehavior					( CPursuit *_pSteering );
	void					AddBehavior					( CArrive *_pSteering );

	int						HasBehavior					( eBehaviorType type );
	
	inline Vect2f			Truncate					(  Vect2f v, float _Max );

	//---- Properties ( get & Set )---------------------------------------
	// Fuerza del steering creada por la combinación en la ejecución de todos los steerings
	const Vect2f&			GetSteeringForce			( void ) const 					{ return m_SteeringForce; }
	const float				GetMaxForce					( void ) const 					{ return m_MaxForce; }

	inline void				SetSeek						( CSeek * _Seek )				{ m_pSeek = _Seek; }
	inline CSeek*			GetSeek						( void );
	
	inline void				SetPursuit					( CPursuit * _Pursuit )			{ m_pPursuit = _Pursuit; }
	inline CPursuit*		GetPursuit					( void );

	inline void				SetArrive					( CArrive * _pArrive )		{ m_pArrive = _pArrive; }
	inline CArrive*			GetArrive					( void );

	//---- Register Methods ----------------------------------------------
	static void				RegisterLUAMethods			( void );

    //---- Members -------------------------------------------------------

protected:
	typedef std::list<eBehaviorType> tBehaviourType;

	/// Behaviors que van a ser ejecutados en el proceso update
    tBehaviourType		  * m_Behaviors;

	Vect2f					m_SteeringForce;		// Resultado de la suma de todas las fuerzas segun los steerings
	float					m_MaxForce;				// Màxima fuerza que puede llegar a alcanzar el behaviour
	CSeek				  * m_pSeek;				// Comportamiento de acercarse al objetivo de forma directa
	CPursuit			  * m_pPursuit;				// Comportamiento de acercarse al objetivo de forma inteligente
	CArrive				  * m_pArrive;				// Comportamiento de acercarse al objetivo de forma suave y frenando

	float					m_WeightObstacleAvoidance;
	float					m_WeightSeek;
	float					m_WeightEvade;
	float					m_WeightFlee;
	float					m_WeightArrive;
	float					m_WeightPursuit;
	float					m_WeightWander;
};

#endif __STEERING_BEHAVIOURS_CLASS_H__
