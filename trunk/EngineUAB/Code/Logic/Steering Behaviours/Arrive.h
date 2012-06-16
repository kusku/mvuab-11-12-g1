#pragma once

#ifndef __CLASS_ARRIVE_H__
#define __CLASS_ARRIVE_H__

#include "Math\Vector2.h"
#include "Steering Behaviours\Steering.h"
#include "SteeringBehavioursDefs.h"

//--- Foward Declarations ---
class CSteeringEntity;
//---------------------------

class CArrive : public CSteering
{
public:


	//--- Init and End protocols------------------------------------------
							CArrive				( eDeceleration _Deceleration, float _DecelerationBeginsAtDistance );
	virtual					~CArrive			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect2f			CalculateSteering	( CSteeringEntity *_pEntity );					// M�todo que deber� ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	inline eBehaviorType	GetType			( void ) const						{ return m_Type; }			// Tipo del steering concreto, se establece en el constructor
	
	inline const Vect2f&	GetTarget		( void ) 							{ return (m_Target); }		
	inline void				SetTarget		( const Vect2f & _Target ) 			{ m_Target = _Target; }		

	//---- Register Methods -------------------------------------
	
	//---- Members -----------------------------------------------
private:
		Vect2f			m_Target;
		eDeceleration	m_Deceleration;
		float	 		m_DecelerationBeginsAtDistance;
};

#endif __CLASS_ARRIVE_H__