#pragma once

#ifndef _PHYSICS_CONTROLLER_HIT_REPORT_H
#define _PHYSICS_CONTROLLER_HIT_REPORT_H

class CPhysicsControllerHitReport : public NxUserControllerHitReport
{
	public:

	virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit)
	{
		if(hit.shape)
		{
			NxCollisionGroup group = hit.shape->getGroup();
			if ( group == ECG_DYNAMIC_OBJECTS )
			{
				NxActor& actor = hit.shape->getActor();

				if(hit.dir.y==0.0f)
				{
					NxF32 coeff = actor.getMass() * hit.length * 10.0f;
					actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);
				}
			}
		}

		return NX_ACTION_NONE;
	}

	virtual NxControllerAction onControllerHit(const NxControllersHit& hit)
	{
		return NX_ACTION_NONE;
	}
};

#endif