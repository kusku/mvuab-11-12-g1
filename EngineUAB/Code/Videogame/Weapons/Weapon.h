#pragma once

#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <string>
#include "Utils\Active.h"
#include "Weapons\WeaponDefs.h"
#include "Object3D.h"

class CAnimatedInstanceModel;

class CWeapon : public CActive
{
public:
	// ----Constructor & Destructor--------------
	CWeapon();
	~CWeapon();

	//---Main Methods----------------------------
	virtual void	Update	( float _ElapsedTime ) = 0;

	//---Get & Set Methods-----------------------
	inline void			SetName			( const std::string & _Name )		{ m_WeaponName = _Name; }
	inline void			SetAnimatedModel( CAnimatedInstanceModel *_Model )	{ m_pAnimatedModel = _Model; }
	inline void			SetLeftHandName	( const std::string & _Name )		{ m_LeftHandName = _Name; }
	inline void			SetRightHandName( const std::string & _Name )		{ m_RightHandName = _Name; }
	inline void			SetHandType		( EWeaponHand _Type )				{ m_WeaponHandType = _Type; }
	inline void			SetWeaponType	( EWeaponType _Type )				{ m_WeaponType = _Type; }
	inline void			SetWeaponLeft	( CObject3D *_pObject )				{ m_pWeaponLeft = _pObject; }
	inline void			SetWeaponRight	( CObject3D *_pObject )				{ m_pWeaponRight = _pObject; }

	inline const std::string&		GetName			() const			{ return m_WeaponName; }
	inline CAnimatedInstanceModel*	GetAnimatedModel() const			{ return m_pAnimatedModel; }			
	inline const std::string&		GetLeftHandName	() const			{ return m_LeftHandName; }
	inline const std::string&		GetRightHandName() const			{ return m_RightHandName; }
	inline EWeaponHand				GetHandType		() const			{ return m_WeaponHandType; }
	inline EWeaponType				GetWeaponType	() const			{ return m_WeaponType; }
	inline CObject3D*				GetWeaponLeft	() const			{ return m_pWeaponLeft; }
	inline CObject3D*				GetWeaponRight	() const			{ return m_pWeaponRight; }

protected:
	std::string		m_WeaponName;

	std::string		m_LeftHandName;
	std::string		m_RightHandName;

	EWeaponHand		m_WeaponHandType;
	EWeaponType		m_WeaponType;

	CObject3D		*m_pWeaponLeft;
	CObject3D		*m_pWeaponRight;
	CAnimatedInstanceModel *m_pAnimatedModel;
};

#endif //_WEAPON_H_