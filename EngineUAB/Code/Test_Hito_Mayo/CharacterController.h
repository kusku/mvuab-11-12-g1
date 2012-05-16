#pragma once

#ifndef __CLASS_CHARACTER_CONTROLER_H__
#define __CLASS_CHARACTER_CONTROLER_H__

#include <string>
#include "Math\Vector3.h"
#include "PhysicController.h"

//---Forward Declarations---
class CPhysicActor;
class CPhysicUserData;
class CRenderManager;
//class CPhysicController;
//--------------------------

class CCharacterController
{
public:
	//--- Init and End protocols------------------------------------------
								CCharacterController	( void );
	virtual						~CCharacterController	( void );

	//----Main Functions -------------------------------------------------
	virtual void				Done					( void );
	virtual bool				IsOK					( void ) const	{ return m_bIsOk; };
	virtual bool				Init					( const std::string &_Name, const Vect3f& _vGlobalPos = 0.f, const Vect3f& _vLocalPos = 0.f ) = 0;
	virtual void				Release					( void );
	
	
	//----Functions -----------------------------------------------------
	virtual void				Update					( float ElapsedTime ) = 0;
	virtual void				Render					( CRenderManager * _RM ) = 0;
	//virtual void				UpdateInputActions		( float ElapsedTime ) = 0;

	//----Test Functions ------------------------------------------------
	void						UpdateMovementControler ( float _ElapsedTime, const Vect3f &_Position = Vect3f (0.f, 0.f, 0.f) );
		
	//----Properties ( get & Set )---------------------------------------
	//CPhysicActor*				GetActor				( void )		{ return m_pBoxActor; }
	virtual Vect3f				GetGlobalPosition		( void )		{ return m_Position; }
	virtual Vect3f				GetAnchor				( void )		{ return m_PositionAnchor; }
	virtual CPhysicController*	GetController			( void )		{ return m_pController; }

	//inline float				GetYaw					( void ) const	{ return m_pController->GetYaw(); }
	//inline float				GetRoll					( void ) const	{ return m_pController->GetRoll(); }
	//inline float				GetPitch				( void ) const	{ return m_pController->GetPitch(); }
	
	//const Vect3f&				GetPosition				( void ) const	{ return m_pController->GetPosition(); }

	//----Members -------------------------------------------------------
private:
	bool						m_bIsOk;
	
	CPhysicController*			m_pController;

	CPhysicUserData*			m_pPhysicUserDataJugador;
	Vect3f						m_PositionAnchor;
	Vect3f						m_Position;
};

#endif __CLASS_CHARACTER_CONTROLER_H__
