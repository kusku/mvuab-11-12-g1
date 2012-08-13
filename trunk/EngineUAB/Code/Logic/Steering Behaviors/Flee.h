#pragma once

#ifndef __CLASS_FLEE_H__
#define __CLASS_FLEE_H__

#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"

//--- Foward Declarations ---
class CSteeringEntity;
//---------------------------

class CFlee : public CSteering
{
public:
	//--- Init and End protocols------------------------------------------
							CFlee			( float _PanicDistance );
	virtual					~CFlee			( void );

	//---- Main Functions ---------------------------------------
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering

	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------

	inline void					SetPanicDistance	( float _PanicDistance )		{ m_PanicDistance = _PanicDistance; }
	inline const float			GetPanicDistance	( void ) const					{ return m_PanicDistance; }


	//---- Register Methods -------------------------------------
	static void				RegisterLUAMethods	( void );


	//---- Members -----------------------------------------------
private:
	float m_PanicDistance;

};

#endif __CLASS_FLEE_H__