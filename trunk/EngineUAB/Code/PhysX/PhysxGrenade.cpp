#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysxGrenade.h"
#include "PhysicsManager.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "PhysicFixedJoint.h"
#include "PhysicSphericalJoint.h"
#include "PhysicRevoluteJoint.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysxBone.h"
#include <cal3d/cal3d.h>
#include <XML/XMLTreeNode.h>
#include "RenderManager.h"
#include "Core.h"
#include "base.h"
//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
//---------------------//


//Release dels elements
void CPhysxGrenade::Release()
{
  CHECKED_DELETE(m_pPhysxActor); 
  CHECKED_DELETE(m_pPhysxUserData);
}


//Funcio per explotar
void CPhysxGrenade::Explosion()
{
  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();
  vector<CPhysicUserData*> l_vUserDatas;
  l_vUserDatas.clear();

  Mat44f l_vMat44;
  m_pPhysxActor->GetMat44(l_vMat44);
  Vect3f l_vTranslationVector = l_vMat44.GetTranslationVector();
  //Particules

  //if (m_pParticleEmitter != 0)
  //{
  //  float l_sizeY=m_pParticleEmitter->GetSizeY()/2;
  //  m_pParticleEmitter->SetPosition(l_vTranslationVector);
  //  m_pParticleEmitter->SetActive(true);
  //}

  l_pPM->OverlapSphereActorGrenade(m_fEffectRadius,l_vTranslationVector,l_vUserDatas,m_fFirePower);

 /* for(size_t i=0;i<l_vUserDatas.size();++i)
  {
    l_vUserDatas[i]->SetColor(colRED);
  }
*/
  m_bExploted = true;
  m_pPhysxUserData->SetPaint(false);
  //m_pPhysxUserData->SetColor(colRED);
}

//Funcio que fa update al temps de vida
void CPhysxGrenade::Update(float _fElapsedTime)
{
  if (m_bExploted == false)
  {
    m_fLifeTime += _fElapsedTime;

    if (m_fLifeTime >= m_fDuration)
    {
      Explosion();
    }
  }
}

void CPhysxGrenade::Init(float _fSphereRadius, float _fDensity, int _iColGroup, Vect3f _vPosition, Vect3f _vDirection,float _fVelocity, string _szEmitter)
{
  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();

  //m_pParticleEmitter = CORE->GetParticleManager()->GetParticleEmitter(_szEmitter);

  m_pPhysxUserData = new CPhysicUserData(GetName());
  m_pPhysxUserData->SetPaint(true);
  m_pPhysxUserData->SetColor(colYELLOW);

  m_pPhysxActor = new CPhysicActor(m_pPhysxUserData);
  m_pPhysxActor->AddSphereShape(_fSphereRadius,_vPosition,v3fZERO,NULL,_iColGroup);
  m_pPhysxActor->CreateBody(_fDensity);
  //m_pPhysxActor->SetGlobalPosition(_vPosition);


  l_pPM->AddPhysicActor(m_pPhysxActor);
  m_pPhysxActor->SetLinearVelocity(_vDirection*_fVelocity);
  
}