#include "PhysicCollisionReport.h"

void  CPhysicCollisionReport::onContactNotify(NxContactPair& _pair, NxU32 _events)
{
  if(_pair.isDeletedActor[0] || _pair.isDeletedActor[1])
    return;

  NxActor* actor = _pair.actors[0];
	CPhysicUserData* entity_collision = (CPhysicUserData*)actor->userData;  

	actor = _pair.actors[1];
	CPhysicUserData* entity_collision2= (CPhysicUserData*)actor->userData;

	if(_events & NX_NOTIFY_ON_START_TOUCH)           
	{                           
		OnStartTouch(entity_collision, entity_collision2);
	}            
  if(_events & NX_NOTIFY_ON_END_TOUCH)            
	{                
		OnEndTouch(entity_collision, entity_collision2);         
	}
  if(_events & NX_NOTIFY_ON_TOUCH)            
	{                
		OnTouch(entity_collision, entity_collision2);         
	}
  if(_events & NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD)           
	{                           
		OnStartTouchForceThreshold(entity_collision, entity_collision2);
	}            
  if(_events & NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD)            
	{                
		OnEndTouchForceThreshold(entity_collision, entity_collision2);         
	}
  if(_events & NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD)            
	{                
		OnTouchForceThreshold(entity_collision, entity_collision2);         
	}
}