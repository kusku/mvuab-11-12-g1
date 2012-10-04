#pragma once

#ifndef __FORMATION_PATTERN_CLASS_H__
#define __FORMATION_PATTERN_CLASS_H__

#include <string>

#include "FormationManager.h"

// -- FOWARD Declarations -----
// ----------------------------

class CFormationPattern
{
public:
	//--- Init and End protocols------------------------------------------
					CFormationPattern	();
					~CFormationPattern	();

	//----Main Functions ---------------------------------------------------
	float			GetDriftOffset		( CFormationManager::SlotAssignment _SlotAssignment );		// Calcula el drift offset cuando caracteres estan en un dado conjunto de slots
	int				GetSlotLocation		( int _SlotNumber );					// Obtiene la localización de un dado indice de slot
	
	//---- Properties -----------------------------------------------------
	bool			SupportsSlots		( int _SlotNumber );					// Retorna true si el patron puede soportar el numero de slots pasado
	//---- Members --------------------------------------------------------
private:
	float			m_NumberOfSlots;		// Representa el total de slots disponibles
};
#endif //__FORMATION_PATTERN_CLASS_H__