#include "Weapon.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

CWeapon::CWeapon()
	: m_pWeaponLeft(NULL)
	, m_pWeaponRight(NULL)
	, m_pAnimatedModel(NULL)
	, m_WeaponName("")
	, m_LeftHandName("")
	, m_RightHandName("")
	, m_WeaponType(NONE_TYPE)
	, m_WeaponHandType(NONE_HAND)
{

}

CWeapon::~CWeapon()
{

}