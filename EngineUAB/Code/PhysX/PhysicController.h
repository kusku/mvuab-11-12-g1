//----------------------------------------------------------------------------------
// CPhysicController class
// Author: Enric Vergara
//
// Description:
// Esta clase representa un controlador de caracter.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_CONTROLLER_H
#define INC_PHYSIC_CONTROLLER_H

#include "Jump.h"
#include "Object3d.h"

//---Forward Declarations---
class NxController;
class NxCapsuleControllerDesc;
class NxScene;
class CPhysicUserData;
class CPhysicsControllerHitReport;

//--------------------------

enum CollisionGroup
{
	GROUP_NON_COLLIDABLE,
	GROUP_COLLIDABLE_NON_PUSHABLE,
	GROUP_COLLIDABLE_PUSHABLE,
};

class CPhysicController : public CObject3D
{
public:
	CPhysicController(	float radius, float height, float slope, float skinwidth, 
		float stepOffset, uint32 collisionGroups, CPhysicUserData* userData, 
		const Vect3f& pos = Vect3f(0.f,0.f,0.f), float gravity = -9.8f );

	~CPhysicController();

	CPhysicUserData*			GetUserData				() const			{ return m_pUserData; }
	void						SetCollision			(bool flag);
	void						Move					(const Vect3f& direction, float elapsedTime);
	void						Jump					(float ammount);
	bool						UpdateCharacterExtents	(bool bent, float ammount);

	Vect3f						GetPosition				();
	void						SetPosition				(const Vect3f& pos);
	float						GetGravity				() const				{ return m_fGravity; }
	void						SetGravity				(float gravity)			{ m_fGravity = gravity; }
	void						SetGroup                (int _iGroup);
	void						SetHeight               (float _fHeight);
	void						SetActive               (bool _bActive);
	//---PhsX Info---
	NxController*				GetPhXController			() const			{ return m_pPhXController; }
	NxCapsuleControllerDesc*	GetPhXControllerDesc		() const			{ return m_pPhXControllerDesc; }

	void						CreateController				(NxController* controller, NxScene* scene);

	void						UseGravity				(bool _bUseGravity)		{ m_bUseGravity = _bUseGravity; }

private:

	CPhysicUserData					*m_pUserData;
  //CPhysicsControllerHitReport m_Report;

	void							*m_Report;
	uint32							m_uCollisionGroups;
	CJump							m_Jump;
	float							m_fGravity;
	float							m_fRadius_Capsule;
	float							m_fHeight_Capsule;
	float							m_fSlopeLimit_Capsule;
	float							m_fSkinWidth_Capsule;
	float							m_fStepOffset_Capsule;

  bool m_bUseGravity;

	//--- PhysX--
	NxCapsuleControllerDesc			*m_pPhXControllerDesc;
	NxController*					m_pPhXController;
	NxScene*						m_pPhXScene;

};

#endif //INC_PHYSIC_CONTROLLER_H