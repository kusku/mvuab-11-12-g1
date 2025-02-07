//----------------------------------------------------------------------------------
// CCharacter_Wrapper class
// Author: Marc Cuscullola
//
// Description:
// Wrapper para Lua de la clase Character
//----------------------------------------------------------------------------------
#pragma once

#ifndef _CHARACTER_WRAPPER_H_
#define _CHARACTER_WRAPPER_H_

#include "Characters\Character.h"
#include "Scripting\ScriptManager.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// ----------------------------------------
//		WRAPPER PARA LUA
// ----------------------------------------
struct CCharacter_Wrapper: CCharacter, luabind::wrap_base
{
	CCharacter_Wrapper()
		: CCharacter()
	{
	}

	CCharacter_Wrapper(const std::string &_Name)
		: CCharacter(_Name)
	{
	}

	CCharacter_Wrapper(int _id, const std::string &_Name)
		: CCharacter(_id, _Name)
	{
	}

	virtual bool Init()
	{
		 return call<bool>("init");
	}

	virtual void UpdatePlayer(float _ElapsedTime)
	{
		call<void>("update", _ElapsedTime);
	}

	/*virtual void UpdateEnemy(float _ElapsedTime)
	{
		call<void>("update", _ElapsedTime);
	}*/

	/*virtual void UpdateIA(float _ElapsedTime)
	{
		call<void>("update_ia", _ElapsedTime);
	}*/

	static void Default_Update(CCharacter *ptr, float _ElapsedTime)
	{
		ptr->CCharacter::UpdatePlayer(_ElapsedTime);
	}

	virtual void BeDead()
	{
		 call<void>("bedead");
	}
};


#endif /* __CHARACTER_WRAPPER_H__ */