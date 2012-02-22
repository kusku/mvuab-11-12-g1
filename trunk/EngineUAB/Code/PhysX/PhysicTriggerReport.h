//----------------------------------------------------------------------------------
// CPhysicTrigger class
// Author: Enric Vergara
//
// Description:
// Clase abstracta la cual ofrece la interfaz para poder controlar los eventos de entrar/salir
// de los triggers contenidos en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef _PHYSIC_TRIGGER_REPORT_H
#define _PHYSIC_TRIGGER_REPORT_H


//---Engine Includes---//
#undef min
#undef max
#include "NxPhysics.h"
#include "NxShape.h"
//---------------------//

class CPhysicUserData;

class CPhysicTriggerReport: public NxUserTriggerReport   
{    
public:
	CPhysicTriggerReport() {;}
	virtual ~CPhysicTriggerReport() {/*Nothing*/;}

	//---NxUserTriggerReport Interface---
	void					onTrigger	(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);
	//-----------------------------------

	//---CPhysicTrigger Interface---
	virtual void	OnEnter		(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape) = 0;
	virtual	void	OnLeave		(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape) = 0;
};

#endif // _PHYSIC_TRIGGER_REPORT_H