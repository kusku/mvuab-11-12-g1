#pragma once

#ifndef _PHYSICS_DEFS_H
#define _PHYSICS_DEFS_H

#include <string>
#include "Utils\Named.h"
#include "Math\Vector3.h"
#include "Math\Color.h"

class CGameEntity;
class CPhysicActor;

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
  ECG_LAST_GROUP
};

//---- Declaracion de nuevos tipos------------
struct SPhysicsInitParams
{
	SPhysicsInitParams::SPhysicsInitParams()
		: m_fSkinWidth(0.001f), m_fGravity(-9.81f)
		, m_Restitution_DefMat(0.5f), m_StaticFriction_DefMat(0.5f)
		, m_DynamicFriction_DefMat(0.5f) 
	{}

	float		m_fSkinWidth;
	float		m_fGravity;
	float		m_Restitution_DefMat;				//Restitution of the default material
	float		m_StaticFriction_DefMat;		//Static Friction of the default material
	float		m_DynamicFriction_DefMat;		//Dynamic Friction of the default material
};

struct SCollisionInfo
{
	float		m_fDistance;
	Vect3f		m_Normal;
	Vect3f		m_CollisionPoint;
};


class CPhysicUserData : public CNamed
{
public:
	CPhysicUserData(const std::string& _szName) 
		: CNamed(_szName)
		, m_bPaintPhysicObject(false)
		, m_ColorPhysicObject(colWHITE)
		, m_pEntity(NULL)
		, m_pActor(NULL) 
	{
	}

	~CPhysicUserData() { /*Nothing*/ }

	void					SetPaint	( bool flag )						{ m_bPaintPhysicObject = flag; }
	void					SetColor	( const CColor& color )				{ m_ColorPhysicObject = color; }
	void					SetEntity	( CGameEntity* _pEntity )			{ m_pEntity = _pEntity; }
	void					SetActor	( CPhysicActor* _pActor )			{ m_pActor = _pActor; }

	bool					GetPaint	() const							{ return m_bPaintPhysicObject; }
	const CColor&			GetColor	() const							{ return m_ColorPhysicObject; }
	CGameEntity* 			GetEntity	() const							{ return m_pEntity; }
	CPhysicActor*			GetActor	() const							{ return m_pActor; }


private:
	bool				m_bPaintPhysicObject;
	CColor				m_ColorPhysicObject;
	CPhysicActor		*m_pActor;
	CGameEntity			*m_pEntity;
};

#endif