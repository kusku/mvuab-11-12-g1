#pragma once

#ifndef __PHYSIC_USER_DATA_CLASS_H__
#define __PHYSIC_USER_DATA_CLASS_H__

#include <string>

#include "Utils\Named.h"
#include "Math\Color.h"
#include "Base.h"

// ----- FOWARD Declarations -------//
class CObject3D;
class CPhysicActor;
class CColor;
// ---------------------------------//

enum EUserDataFlag
{
	UD_IS_HW_ACTOR					= (1<<0),
	UD_IS_TRIGGER			        = (1<<1),
    UD_IS_INSIDE_TRIGGER            = (1<<2),
    UD_PASSES_INTERSECTION_TEST     = (1<<3),
    UD_HIT_BY_RAYCAST               = (1<<4),
    UD_NO_RENDER                    = (1<<5),
    UD_IS_DRAIN                     = (1<<6),
	UD_IS_ASLEEP				    = (1<<7),
	UD_RENDER_USING_LIGHT1			= (1<<8),
}; 

class CPhysicUserData : public CNamed
{
public:
					CPhysicUserData				( const std::string& _szName, EUserDataFlag _eflags = UD_IS_HW_ACTOR )	// Afegeixo el flag per distingir-lo
						: CNamed				( _szName )
						, m_bPaintPhysicObject	( false )
						, m_ColorPhysicObject	( colWHITE )
						, m_pEntity				( 0 )
						, m_pActor				( 0 )
						, m_eFlags				( _eflags )
					{}
					
					~CPhysicUserData			( void )							{
																						 m_pActor = NULL;
																						 m_pEntity = NULL;
																					}

	void			SetPaint					( bool _bFlag )						{ m_bPaintPhysicObject = _bFlag; };
	void			SetColor					( const CColor& _Color )			{ m_ColorPhysicObject = _Color; };
	void			SetEntity					( CObject3D* _pEntity )				{ m_pEntity = _pEntity; };
	void			SetActor					( CPhysicActor* _pActor )			{ m_pActor = _pActor; };

	bool			GetPaint					( void ) const						{ return m_bPaintPhysicObject; };
	const CColor&	GetColor					( void ) const						{ return m_ColorPhysicObject; };
	CObject3D* 		GetEntity					( void ) const						{ return m_pEntity; };
	CPhysicActor*   GetActor					( void ) 							{ return m_pActor; };
	
	// Per permetre saber info del objecte
	void			setFlags					( EUserDataFlag	_eFlags ) 			{ m_eFlags = _eFlags; }
	EUserDataFlag	GetFlags					( void ) const						{ return m_eFlags; }

private:
	bool			m_bPaintPhysicObject;
	CColor			m_ColorPhysicObject;
	CPhysicActor*	m_pActor;
	CObject3D*		m_pEntity;
	EUserDataFlag	m_eFlags;
};

#endif __PHYSIC_USER_DATA_CLASS_H__