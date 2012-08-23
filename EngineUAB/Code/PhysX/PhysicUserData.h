#pragma once

#ifndef __PHYSIC_USER_DATA_CLASS_H__
#define __PHYSIC_USER_DATA_CLASS_H__

#include <string>

#include "Utils\Named.h"
#include "Math\Color.h"
#include "Base.h"
#include "PhysicController.h"
#include "PhysicsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"

// ----- FOWARD Declarations -------//
class CObject3D;
class CPhysicActor;
class CColor;
//class CSteeringEntity;
// ---------------------------------//

enum EUserDataFlag
{
	UD_IS_HW_ACTOR					= (1<<0),
	UD_IS_HW_CONTROLLER				= (1<<1),
	UD_IS_TRIGGER			        = (1<<2),
    UD_IS_INSIDE_TRIGGER            = (1<<3),
    UD_PASSES_INTERSECTION_TEST     = (1<<4),
    UD_HIT_BY_RAYCAST               = (1<<5),
    UD_NO_RENDER                    = (1<<6),
    UD_IS_DRAIN                     = (1<<7),
	UD_IS_ASLEEP				    = (1<<8),
	UD_RENDER_USING_LIGHT1			= (1<<9),
}; 

class CPhysicUserData : public CNamed
{
public:
	CPhysicUserData				( const std::string& _szName, EUserDataFlag _eflags = UD_IS_HW_ACTOR, ECollisionGroup _uiGrup = ::ECG_ESCENE )	// Afegeixo el flag per distingir-lo
		: CNamed				( _szName )
		, m_bPaintPhysicObject	( false )
		, m_ColorPhysicObject	( colWHITE )
		, m_pEntity				( 0 )
		, m_pActor				( 0 )
		, m_pController			( 0 )
		, m_eFlags				( _eflags )
		, m_uiCollisionGroup	( _uiGrup )
		, m_pSteeringEntity		( NULL )
	{}
		
	~CPhysicUserData()						
	{
		m_pActor = NULL;
		m_pEntity = NULL;
		m_pSteeringEntity = NULL;
	}

	void						SetPaint			( bool _bFlag )							{ m_bPaintPhysicObject = _bFlag; };
	void						SetColor			( const CColor& _Color )				{ m_ColorPhysicObject = _Color; };
	void						SetEntity			( CObject3D* _pEntity )					{ m_pEntity = _pEntity; };
	void						SetActor			( CPhysicActor* _pActor )				{ m_pActor = _pActor; };
	void						SetController		( CPhysicController* _pController )		{ m_pController = _pController; };
	void						SetRadius			( float _Radius )						{ m_fRadius = _Radius; };
	void						SetGroup			( ECollisionGroup _uiGroup )			{ m_uiCollisionGroup = _uiGroup; };
	void						SetSteeringEntity	( CSteeringEntity* _pSteeringEntity )	{ m_pSteeringEntity = _pSteeringEntity; };
	
	inline bool					GetPaint			() const								{ return m_bPaintPhysicObject; };
	inline const CColor&		GetColor			() const								{ return m_ColorPhysicObject; };
	inline CObject3D* 			GetEntity			() const								{ return m_pEntity; };
	inline CPhysicActor*		GetActor			() const								{ return m_pActor; };
	inline CPhysicController*	GetController		() const								{ return m_pController; };
	inline float				GetRadius			() const								{ return m_fRadius; };
	inline ECollisionGroup		GetGroup			() const								{ return m_uiCollisionGroup; };
	
	// Per permetre saber info del objecte
	void						setFlags			( EUserDataFlag	_eFlags ) 				{ m_eFlags = _eFlags; }
	EUserDataFlag				GetFlags			() const								{ return m_eFlags; }

	inline CSteeringEntity*		GetSteeringEntity	() const								{ return m_pSteeringEntity; };
	

private:
	float				m_fRadius;
	bool				m_bPaintPhysicObject;
	CColor				m_ColorPhysicObject;
	CPhysicActor*		m_pActor;
	CPhysicController*	m_pController;
	CObject3D*			m_pEntity;
	EUserDataFlag		m_eFlags;
	ECollisionGroup		m_uiCollisionGroup;
	CSteeringEntity*	m_pSteeringEntity;
};

#endif __PHYSIC_USER_DATA_CLASS_H__