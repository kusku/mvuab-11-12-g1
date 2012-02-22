#ifndef _PHYSIC_COLLISION_REPORT_H
#define _PHYSIC_COLLISION_REPORT_H
#pragma once

#include "NxPhysics.h"

class CPhysicUserData;

class CPhysicCollisionReport : public NxUserContactReport
{
public:
  void  onContactNotify(NxContactPair& _pair, NxU32 _events);

  virtual void	OnStartTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape) = 0;
  virtual	void	OnTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape) = 0;
  virtual	void	OnEndTouch(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape) = 0;
  virtual void	OnStartTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape) = 0;
  virtual	void	OnTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape) = 0;
  virtual	void	OnEndTouchForceThreshold(CPhysicUserData* _pEntity, CPhysicUserData* _pOther_shape) = 0;
};

#endif
