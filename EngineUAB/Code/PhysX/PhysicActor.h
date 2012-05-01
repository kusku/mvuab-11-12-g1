//----------------------------------------------------------------------------------
// CPhysicActor class
// Author: Enric Vergara
//
// Description:
// Esta clase representa un actor físico.
//----------------------------------------------------------------------------------
#pragma once
#ifndef __PHYSIC_ACTOR_CLASS_H__
#define __PHYSIC_ACTOR_CLASS_H__

#include <vector>
#include "base.h"
#include "Math\Vector3.h"

//---Forward Declarations---
class NxActor;
class NxActorDesc;
class NxBodyDesc;
class NxCCDSkeleton;
class NxTriangleMesh;
class NxBoxShapeDesc;
class NxBoxShapeDesc;
class NxTriangleMeshShapeDesc;
class NxCapsuleShapeDesc;
class NxSphereShapeDesc;
class NxPlaneShapeDesc;
class CPhysicUserData;

enum NxForceMode;

class CPhysicUserData;
//--------------------------

class CPhysicActor
{
public:
	//--- Init and End protocols------------------------------------------
						CPhysicActor::CPhysicActor	( CPhysicUserData* _pUserData );
						//CPhysicActor::CPhysicActor	( NxActor * _pActor );
						CPhysicActor::~CPhysicActor	( void );

	//---- Main Functions ---------------------------------------
	void				CreateBody					( float _Density, float _AngularDamping = 0.5f, float _LinearDamping = 0.5f );
	
	//---Trigger Function---
	void				CreateBoxTrigger			( const Vect3f& _vGlobalPos, const Vect3f& _vSize, uint32 _uiGroup = 0 );
	void				CreateSphereTrigger			( const Vect3f& _vGlobalPos, const float _fRadius, uint32 _uiGroup );
	void				ActivateAllTriggers			( void );

	//---Get PhsX Info---
	void				CreateActor					( NxActor* actor );

	//---- Functions ---------------------------------------
	void				AddImpulseAtPos				( const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, bool _bLocal = true );
	void				AddVelocityAtPos			( const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, bool _bLocal = true );
	void				AddAcelerationAtPos			( const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, bool _bLocal = true );
	void				AddForceAtPos				( const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, bool _bLocal = true );
	void				AddTorque					( const Vect3f  _vTorque );
	
	//---AddShape Functions-----
	void				AddSphereShape				( float radius, const Vect3f& _vGlobalPos = v3fZERO, const Vect3f& _vLocalPos = v3fZERO, NxCCDSkeleton* _pSkeleton = 0, uint32 _uiGroup = 0 );
	void				AddBoxSphape				( const Vect3f& _vSize, const Vect3f& _vGlobalPos = v3fZERO, const Vect3f& _vLocalPos = v3fZERO, NxCCDSkeleton* _pSkeleton = 0, uint32 _uiGroup = 0 );
	void				AddCapsuleShape				( float _Radius, float _Height, const Vect3f& _vGlobalPos = v3fZERO, const Vect3f& _vLocalPos = v3fZERO, NxCCDSkeleton* _pSkeleton = 0, uint32 _uiGroup = 0 );
	void				AddMeshShape				( NxTriangleMesh* mesh, const Vect3f& _vGlobalPos = v3fZERO, const Vect3f& _vLocalPos = v3fZERO, NxCCDSkeleton* _pSkeleton = 0, uint32 _uiGroup = 0);
	void				AddPlaneShape				( const Vect3f& _vNormal, float _Distance, uint32 _uiGroup = 0);

	//---Activate---
	void				Activate					( bool _bActivate );
	void				SetKinematic				( bool _bValue );

  //---- Properties ( get & Set )---------------------------------------
	CPhysicUserData*	GetUserData					( void )							{ return m_pUserData; }

	void				SetLinearVelocity			( const Vect3f& _vVelocity );
	Vect3f				GetLinearVelocity			( void );
	Vect3f				GetPosition					( void );
	Vect3f				GetRotation					( void );
	void				SetRotation     			( const Vect3f& _vRot);
	void				SetRotation     			( const Mat33f& _mRot);
	void				SetAngularVelocity			( const Vect3f  _vVelocity );
	Vect3f				GetAngularVelocity			( void );
	void				SetGlobalPosition			( const Vect3f& _vPos = Vect3f ( 0.f, 0.f, 0.f ) );
	void				MoveGlobalPosition			( const Vect3f& _vPos);
	Vect3f				GetAngularMomentum			( void );
	Mat33f				GetInertiaTensor			( void );
	void				SetCollisionGroup			( uint32 _uiGroup );

	//---Get Info-------
	void				GetMat44					( Mat44f& _mMatrix ) const;
	void				SetMat44					( const Mat44f& _mMatrix );
	void				MoveGlobalPoseMat44			( const Mat44f& _mMatrix );
	
	//---Get PhsX Info---
	NxActor*			GetPhXActor					( void ) { return m_pPhXActor; }
	NxActorDesc*		GetActorDesc				( void ) { return m_pPhXActorDesc; }

//---Get Info-------
	void				SetActorSolverIterationCount( int _iCount );
	void				SetContactReportFlags		( unsigned int _uiFlags );
	void				SetContactReportThreshold	( float _fThreshold );

private:
	void				Destroy						( void );
	void				AddForceAtPos				( const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, NxForceMode _sForceMode, bool _bLocal );

	//---- Members ---------------------------------------
private:

	CPhysicUserData*							m_pUserData;
	NxActor*									m_pPhXActor;
	NxActorDesc*								m_pPhXActorDesc;
	NxBodyDesc*									m_pPhXBodyDesc;

	std::vector<NxBoxShapeDesc*>				m_vBoxDesc;
	std::vector<NxTriangleMeshShapeDesc*>		m_vMeshDesc;
	std::vector<NxCapsuleShapeDesc*>			m_vCapsuleDesc;
	std::vector<NxSphereShapeDesc*>				m_vSphereDesc;
	std::vector<NxPlaneShapeDesc*>				m_vPlaneDesc;
};

#endif __PHYSIC_ACTOR_CLASS_H__