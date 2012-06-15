#pragma once

#ifndef __STEERING_BEHAVIOURS_CLASS_H__
#define __STEERING_BEHAVIOURS_CLASS_H__

#include <list>
#include "SteeringBehavioursDefs.h"
#include "Math\Vector2.h"

class CSteeringBehaviours
{
	//--- Init and End protocols------------------------------------------
				CSteeringBehaviours			( void );
	virtual		~CSteeringBehaviours		( void );

	//---- Main Functions ------------------------------------------------
	void		Initialize					( void );
	void		Destroy						( void );

	//---- Functions -----------------------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	int			HasBehavior					( BehaviorType type );
	
	// Fuerza del steering creada por la combinación en la ejecución de todos los steerings
	Vect2f		GetSteeringForce			( void )				{ return m_SteeringForce; }

	//---- Register Methods ----------------------------------------------
private:	
	void		RegisterLUAMethods			( void );

    //---- Members -------------------------------------------------------

protected:
	typedef std::list<BehaviorType> tBehaviourType;

	/// Behaviors que van a ser ejecutados en el proceso update
    std::list<BehaviorType> m_behaviors;

	Vect2f					m_SteeringForce;
};

#endif __STEERING_BEHAVIOURS_CLASS_H__