//----------------------------------------------------------------------------------
// CPhysicsManager class
// Author: Enric Vergara
//
// Description:
// This secures availability of the necessary physic functions.
// It internally uses the PhysX library.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSICS_MANAGER_H_
#define INC_PHYSICS_MANAGER_H_

#include "PhysicsDefs.h"
#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "Math/Vector3.h"
#include "Utils\Named.h"
#include <vector>
#include <string>
#include <map>
//#include "Script/ScriptRegister.h"

//---Forward Declarations---
class NxPhysicsSDK;
class NxScene;
class NxControllerManager;
class CPhysicUserAllocator;
class CPhysicCookingMesh;
class CPhysicActor;
class NxActor;
class NxCCDSkeleton;
class CPhysicController;
class CRenderManager;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CPhysicFixedJoint;
class CPhysicTriggerReport;
class CPhysicCollisionReport;
class CScriptManager;
class CGameEntity;

int GetCollisionGroup(const std::string& _szGroup);

class CPhysicsManager
{
public:
	//--- Init and End protocols	
	CPhysicsManager();
	~CPhysicsManager()		{ Done(); }

	bool					Init						( const std::string& _physXConfig );
	void					Done						();
	bool					IsOk						() const									{ return m_bIsOk; }
	void					SetInitParams				( const SPhysicsInitParams& initParams )	{ m_InitParams = initParams; }

	//----CScriptRegister interface-------------------
	virtual void			RegisterFunctions			(CScriptManager* scriptManager);

	//--- Intersection Functions:	
	CPhysicUserData*		RaycastClosestActor			(const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info);
	CPhysicUserData*		RaycastClosestActorShoot	(const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info, float _fPower);
	void					OverlapSphereActor			(float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> &impactObjects, uint32 impactMask);
	void					OverlapSphereActorGrenade	(float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects, float _fPower);
	void					ApplyExplosion              (NxActor* _pActor, const Vect3f& _vPosSphere, float _fEffectRadius, float _fPower);


	//--- Get CookingMesh
	CPhysicCookingMesh*				GetCookingMesh					() const					{ return m_pCookingMesh; }

	//--- Rendering Stuff:
	void							DebugRender						(CRenderManager* render);
	void							SetDebugRenderMode				( bool flag )				{ m_bDebugRenderMode = flag; }
	bool							GetDebugRenderMode				() const					{ return m_bDebugRenderMode; }
	NxScene*						GetScene						() const					{ return m_pScene; }	
	//----Update
	void							Update							( float elapsedTime );
	void							WaitForSimulation				();

	//--- Add/Release Actors
	bool							AddPhysicActor					(CPhysicActor* actor);
	bool							ReleasePhysicActor				(CPhysicActor* actor);

	//--- Add/Release CharacterControllers
	bool							AddPhysicController				(CPhysicController* controller);
	bool							ReleasePhysicController			(CPhysicController* controller);

	//--- Add/Release Joints
	bool							AddPhysicSphericalJoint			(CPhysicSphericalJoint* joint);
	bool							RelasePhysicSphericalJoint		(CPhysicSphericalJoint* joint);
	bool							AddPhysicRevoluteJoint			(CPhysicRevoluteJoint* joint);
	bool							RelasePhysicRevoluteJoint		(CPhysicRevoluteJoint* joint);
	bool							AddPhysicFixedJoint				(CPhysicFixedJoint* joint);
	bool							RelasePhysicFixedJoint			(CPhysicFixedJoint* joint);

	//--- Create CCDSkeleton
	NxCCDSkeleton*					CreateCCDSkeleton					(float size);

	void							SetTriggerReport					(CPhysicTriggerReport* report);
	void							SetCollisionReport					(CPhysicCollisionReport* report);
  
	int								GetCollisionMask		(ECollisionGroup _szGroup)			{ return m_CollisionMasks[_szGroup]; }
	const std::string&				GetConfigFileName		() const							{ return m_szConfigFileName; }
		
private:
	void					Release				();
	void					DrawActor			(NxActor* actor, CRenderManager* render);

private:
	bool					m_bIsOk;
	bool					m_bDebugRenderMode;

	std::string             m_szConfigFileName;
	std::map<int, int>		m_CollisionMasks;

	//---PhysX------------------------------
	NxPhysicsSDK*			m_pPhysicsSDK;
	NxScene*				m_pScene;
	NxControllerManager*	m_pControllerManager;
	CPhysicUserAllocator*	m_pMyAllocator;
	CPhysicCookingMesh*		m_pCookingMesh;
	SPhysicsInitParams		m_InitParams;
	//-------------------------------------------
};

#endif // INC_PHYSICS_MANAGER_H_