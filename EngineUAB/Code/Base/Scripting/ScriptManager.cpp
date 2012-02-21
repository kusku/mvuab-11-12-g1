#include "ScriptManager.h"
#include "Logger\Logger.h"
#include "Base.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
using namespace luabind;

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(m_LS) [ luabind::def(FunctionName,AddrFunction) ];}

int Alert(lua_State * State);

CScriptManager::CScriptManager()
{	
}

CScriptManager::~CScriptManager()
{
	Destroy();
}

void CScriptManager::Initialize()
{
	m_LS=luaL_newstate();
	luaL_openlibs(m_LS);
	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA
	lua_register(m_LS,"_ALERT",Alert);
	luabind::open(m_LS);
	RegisterLUAFunctions();
}

int Alert(lua_State * State)
{
	std::string l_Text;
	int n = lua_gettop(State);
	int i;
	lua_getglobal(State, "tostring");
	for (i=1; i<=n; i++) 
	{
		const char *s;
		lua_pushvalue(State, -1);
		lua_pushvalue(State, i);
		lua_call(State, 1, 1);
		s = lua_tostring(State, -1);
		if (s == NULL)
			return luaL_error(State, "`tostring' must return a string to `print'");

		if (i>1) l_Text += '\t';
		l_Text += s;
		lua_pop(State, 1);
	}
	l_Text += '\n';
	LOGGER->AddNewLog( ELL_ERROR, l_Text.c_str() );
	return true;
}

void CScriptManager::Destroy()
{
	lua_close(m_LS);
}

void CScriptManager::RunCode(const std::string &Code) const
{
	if(luaL_dostring(m_LS,Code.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		LOGGER->AddNewLog( ELL_ERROR, l_Str );
	}
}

void CScriptManager::RunFile(const std::string &FileName) const
{
	if(luaL_dofile(m_LS, FileName.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		LOGGER->AddNewLog( ELL_ERROR, l_Str );
	}
}

void CScriptManager::Load(const std::string &XMLFile)
{
}

//void SetSpeedPlayer(int Speed);
int SetSpeedPlayer(lua_State *L)
{
	int l_Speed = (int)lua_tointeger(L,1);
	return 0;
}

void CScriptManager::RegisterLUAFunctions()
{
	lua_register(m_LS, "set_speed_player", SetSpeedPlayer);
}