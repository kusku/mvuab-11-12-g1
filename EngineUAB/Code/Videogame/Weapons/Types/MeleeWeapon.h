#pragma once

#ifndef _MELEE_WEAPON_H_
#define _MELEE_WEAPON_H_

#include "Weapons\Weapon.h"
#include "Weapons\WeaponDefs.h"

class CMeleeWeapon : public CWeapon
{
public: 
	CMeleeWeapon();
	~CMeleeWeapon();

	void Update (float _ElapsedTime);

private:
	
};

#endif //_MELEES_WEAPON_H_
