#define __DONT_INCLUDE_MEM_LEAKS__
#include "base.h"
#include "PhysicTriggerReport.h"
//#include "PhysicsManager.h"

void CPhysicTriggerReport::onTrigger( NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status )       
{           
	if(status & NX_TRIGGER_ON_ENTER)           
	{               
		//A body entered the trigger area for the first time               
		NxActor* actor = &triggerShape.getActor();
		CPhysicUserData* entity_trigger1= (CPhysicUserData*)actor->userData;  

		actor = &otherShape.getActor();
		CPhysicUserData* entity_trigger2= (CPhysicUserData*)actor->userData;  
		OnEnter(entity_trigger1, entity_trigger2);
	}            
	if(status & NX_TRIGGER_ON_LEAVE)            
	{                
		NxActor* actor = &triggerShape.getActor();
		CPhysicUserData* entity_trigger1= (CPhysicUserData*)actor->userData;  

		actor = &otherShape.getActor();
		CPhysicUserData* entity_trigger2= (CPhysicUserData*)actor->userData;  
		OnLeave(entity_trigger1, entity_trigger2);         
	}           
}  