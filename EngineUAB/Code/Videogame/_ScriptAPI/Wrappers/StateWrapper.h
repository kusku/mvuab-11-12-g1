#pragma once

#ifndef __STATE_WRAPPER_H__
#define __STATE_WRAPPER_H__

#include <luabind\wrapper_base.hpp>
#include <string>
#include "Scripting\ScriptManager.h"

#include "StatesMachine\State.h"
#include "Characters\Character.h"
#include "StatesMachine\Telegram.h"

#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

// ----------------------------------------
//              WRAPPER PARA LUA
// ----------------------------------------
struct CState_Wrapper: CState<CCharacter>, luabind::wrap_base
{
	CState_Wrapper(const std::string& _Name)
		:CState(_Name)
	{
	}

	virtual ~CState_Wrapper()
	{
	}

	virtual void Execute(CCharacter *_pCharacter, float _ElapsedTime)
	{
		call<void>("Execute", _pCharacter, _ElapsedTime);
	}

	virtual void OnEnter(CCharacter *_pCharacter)
	{
		call<void>("OnEnter", _pCharacter);
	}

	virtual void OnExit(CCharacter *_pCharacter)
	{
		call<void>("OnExit", _pCharacter);
	}

	virtual bool OnMessage(CCharacter *_pCharacter, const STelegram &_Msg )
	{
		return call<bool>("OnMessage", _pCharacter, _Msg );
	}
};


#endif /* __STATE_WRAPPER_H__ */

