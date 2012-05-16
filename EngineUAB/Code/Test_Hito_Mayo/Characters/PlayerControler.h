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
///class CPhysicController;
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

	//----Test Functions ------------------------------------------------
	void						MoveController			( float _ElapsedTime, const Vect3f &_Position = Vect3f (0.f, 0.f, 0.f) );
		
	//----Properties ( get & Set )---------------------------------------
	CPhysicController*			GetController			( void )								{ return m_pController; }

	inline float				GetYaw					( void ) const							{ return m_pController->GetYaw(); }
	inline float				GetRoll					( void ) const							{ return m_pController->GetRoll(); }
	inline float				GetPitch				( void ) const							{ return m_pController->GetPitch(); }
	inline Vect3f				GetPosition				( void ) const							{ return m_pController->GetPosition(); }

	inline void					SetYaw					( float _Yaw )							{ m_pController->SetYaw( _Yaw ); }
	inline void					SetRoll					( float _Roll )							{ m_pController->SetRoll( _Roll ); }
	inline void					SetPitch				( float _Pitch )						{ m_pController->SetPitch( _Pitch ); }
	inline void					SetPosition				( const Vect3f &_Position ) 			{ m_pController->SetPosition( _Position ); }
	

	//----Members -------------------------------------------------------
private:
	bool						m_bIsOk;
	
	CPhysicController*			m_pController;

	CPhysicUserData*			m_pPhysicUserDataJugador;
};

#endif __CLASS_CONTROLADOR_JUGADOR_H__