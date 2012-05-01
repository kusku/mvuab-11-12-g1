#pragma once

#ifndef _TRIGGER_H
#define _TRIGGER_H

#include "PhysicTriggerReport.h"

class CPhysicsManager;

class CTrigger : public CPhysicTriggerReport	
{
public:
	CTrigger();
	~CTrigger();

	void	OnEnter		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );
	void	OnLeave		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );
	void	OnStay		( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );
};

#endif
