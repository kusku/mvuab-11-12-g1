#pragma once

#ifndef _WEAPON_DEFS_H_
#define _WEAPON_DEFS_H_

typedef enum EWeaponType
{
	NONE_TYPE = 0,
	MELEE_TYPE,
	DISTANCE_TYPE
};

typedef enum EWeaponHand
{
	NONE_HAND = 0,
	LEFT_HAND,
	RIGHT_HAND,
	BOTH_HAND
};

#endif //_WEAPON_DEFS_H_