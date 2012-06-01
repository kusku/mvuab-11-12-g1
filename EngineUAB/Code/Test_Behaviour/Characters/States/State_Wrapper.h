
#ifndef __STATE_WRAPPER_H__
#define __STATE_WRAPPER_H__

#include <luabind\wrapper_base.hpp>
#include <string>
#include "Scripting\ScriptManager.h"

#include "StatesMachine\State.h"
#include "Characters\Character.h"

#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

// ----------------------------------------
//		WRAPPER PARA LUA
// ----------------------------------------
struct CState_Wrapper: CState<CCharacter>, luabind::wrap_base
{
	CState_Wrapper()
		:CState()
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CState_Wrapper] constructor");
	}

	virtual ~CState_Wrapper()
	{
		LOGGER->AddNewLog ( ELL_INFORMATION, "[C++CState_Wrapper] destructor");
	}

	virtual void Execute(CCharacter *_pCharacter)
	{
		 call<void>("Execute", _pCharacter);
	}

	virtual void OnEnter(CCharacter *_pCharacter)
	{
		call<void>("OnEnter", _pCharacter);
	}

	virtual void OnExit(CCharacter *_pCharacter)
	{
		call<void>("OnExit", _pCharacter);
	}
	
	/*virtual bool OnMessage(CCharacter *_pCharacter, )
	{
		ptr->CCharacter::Update(_ElapsedTime);
	}*/
};


#endif /* __STATE_WRAPPER_H__ */