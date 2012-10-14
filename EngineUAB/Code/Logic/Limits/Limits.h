
#pragma once

#ifndef _LIMITS_H 
#define _LIMITS_H 



#include "Math\Vector3.h"
#include "Object3D.h"
#include "Base.h"

#include <string>

class CPhysicActor;
class CParticleEmitterInstance;

class CLimit : public CObject3D
{
	CPhysicActor* m_MeshActor;
	CParticleEmitterInstance* m_PEI;

	std::string m_Group;

	bool m_Active;
	Vect3f m_Dimensions;

	bool CreatePhysX();
	bool CreateParticleEI();

public:
	CLimit();
	virtual ~CLimit();

	Vect3f GetDimensions() const { return m_Dimensions; }

	void SetDimension(const Vect3f& dim) { m_Dimensions = dim; }

	std::string GetGroup() const { return m_Group; }
	void SetGroup(const std::string& group) { m_Group = group; }


	bool Init();

	bool SetActive(bool active);
};

#endif