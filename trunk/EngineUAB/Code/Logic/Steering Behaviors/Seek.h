#pragma once

#ifndef __CLASS_SEEK_H__
#define __CLASS_SEEK_H__

#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"

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
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering
    
	//---- Functions ---------------------------------------

	//---- Properties ( get & Set )---------------------------------------
	
	//---- Members -----------------------------------------------
private:


};

#endif __CLASS_SEEK_H__