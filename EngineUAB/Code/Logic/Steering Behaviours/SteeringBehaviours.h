#pragma once

#ifndef __STEERING_BEHAVIOURS_CLASS_H__
#define __STEERING_BEHAVIOURS_CLASS_H__

#include <list>
#include "SteeringBehavioursDefs.h"

class CSteeringBehaviours
{
	//--- Init and End protocols------------------------------------------
				CSteeringBehaviours			( void );
	virtual		~CSteeringBehaviours		( void );

	//---- Main Functions ---------------------------------------
	void		Initialize					( void );
	void		Destroy						( void );

	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	int			HasBehavior						( BehaviorType type );

	//---- Register Methods -------------------------------------
private:	
	void		RegisterLUAMethods			( void );

    

protected:
	typedef std::list<BehaviorType> tBehaviourType;

	/// Behaviors que van a ser ejecutados en el proceso update
    std::list<BehaviorType> m_behaviors;
};

#endif __STEERING_BEHAVIOURS_CLASS_H__