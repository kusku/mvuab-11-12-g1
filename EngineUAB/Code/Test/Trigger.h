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

	void	OnEnter		(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape);
	void	OnLeave		(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape);
};

#endif
