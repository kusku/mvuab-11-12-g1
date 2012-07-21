#pragma once

#ifndef _WEAPON_MANAGER_H_
#define _WEAPON_MANAGER_H_

#include "Utils\TemplatedVectorMapManager.h"
#include "WeaponDefs.h"
#include "Weapon.h"
#include <string>

class CWeaponManager : public CTemplatedVectorMapManager<CWeapon>
{
public:
	// ----Constructor & Destructor--------------
	CWeaponManager();
	~CWeaponManager();

	//---Main Methods----------------------------
	void	CleanUp					();

	bool	Load					( const std::string & _Filename );
	bool	Reload					();

	void	Update					( float _ElapsedTime );
	void	ChangeCurrentWeapon		( const std::string _Name );
	void	ChangeCurrentWeapon		( CWeapon *_pWeapon );

	//---Get & Set Methods-----------------------
	inline void		SetCurrentWeapon	( CWeapon * _weapon )		{ m_pCurrentWeapon = _weapon; }
	inline CWeapon*	GetCurrentWeapon	() const					{ return m_pCurrentWeapon; }

	//---Conversor Methods-----------------------
	EWeaponType		String2WeaponType	( const std::string & _Type );
	EWeaponHand		String2WeaponHands	( const std::string & _Left, const std::string & _Right );

private:
	bool	LoadFile				();

private:
	CWeapon		*m_pCurrentWeapon;

	std::string m_Filename;
};

#endif //_WEAPON_MANAGER_H_