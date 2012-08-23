#pragma once

#ifndef __CLASS_EVADE_H__
#define __CLASS_EVADE_H__

#include "Math\Vector3.h"
#include "Steering Behaviors\Steering.h"

//--- Foward Declarations ---
class CSteeringEntity;
class CFlee;
//---------------------------

class CEvade : public CSteering
{
public:
	//--- Init and End protocols------------------------------------------
							CEvade			( float _PanicDistance );
	virtual					~CEvade			( void );

	//---- Main Methods --------------------------------------------------
	virtual	Vect3f			CalculateSteering	( CSteeringEntity *_pEntity );					// Método que deberá ser sobreescrito en cada clase que herede de Steering

	//---- Methods -------------------------------------------------------
	void					UpdatePursuerEntity( CSteeringEntity *_pPursuer, float _PanicDistance );

	//---- Properties ( get & Set )---------------------------------------


	//---- Members -------------------------------------------------------
private:
	CSteeringEntity *	m_pPursuer;
	CFlee			*	m_pFlee;
	bool				m_PursuerUpdated;
	float				m_PanicDistance;
};

#endif __CLASS_EVADE_H__