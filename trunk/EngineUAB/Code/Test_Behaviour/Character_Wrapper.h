
#ifndef __CHARACTER_WRAPPER_H__
#define __CHARACTER_WRAPPER_H__

#include <luabind\wrapper_base.hpp>
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

	virtual ~CCharacter_Wrapper()
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CCharacter_Wrapper] destructor");
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