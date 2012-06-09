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

#include "Character.h"
#include "Scripting\ScriptManager.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

struct CCharacter_Wrapper : CCharacter, luabind::wrap_base
{
	CCharacter_Wrapper()
		: CCharacter()
	{
	}

	CCharacter_Wrapper(const std::string &name)
		: CCharacter(name)
	{
	}

	CCharacter_Wrapper(int id, const std::string &name)
		: CCharacter(id, name)
	{
	}

	virtual bool Init()
	{
		 return call<bool>("init");
	}

	virtual void Update(float _ElapsedTime)
	{
		call<void>("update", _ElapsedTime);
	}

	static void Default_Update(CCharacter *ptr, float _ElapsedTime)
	{
		ptr->CCharacter::Update(_ElapsedTime);
	}
};

#endif