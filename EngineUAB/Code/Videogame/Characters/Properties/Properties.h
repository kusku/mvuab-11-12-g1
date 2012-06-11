#pragma once
#ifndef __CLASS_PROPERTIES_H__
#define __CLASS_PROPERTIES_H__

#include <string>

#include "Object3D.h"
#include "Utils\Named.h"
#include "Math\Vector3.h"

class CProperties : public CObject3D, public CNamed
{
public:
	//--- Init and End protocols ----------------------------------------
						CProperties				( const std::string &_Name, int _Life = 0, int _Strong = 0, int _Speed = 0 , float _Height = 0.f, const Vect3f &_Direction = Vect3f(0.f,0.f,0.f) );
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

	inline void					SetSpeed				( int _Speed )										{ m_Speed = _Speed; }
	inline const int			GetSpeed				( void ) const										{ return m_Speed; }

	inline void					SetHeight				( float _Height )									{ m_Height = _Height; }
	inline const float			GetHeight				() const											{ return m_Height; }

	inline void					SetDirection			( const Vect3f &_Direction )						{ m_Direction = _Direction; }
	inline const Vect3f			GetDirection			( void ) const										{ return m_Direction; }

	inline void					SetRespawnPosition		( const Vect3f &_RespawnPosition )					{ m_RespawnPosition = _RespawnPosition; }
	inline const Vect3f			GetRespawnPosition		( void ) const										{ return m_RespawnPosition; }


	//---- Members -------------------------------------------------------
private:
	std::string			m_Core;
	std::string			m_AnimationInstanceName;
	int					m_Life;
	int					m_Strong;
	int					m_Speed;
	Vect3f				m_Direction;
	Vect3f				m_RespawnPosition;
	float				m_Height;
};

#endif __CLASS_PROPERTIES_H__