#pragma once

#ifndef __CLASS_CONTROLADOR_JUGADOR_H__
#define __CLASS_CONTROLADOR_JUGADOR_H__

#include <string>
#include "Math\Vector3.h"
#include "PhysicController.h"

//---Forward Declarations---
class CPhysicActor;
class CPhysicUserData;
class CRenderManager;
//class CPhysicController;
//--------------------------

class CPlayerControler
{
public:
	//--- Init and End protocols------------------------------------------
								CPlayerControler		( void );
	virtual						~CPlayerControler		( void );

	//----Main Functions -------------------------------------------------
	void						Done					( void );
	virtual bool				IsOK					( void ) const	{ return m_bIsOk; };
	bool						Init					( const std::string &_Name, const Vect3f& _vGlobalPos = 0.f, const Vect3f& _vLocalPos = 0.f );
	void						Release					( void );
	
	
	//----Functions -----------------------------------------------------
	void						Update					( float ElapsedTime );
	void						Render					( CRenderManager * _RM );
	/*void						RenderScene				( void );*/
	void						UpdateInputActions		( float ElapsedTime );

	//----Test Functions ------------------------------------------------
	void						UpdateMovementControler ( float _ElapsedTime, const Vect3f &_Position = Vect3f (0.f, 0.f, 0.f) );
		
	//----Properties ( get & Set )---------------------------------------
	//CPhysicActor*				GetActor				( void )		{ return m_pBoxActor; }
	Vect3f						GetGlobalPosition		( void )		{ return m_PosActor; }
	Vect3f						GetAnchor				( void )		{ return m_PosAnchor; }
	CPhysicController*			GetController			( void )		{ return m_pController; }

	//inline float				GetYaw				( void ) const		{ return m_pController->GetYaw(); }
	//inline float				GetRoll				( void ) const		{ return m_pController->GetRoll(); }
	//inline float				GetPitch			( void ) const		{ return m_pController->GetPitch(); }
	
	//Vect3f&		GetPosition			( void ) const		{ return m_pController->GetPosition(); }

	//----Members -------------------------------------------------------
private:
	bool						m_bIsOk;
	
	CPhysicController*			m_pController;

	CPhysicUserData*			m_pPhysicUserDataJugador;
	Vect3f						m_PosAnchor;
	Vect3f						m_PosActor;
};

#endif __CLASS_CONTROLADOR_JUGADOR_H__