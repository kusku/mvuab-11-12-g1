//----------------------------------------------------------------------------------
// CPhysicActor class
// Author: Enric Vergara
//
// Description:
// Esta clase representa un actor físico.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_ACTOR_H_
#define INC_PHYSIC_ACTOR_H_

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
//--------------------------

class CPhysicActor
{
public:
	CPhysicActor::CPhysicActor(CPhysicUserData* userData);
	CPhysicActor::~CPhysicActor();

	CPhysicUserData*	GetUserData				()			{ return m_pUserData; }
	void				SetLinearVelocity		(const Vect3f& velocity);
	Vect3f				GetPosition				();
	Vect3f				GetRotation				();
	void				CreateBody				(float density, float angularDamping = 0.5f, float linearDamping = 0.5f);
	void				SetGlobalPosition		(const Vect3f& pos = Vect3f(0.f,0.f,0.f));
	void				MoveGlobalPosition		(const Vect3f& pos);
	void				SetRotation     		(const Vect3f& _vRot);
	void				SetRotation     		(const Mat33f& _vRot);
	Vect3f				GetLinearVelocity		();
	Vect3f				GetAngularVelocity		();
	Vect3f				GetAngularMomentum		();
	Mat33f				GetInertiaTensor		();

	void			    AddImpulseAtPos		(const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, bool _bLocal = true);
	void			    AddVelocityAtPos	(const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, bool _bLocal = true);
	void			    AddAcelerationAtPos	(const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, bool _bLocal = true);
	void			    AddForceAtPos		(const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, bool _bLocal = true);
	void			    AddTorque			(const Vect3f _vTorque);
	void			    SetAngularVelocity	(const Vect3f _vVelocity);

	//---AddShape Functions-----
	void				AddSphereShape		(float radius, const Vect3f& globalPos = v3fZERO, const Vect3f& localPos = v3fZERO, NxCCDSkeleton* skeleton = 0, uint32 group = 0);
	void				AddBoxSphape		(const Vect3f& size, const Vect3f& globalPos = v3fZERO, const Vect3f& localPos = v3fZERO, NxCCDSkeleton* skeleton = 0, uint32 group = 0);
	void				AddCapsuleShape		(float radius, float height, const Vect3f& globalPos = v3fZERO, const Vect3f& localPos = v3fZERO, NxCCDSkeleton* skeleton = 0, uint32 group = 0);
	void				AddMeshShape		(NxTriangleMesh* mesh, const Vect3f& globalPos = v3fZERO, const Vect3f& localPos = v3fZERO, NxCCDSkeleton* skeleton = 0, uint32 group = 0);
	void				AddPlaneShape		(const Vect3f& normal, float distance, uint32 group = 0);

	//---Trigger Function---
	void				CreateBoxTrigger	(const Vect3f& globalPos, const Vect3f& size, uint32 group = 0);
	void				ActivateAllTriggers	();

  //---Activate---
  void				    Activate			( bool _bActivate );
  void				    SetKinematic		( bool _bValue );

	//---Get Info-------
	void				GetMat44			(Mat44f& matrix) const;
	void				SetMat44			(const Mat44f& matrix);
	void				MoveGlobalPoseMat44 (const Mat44f& matrix);

	//---Get PhsX Info---
	void				CreateActor			(NxActor* actor);
	NxActor*			GetPhXActor			() const			{ return m_pPhXActor; }
	NxActorDesc*		GetActorDesc		() const			{ return m_pPhXActorDesc; }

	void				SetActorSolverIterationCount		(int _iCount);
	void				SetContactReportFlags				(uint32 _uiFlags);
	void				SetContactReportThreshold			(float _fThreshold);

private:
	void				DeInit			();
	void				AddForceAtPos		(const Vect3f& _vDirection, const Vect3f& _vPos, float _fPower, NxForceMode _sForceMode, bool _bLocal);

private:
	CPhysicUserData								*m_pUserData;
	NxActor										*m_pPhXActor;
	NxActorDesc									*m_pPhXActorDesc;
	NxBodyDesc									*m_pPhXBodyDesc;

	std::vector<NxBoxShapeDesc*>				m_vBoxDesc;
	std::vector<NxTriangleMeshShapeDesc*>		m_vMeshDesc;
	std::vector<NxCapsuleShapeDesc*>			m_vCapsuleDesc;
	std::vector<NxSphereShapeDesc*>				m_vSphereDesc;
	std::vector<NxPlaneShapeDesc*>				m_vPlaneDesc;
};

#endif //INC_PHYSIC_ACTOR_H_