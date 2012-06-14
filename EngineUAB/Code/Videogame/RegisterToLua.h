#pragma once 

#ifndef __REGISTERTOLUA_H__
#define __REGISTERTOLUA_H__

#include <luabind/luabind.hpp>
using namespace luabind;

enum {
	ent_caperucita= 1, 
	ent_lobo = 2
};

void RegisterToLuaGlobals				( lua_State* _pLua );
void RegisterToLuaTelegram				( lua_State* _pLua );
void RegisterToLuaCObject3D				( lua_State* _pLua );
void RegisterToLuaBaseGameEntity		( lua_State* _pLua );
void RegisterToLuaCNamed				( lua_State* _pLua );
void RegisterToLuaCharacter				( lua_State* _pLua );
void RegisterToLuaCharacterManager		( lua_State *_pLua );
void RegisterToLuaStateMachine			( lua_State* _pLua );
void RegisterToLuaState					( lua_State* _pLua );
void RegisterToLuaProperties			( lua_State* _pLua );

#endif /* __REGISTERTOLUA_H__ */
