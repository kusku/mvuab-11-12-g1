//----------------------------------------------------------------------------------
// CPhysicsManager class
// Author: Enric Vergara
// Changed: Jordi Arenas
// Description:
// This secures availability of the necessary physic functions.
// It internally uses the PhysX library.
//----------------------------------------------------------------------------------
#pragma once

#ifndef __PHYSX_MANAGER_CLASS_H__
#define __PHYSX_MANAGER_CLASS_H__

#include <vector>
#include <string>
#include <map>

#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "Math/Vector3.h"
#include "Utils\Named.h"

#include "PhysicsDefs.h"

//#include "Script/ScriptRegister.h"

//---Forward Declarations---//
class NxPhysicsSDK;
class NxScene;
class NxActor;
class NxCCDSkeleton;
class NxControllerManager;

class CPhysicActor;
class CPhysicCollisionReport;
class CPhysicCookingMesh;
class CPhysicFixedJoint;
class CPhysicController;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CPhysicTriggerReport;
class CPhysicUserAllocator;
class CPhysicUserData;
//class CScriptManager;
class CGameEntity;
class CRenderManager;
//--------------------------

class CPhysicsManager
{
public:
	//--- Init and End protocols------------------------------------------
							CPhysicsManager		();
	virtual					~CPhysicsManager	()		{ Done(); }

	//---- Main Functions ---------------------------------------
	bool					Init				();
	void					Done				();
	bool					IsOk				() const						{ return m_bIsOk; }
	bool					Load				( const std::string &_PhysXConfig );
	bool					Reload				();
	
	//--- Rendering Stuff:
	void					DebugRender			( CRenderManager *_RM );
	void					DrawActor			( NxActor* actor, CRenderManager* _RM );

	//----CScriptRegister interface---------------------------------------
	static void				RegisterMethods		();

	//--- Add/Release Actors
	bool					AddPhysicActor					( CPhysicActor* _pActor);
	bool					ReleasePhysicActor				( CPhysicActor* _pActor);
	
	bool					ReleaseAllActors				(); //EUserDataFlag _eFlags );

	//--- Add/Release CharacterControllers
	bool					AddPhysicController				( CPhysicController* _pController, EControleType _Tipus = ::CAPSULE );
	bool					ReleasePhysicController			( CPhysicController* _pController );

	////--- Add/Release Joints
	bool					AddPhysicSphericalJoint			( CPhysicSphericalJoint* _pJoint );
	bool					RelasePhysicSphericalJoint		( CPhysicSphericalJoint* _pJoint );
	bool					AddPhysicRevoluteJoint			( CPhysicRevoluteJoint* _pJoint );
	bool					RelasePhysicRevoluteJoint		( CPhysicRevoluteJoint* _pJoint );
	bool					AddPhysicFixedJoint				( CPhysicFixedJoint* _pJoint );
	bool					RelasePhysicFixedJoint			( CPhysicFixedJoint* _pJoint );

	////--- Intersection Functions:	
	CPhysicUserData*		RaycastClosestActor				( const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info );
	CPhysicUserData*		RaycastClosestActorShoot		( const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info, float _fPower );
	void					OverlapSphereActor				( float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> &impactObjects, uint32 impactMask );
	void					OverlapSphereActorGrenade		( float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects, float _fPower );
	void					ApplyExplosion					( NxActor* _pActor, const Vect3f& _vPosSphere, float _fEffectRadius, float _fPower );

	//----Update
	void					Update							( float _ElapsedTime );
	void					WaitForSimulation				();

	////--- Create CCDSkeleton
	NxCCDSkeleton*			CreateCCDSkeleton				( float size );

	////---- Properties ( get & Set )---------------------------------------
	void					SetDebugRenderMode				( bool _Flag )								{ m_bDebugRenderMode = _Flag; }
	bool					GetDebugRenderMode				( void ) const								{ return m_bDebugRenderMode; }
	
	NxScene*				GetScene						() const									{ return m_pScene; }	
	
	void					SetInitParams					( const SPhysicsInitParams& initParams )	{ m_InitParams = initParams; }
	
	//--- Get CookingMesh
	CPhysicCookingMesh*		GetCookingMesh					() const							{ return m_pCookingMesh; }
	NxPhysicsSDK*			GetPhysicsSDK					()									{ return m_pPhysicsSDK; }
	CPhysicActor*			GetActor						( std::string _ActorName );

	void					SetTriggerReport				( CPhysicTriggerReport* _pReport );
	void					SetCollisionReport				( CPhysicCollisionReport* _pReport );
  
	int						GetCollisionMask				( ECollisionGroup _szGroup )		{ return m_CollisionMasks[_szGroup]; }
	const std::string&		GetConfigFileName				() const							{ return m_szConfigFileName; }
	int						GetCollisionGroup				( const std::string& _szGroup );

private:
	bool					LoadXML				();
	void					Release				();

private:
	bool					m_bIsOk;
	bool					m_bDebugRenderMode;
	std::string				m_szConfigFileName;
	
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

#endif __PHYSX_MANAGER_CLASS_H__


