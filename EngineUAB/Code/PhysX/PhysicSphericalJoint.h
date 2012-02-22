//----------------------------------------------------------------------------------
// CPhysicSphericalJoint class
// Author: Enric Vergara
//
// Description:
// Esta clase representa joint de tipo esferico
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_SPHERICAL_JOINT_H_
#define INC_PHYSIC_SPHERICAL_JOINT_H_

#include "Math/Vector3.h"

//---Forward Declarations---
class NxJoint;
class NxSphericalJointDesc;
class CPhysicActor;
//--------------------------


struct SSphericalLimitInfo
{
	Vect3f m_vAnchor, m_vAxis;
	bool  TwistLimit;
	float TwistLowValue, TwistLowRestitution, TwistHighValue, TwistHighRestitution;

	bool  SwingLimit;
	float SwingValue, SwingRestitution;

	bool  TwistSpring;
	float TwistSpringValue, TwistSpringDamper;

	bool  SwingSpring;
	float SwingSpringValue, SwingSpringDamper;

	bool  JointSpring;
	float JointSpringValue, JointSpringDamper;
};


class CPhysicSphericalJoint
{
public:
	CPhysicSphericalJoint();
	~CPhysicSphericalJoint();

	void									CreateJoint			(NxJoint* joint);
	void									SetInfoComplete	(const Vect3f& anchor, const Vect3f& _axis, CPhysicActor* actorA,  CPhysicActor* actorB =0);
  void									SetInfoRagdoll	(SSphericalLimitInfo _sInfo, CPhysicActor* actorA,  CPhysicActor* actorB =0);
  void									SetInfo					(const Vect3f& anchor, CPhysicActor* actorA,  CPhysicActor* actorB =0);


	//---Get PhsX Info---
	NxJoint*							GetPhXJoint			() {return m_pJoint;}
	NxSphericalJointDesc*	GetPhXDescJoint	() {return m_pSphericalDesc;}

private:
	NxJoint*							m_pJoint;
	NxSphericalJointDesc* m_pSphericalDesc;

};

#endif //INC_PHYSIC_SPHERICAL_JOINT_H_