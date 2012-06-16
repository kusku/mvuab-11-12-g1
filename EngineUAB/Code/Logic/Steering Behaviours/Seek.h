#pragma once

#ifndef __CLASS_SEEK_H__
#define __CLASS_SEEK_H__

#include "Math\Vector2.h"
#include "Steering Behaviours\Steering.h"

//--- Foward Declarations ---
class CSteeringEntity;
//---------------------------

class CSeek : public CSteering
{
public:
	//--- Init and End protocols------------------------------------------
							CSeek			( void );
	virtual					~CSeek			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect2f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	// inline eBehaviorType		GetType				( void ) const		{ return m_Type; }		// Tipo del steering concreto, se establece en el constructor
		
	//---- Register Methods -------------------------------------
	static void				RegisterLUAMethods	( void );
	
	
	//---- Members -----------------------------------------------
private:


};

#endif __CLASS_SEEK_H__