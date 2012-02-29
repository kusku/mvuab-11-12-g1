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

#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "Math/Vector3.h"
#include "Utils\Named.h"
#include <vector>
#include <string>
#include <map>
//#include "Script/ScriptRegister.h"

using namespace std;

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

//--------------------------



enum ECollisionGroup {
  ECG_ESCENARI,
  ECG_PERSONATGE,
  ECG_ENEMICS,
  ECG_TRIGGERS,
  ECG_MALGLOBAL,
  ECG_COBERTURES,
  ECG_OBJECTES_DINAMICS,
  ECG_EXPLOSIONS,
  ECG_RAY_SHOOT,
  ECG_RAY_SHOOT_PLAYER,
  ECG_FORCE,
  ECG_RAY_IA_GRAPH,
  ECG_CAMERA,
  ECG_RAGDOLL,
  ECG_RAGDOLL_PLAYER,
  ECG_VIGIA,

  //aquest ha de ser l'últim sempre!!!!
  ECG_LAST_GROUP
};

int GetCollisionGroup(const std::string& _szGroup);

//---- Declaracion de nuevos tipos------------
struct SPhysicsInitParams
{
	SPhysicsInitParams::SPhysicsInitParams():	m_fSkinWidth(0.001f), m_fGravity(-9.81f), 
																						m_Restitution_DefMat(0.5f), m_StaticFriction_DefMat(0.5f),
																						m_DynamicFriction_DefMat(0.5f) {}
	float		m_fSkinWidth;
	float		m_fGravity;
	float		m_Restitution_DefMat;				//Restitution of the default material
	float		m_StaticFriction_DefMat;		//Static Friction of the default material
	float		m_DynamicFriction_DefMat;		//Dynamic Friction of the default material
};

struct SCollisionInfo
{
	float		m_fDistance;
	Vect3f	m_Normal;
	Vect3f	m_CollisionPoint;
};

class CGameEntity;

class CPhysicUserData : public CNamed
{
public:
	CPhysicUserData(const std::string& _szName) : CNamed(_szName), m_bPaintPhysicObject(false), m_ColorPhysicObject(colWHITE), m_pEntity(0),m_pActor(0) {}
	~CPhysicUserData() {/*Nothing*/}

	void					SetPaint	(bool flag)						  {m_bPaintPhysicObject = flag;};
	void					SetColor	(const CColor& color)   {m_ColorPhysicObject = color;};
	void					SetEntity (CGameEntity* _pEntity) {m_pEntity = _pEntity;};
	void					SetActor (CPhysicActor* _pActor) {m_pActor = _pActor;};

	bool					GetPaint	() const							{return m_bPaintPhysicObject;};
	const CColor&	GetColor	() const							{return m_ColorPhysicObject;};
	CGameEntity* 	GetEntity	() const							{return m_pEntity;};
  CPhysicActor*      GetActor() {return m_pActor;};


private:
	bool		m_bPaintPhysicObject;
	CColor	m_ColorPhysicObject;
  CPhysicActor* m_pActor;
  CGameEntity* m_pEntity;
};
//--------------------------------------------

class CPhysicsManager
{

public:
	//--- Init and End protocols	
	CPhysicsManager()	:	m_bIsOk(false), m_bDebugRenderMode(true),
											m_pPhysicsSDK(0), m_pScene(0),
											m_pControllerManager(0), m_pMyAllocator(0),
											m_pCookingMesh(0), m_InitParams() {}

	~CPhysicsManager() {Done();}

	bool								Init										(const std::string& _physXConfig);
	void								Done										();
	bool								IsOk										() const { return m_bIsOk; }
	void								SetInitParams								(const SPhysicsInitParams& initParams) {m_InitParams = initParams;}

	//----CScriptRegister interface-------------------
	virtual void				RegisterFunctions					  (CScriptManager* scriptManager);

	//--- Intersection Functions:	
	CPhysicUserData*		RaycastClosestActor			(const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info);
	CPhysicUserData*		RaycastClosestActorShoot	(const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info, float _fPower);
	void					OverlapSphereActor			(float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> &impactObjects, uint32 impactMask);
	void					OverlapSphereActorGrenade	(float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects, float _fPower);
	void					ApplyExplosion              (NxActor* _pActor, const Vect3f& _vPosSphere, float _fEffectRadius, float _fPower);


	//--- Get CookingMesh
	CPhysicCookingMesh*	GetCookingMesh							() {return m_pCookingMesh;}


	//--- Rendering Stuff:
	void								DebugRender							(CRenderManager* render);
	void								SetDebugRenderMode					(bool flag)					{ m_bDebugRenderMode = flag; }
	bool								GetDebugRenderMode					() const					{ return m_bDebugRenderMode; }
	NxScene*							GetScene							() const					{ return m_pScene; }	
	//----Update
	void								Update						(float elapsedTime);
	void								WaitForSimulation           ();

	//--- Add/Release Actors
	bool								AddPhysicActor					(CPhysicActor* actor);
	bool								ReleasePhysicActor				(CPhysicActor* actor);

	//--- Add/Release CharacterControllers
	bool								AddPhysicController				(CPhysicController* controller);
	bool								ReleasePhysicController			(CPhysicController* controller);

	//--- Add/Release Joints
	bool								AddPhysicSphericalJoint			(CPhysicSphericalJoint* joint);
	bool								RelasePhysicSphericalJoint		(CPhysicSphericalJoint* joint);
	bool								AddPhysicRevoluteJoint			(CPhysicRevoluteJoint* joint);
	bool								RelasePhysicRevoluteJoint		(CPhysicRevoluteJoint* joint);
	bool								AddPhysicFixedJoint				(CPhysicFixedJoint* joint);
	bool								RelasePhysicFixedJoint			(CPhysicFixedJoint* joint);

	//--- Create CCDSkeleton
	NxCCDSkeleton*					CreateCCDSkeleton					(float size);

	void							SetTriggerReport					(CPhysicTriggerReport* report);
	void							SetCollisionReport					(CPhysicCollisionReport* report);
  
  int GetCollisionMask (ECollisionGroup _szGroup) {return m_CollisionMasks[_szGroup];};


  std::string GetConfigFileName() const {return m_szConfigFileName;};

private:
	void								Release											();
	void								DrawActor										(NxActor* actor, CRenderManager* render);


private:
	bool									m_bIsOk;
	bool									m_bDebugRenderMode;

  std::string                m_szConfigFileName;

  map<int, int> m_CollisionMasks;

	//---PhysX------------------------------
	NxPhysicsSDK*					m_pPhysicsSDK;
	NxScene*							m_pScene;
	NxControllerManager*	m_pControllerManager;
	CPhysicUserAllocator*	m_pMyAllocator;
	CPhysicCookingMesh*		m_pCookingMesh;
	SPhysicsInitParams		m_InitParams;
	//-------------------------------------------
};

#endif // INC_PHYSICS_MANAGER_H_