#pragma once
#ifndef __CLASS_PROPERTIES_H__
#define __CLASS_PROPERTIES_H__

#include <string>

#include "Object3D.h"
#include "Utils\Named.h"
#include "Math\Vector3.h"

class CProperties : public CObject3D
{
public:
	//--- Init and End protocols ----------------------------------------
						CProperties				( const std::string &_Name, int _Life = 0, int _Strong = 0, float _Speed = 0 , const Vect3f &_Direction = Vect3f(0.f,0.f,0.f) );
						CProperties				( void );
						~CProperties			( void );

	//---- Main Methods --------------------------------------------------
	//---- Methods -------------------------------------------------------
	//---- Properties ( get & Set )---------------------------------------

	inline void					SetCore					( const std::string &_CoreName )					{ m_Core = _CoreName; }
	inline const std::string	GetCore					( void ) const										{ return m_Core; }

	inline void					SetAnimationInstance	( const std::string &_AnimationInstanceName )		{ m_AnimationInstanceName = _AnimationInstanceName; }
	inline std::string			GetAnimationInstance	( void ) const										{ return m_AnimationInstanceName; }

	inline void					SetLife					( int _Life )										{ m_Life = _Life; }
	inline const int			GetLife					( void ) const										{ return m_Life; }

	inline void					SetStrong				( int _Strong )										{ m_Strong = _Strong; }
	inline const int			GetStrong				( void ) const										{ return m_Strong; }

	inline void					SetSpeed				( float _Speed )									{ m_Speed = _Speed; }
	inline const float			GetSpeed				( void ) const										{ return m_Speed; }

	inline void					SetRotationSpeed		( float _RotationSpeed )							{ m_RotationSpeed = _RotationSpeed; }
	inline float				GetRotationSpeed		( void ) const										{ return m_RotationSpeed; }

	inline void					SetDirection			( const Vect3f &_Direction )						{ m_Direction = _Direction; }
	inline const Vect3f			GetDirection			( void ) const										{ return m_Direction; }

	inline void					SetRespawnPosition		( const Vect3f &_RespawnPosition )					{ m_RespawnPosition = _RespawnPosition; }
	inline const Vect3f			GetRespawnPosition		( void ) const										{ return m_RespawnPosition; }

	inline void					SetDetectionDistance	( float _DetectionDistance )						{ m_DetectionDistance = _DetectionDistance; }
	inline float				GetDetectionDistance	( void ) const										{ return m_DetectionDistance; }
	
	inline void					SetDistanceChase		( float _DistanceChase )							{ m_DistanceChase = _DistanceChase; }
	inline float				GetDistanceChase		( void ) const										{ return m_DistanceChase; }

	inline void					SetAttackDistance		( float _AttackDistance )							{ m_AttackDistance = _AttackDistance; }
	inline float				GetAttackDistance		( void ) const										{ return m_AttackDistance; }

	
	//---- Members -------------------------------------------------------
private:
	std::string			m_Core;
	std::string			m_AnimationInstanceName;
	int					m_Life;
	int					m_Strong;
	float				m_Speed;
	float				m_RotationSpeed;								
	Vect3f				m_Direction;
	Vect3f				m_RespawnPosition;
	float				m_DetectionDistance;			// distancia de detección de otros caràcteres
	float				m_DistanceChase;			// distancia de persecución 
	float				m_AttackDistance;			// distancia de persecución 
};

#endif __CLASS_PROPERTIES_H__