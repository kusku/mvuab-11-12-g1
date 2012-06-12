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
#include "Logger\Logger.h"

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
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CCharacter_Wrapper] first constructor");
	}

	CCharacter_Wrapper(const std::string &_Name)
		: CCharacter(_Name)
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CCharacter_Wrapper] second constructor");
	}

	CCharacter_Wrapper(int _id, const std::string &_Name)
		: CCharacter(_id, _Name)
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CCharacter_Wrapper] destructor");
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


#endif /* __CHARACTER_WRAPPER_H__ */