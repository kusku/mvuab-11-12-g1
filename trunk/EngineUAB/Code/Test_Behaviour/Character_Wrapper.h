
#ifndef __CHARACTER_WRAPPER_H__
#define __CHARACTER_WRAPPER_H__

#include <luabind\wrapper_base.hpp>
#include <string>
#include "Scripting\ScriptManager.h"

#include "Characters\Character.h"

#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

// ----------------------------------------
//		WRAPPER PARA LUA
// ----------------------------------------
struct CCharacter_Wrapper: CCharacter, luabind::wrap_base
{
	CCharacter_Wrapper(int Id)
		:CCharacter(Id)
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CCharacter_Wrapper] constructor");
	}

	CCharacter_Wrapper(int Id, const std::string &_Name)
		:CCharacter(Id, _Name)
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CCharacter_Wrapper] constructor");
	}

	virtual ~CCharacter_Wrapper()
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CCharacter_Wrapper] destructor");
	}

	virtual bool Init()
	{
		 bool l = call<bool>("init");
		 //int s = this->GetAnimationId("run");		// Això sembla que funciona
		 return l;
	}

	virtual void Update(float _ElapsedTime)
	{
		call<void>("update", _ElapsedTime);
	}

	static void default_update(CCharacter *ptr, float _ElapsedTime)
	{
		ptr->CCharacter::Update(_ElapsedTime);
	}
};


#endif /* __CHARACTER_WRAPPER_H__ */