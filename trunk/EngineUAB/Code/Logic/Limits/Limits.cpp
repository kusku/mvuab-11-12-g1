
#include "Limits.h"
#include "Core.h"
#include "PhysicsManager.h"

////----PhysX Includes-------------
#undef min
#undef max
/*#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
#include "NxActor.h"*/
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysicTriggerReport.h"
#include "PhysicCookingMesh.h"
#include "PhysicUserData.h"
#include "Particles\ParticleEmitter.h"
#include "Particles\ParticleEmitterManager.h"
#include "Particles\ParticleEmitterInstance.h"
#include "Particles\Emitters\ParticleEmitterBox.h"
#include "Particles\ParticleSystem.h"
#include "Particles\ParticleSystemManager.h"
////--------------------------------


CLimit::CLimit()
	: m_Dimensions(v3fZERO)
	, m_Active(false)
	, m_Group("")
	, m_PEI(NULL)
	, m_MeshActor(NULL)
{

}

CLimit::~CLimit()
{

}

bool CLimit::Init()
{
	CreatePhysX();

	CreateParticleEI();

	return true;
}

bool CLimit::SetActive( bool active )
{
	m_Active = active;

	m_MeshActor->Activate(active);

	m_PEI->SetActive(active);

	return true;
}

bool CLimit::CreatePhysX()
{
	CPhysicUserData* l_pPhysicUserDataMesh = new CPhysicUserData( GetName()  );

	l_pPhysicUserDataMesh->SetGroup(ECG_LIMITS);

	Vect3f rotationVect = v3fZERO;

	rotationVect.x = mathUtils::Deg2Rad(GetPitch());
	rotationVect.y = mathUtils::Deg2Rad(GetYaw());
	rotationVect.z = mathUtils::Deg2Rad(GetRoll());
	
	Vect3f size = m_Dimensions;
	size /= 2;

	m_MeshActor = new CPhysicActor(l_pPhysicUserDataMesh);
	l_pPhysicUserDataMesh->SetPaint (true);

	m_MeshActor->AddBoxSphape(size, GetPosition(), Vect3f(0, 0, 0), rotationVect, NULL, ECG_LIMITS);

	CORE->GetPhysicsManager()->AddPhysicActor(m_MeshActor);

	m_MeshActor->Activate(false);

	return true;
}

bool CLimit::CreateParticleEI()
{
	CParticleSystem* psys = CORE->GetParticleSystemManager()->GetResource("FireLimit");
	
	float pps = (20 * (m_Dimensions.x / 10.0f)) / (float)CORE->GetConfig().particle_level;

	CParticleEmitterBox* pe = new CParticleEmitterBox(m_Name + "Limit", psys, pps, m_Position, false, -(m_Dimensions/2), m_Dimensions/2, true, GetYaw(), GetPitch(), GetRoll());

	CORE->GetParticleEmitterManager()->AddResource(pe->GetName(), pe);

	m_PEI = pe->GetParticleEmitterInstance(m_Name + "Limit Instance");

	m_PEI->SetActive(false);

	return true;
}
