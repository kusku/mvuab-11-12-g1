#define __DONT_INCLUDE_MEM_LEAKS__
#include "base.h"
#include "PhysicController.h"

//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
#include "NxController.h"
#include "PhysicsManager.h"
#include "NxCapsuleController.h"
//---------------------//

#include "Utils/MemLeaks.h"


class CPhysicsControllerHitReport : public NxUserControllerHitReport
{
	public:

	virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit)
	{
		if(hit.shape)
		{
			NxCollisionGroup group = hit.shape->getGroup();
			if(group==ECG_OBJECTES_DINAMICS)
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


CPhysicController::CPhysicController(float radius, float height, float slope, float skinwidth, float stepOffset,
																		 uint32 collisionGroups, CPhysicUserData* userData, const Vect3f& pos, float gravity)
																		 : m_pPhXController(NULL)
																		 , m_pPhXControllerDesc(NULL)
																		 , m_pUserData(userData)
																		 , m_fRadius_Capsule(radius)
																		 , m_fHeight_Capsule(height)
																		 , m_fSlopeLimit_Capsule(slope)
																		 , m_fSkinWidth_Capsule(skinwidth)
																		 , m_fStepOffset_Capsule(stepOffset)
																		 , m_fGravity(gravity)
																		 , m_uCollisionGroups(collisionGroups),
                                     m_bUseGravity(true)
                                    
{
	assert(userData);

	//---- Crear un nuevo NxController----
	m_pPhXControllerDesc = new NxCapsuleControllerDesc();
  CPhysicsControllerHitReport* l_Report  = new CPhysicsControllerHitReport();
  m_Report = l_Report;

	m_pPhXControllerDesc->position.x	    = pos.x;
	m_pPhXControllerDesc->position.y	    = pos.y;
	m_pPhXControllerDesc->position.z	    = pos.z;
	m_pPhXControllerDesc->radius			    = m_fRadius_Capsule;
	m_pPhXControllerDesc->height			    = m_fHeight_Capsule;
	m_pPhXControllerDesc->slopeLimit	    = cosf(NxMath::degToRad(m_fSlopeLimit_Capsule));
	m_pPhXControllerDesc->skinWidth		    = m_fSkinWidth_Capsule;
	m_pPhXControllerDesc->stepOffset	    = m_fStepOffset_Capsule;
	m_pPhXControllerDesc->upDirection	    = NX_Y;
  m_pPhXControllerDesc->callback        = l_Report;
  m_pPhXControllerDesc->interactionFlag = NXIF_INTERACTION_USE_FILTER;
}


CPhysicController::~CPhysicController()
{
  //delete m_pPhXControllerDesc->callback;
	CHECKED_DELETE(m_pPhXControllerDesc);
  CHECKED_DELETE(m_Report);
}


void CPhysicController::CreateController (NxController* controller, NxScene* scene)
{
	m_pPhXScene = scene;
	m_pPhXController = controller;
	assert(m_pPhXScene);
	assert(m_pPhXController);
	CHECKED_DELETE(m_pPhXControllerDesc);

}

void CPhysicController::SetPosition	(const Vect3f& pos)
{
  
	if (m_pPhXController != NULL)
	{
		NxExtendedVec3 position;
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
		m_pPhXController->setPosition(position);
	}
	else
	{
		m_pPhXControllerDesc->position.x		= pos.x;
		m_pPhXControllerDesc->position.y		= pos.y;
		m_pPhXControllerDesc->position.z		= pos.z;
	}
	//CObject3D::m_vPosition = pos;
  //CObject3D::InitMat44();
  CObject3D::SetPosition(pos);
}

Vect3f CPhysicController::GetPosition ()
{
	Vect3f vec;
	if (m_pPhXController != NULL)
	{
		NxExtendedVec3 tmp = m_pPhXController->getPosition();
		vec.x = (float)tmp.x;
		vec.y = (float)tmp.y;
		vec.z = (float)tmp.z;
	}
	else
	{
		vec.x = (float)m_pPhXControllerDesc->position.x;
		vec.y = (float)m_pPhXControllerDesc->position.y;
		vec.z = (float)m_pPhXControllerDesc->position.z;
	}
	return vec;
}

void CPhysicController::Jump(float ammount)
{
	m_Jump.StartJump(ammount);
}

void CPhysicController::Move(const Vect3f& direction, float elapsedTime)
{
	assert (m_pPhXController!=NULL);

  float l_fDirectionY = direction.y;

  if(m_bUseGravity)
  {
    l_fDirectionY += (m_fGravity*elapsedTime);
  }

	NxVec3 d(direction.x, l_fDirectionY, direction.z);
	NxF32 sharpness = 1.0f;
	NxU32 collisionFlags = 0;
  //NxU32 Collision = 0;
	float heightDelta = m_Jump.GetHeight(elapsedTime);
	if( heightDelta != 0.f )
	{
		d.y+=heightDelta;
		d.x *= 0.3f;
		d.z *= 0.3f;
	}

	//m_pPhXController->move(d, m_uCollisionGroups, 0.000001f, collisionFlags, sharpness);
  m_pPhXController->move(d, m_uCollisionGroups, 0.000001f, collisionFlags, sharpness);
	if(	(collisionFlags & NXCC_COLLISION_DOWN) || (collisionFlags & NXCC_COLLISION_UP) )
	{
		m_Jump.StopJump();
	}
	NxExtendedVec3 tmp = m_pPhXController->getPosition();
	CObject3D::m_vPosition.x = (float)tmp.x;
	CObject3D::m_vPosition.y = (float)tmp.y;
	CObject3D::m_vPosition.z = (float)tmp.z;
  CObject3D::InitMat44();
}

void CPhysicController::SetCollision (bool flag)
{
	assert(m_pPhXController);
	m_pPhXController->setCollision(flag);
}

bool CPhysicController::UpdateCharacterExtents (bool bent, float ammount)
{
	NxF32 height = m_fHeight_Capsule;
	NxF32 radius = m_fRadius_Capsule;
	NxExtendedVec3 pos = m_pPhXController->getPosition();
	if ( bent )
	{
		//Ponerse de pie
		height += ammount;
		pos.y += ammount*0.5f;
	} 
	else 
	{ 
		//Agacharse
		height -= ammount;
		pos.y -= ammount*0.5f;
	}

	NxCapsule worldCapsule;
	worldCapsule.p0.x = worldCapsule.p1.x = (NxReal)pos.x;
	worldCapsule.p0.y = worldCapsule.p1.y = (NxReal)pos.y;
	worldCapsule.p0.z = worldCapsule.p1.z = (NxReal)pos.z;
	worldCapsule.p0.y -= height*0.5f;
	worldCapsule.p1.y += height*0.5f;
	worldCapsule.radius = radius;
	m_pPhXController->setCollision(false);	// Avoid checking overlap with ourself
	bool Status = m_pPhXScene->checkOverlapCapsule(worldCapsule);
	m_pPhXController->setCollision(true);
	if(Status)
	{
		return false;
	}

	NxExtendedVec3 position(pos.x, pos.y, pos.z);
	m_pPhXController->setPosition(position);
	CObject3D::m_vPosition.x = (float)pos.x;
	CObject3D::m_vPosition.y = (float)pos.y;
	CObject3D::m_vPosition.z = (float)pos.z;
	NxCapsuleController* c = static_cast<NxCapsuleController*> (m_pPhXController);
	c->setHeight(height);
	m_fHeight_Capsule = height;
  CObject3D::InitMat44();
	return true;
}

void CPhysicController::SetGroup(int _iGroup)
{
  //m_pPhXController->getActor()->setGroup(_iGroup);
  NxShape *const* shapes = m_pPhXController->getActor()->getShapes();
  int l_iNumShapes = m_pPhXController->getActor()->getNbShapes();
  for(int i = 0; i < l_iNumShapes; ++i)
  {
    shapes[i]->setGroup(_iGroup);
  }
}


void CPhysicController::SetHeight(float _fHeight)
{
  NxCapsuleController* l_CC = dynamic_cast<NxCapsuleController*>(m_pPhXController);
  if(l_CC)
  {
    l_CC->setHeight(_fHeight);
  }
}

void CPhysicController::SetActive(bool _bActive)
{
  m_pPhXController->setCollision(_bActive);
}
