#ifndef __LUA_HELPER_FUNCS__
#define __LUA_HELPER_FUNCS__

#include <string>

//get rid of the annoying but unavoidable forcing int to bool warning
#pragma warning (disable:4800)

extern "C"
{
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}


#include <string>
//#include "misc/utils.h"
//#include "misc/Stream_Utility_Functions.h"


//----------------------------- PopLuaNumber-----------------------------------
//
//  Función templatizada para recuperar un numero de una pila de LUA
//-----------------------------------------------------------------------------
template <class T>
inline T PopLuaNumber(lua_State* pL, const char* name)
{
	lua_settop(pL, 0);

	lua_getglobal(pL, name);

	// Chequeamos que la variable es del tipo correcto. Sino, se lanza una excepción
	if (!lua_isnumber(pL, 1))
	{
		std::string err("<PopLuaNumber> Cannot retrieve: ");

		throw std::runtime_error(err + name);
	}

	// Tomamos el valor, lo convertimos al tipo correcto y lo devolvemos
	T val = (T)lua_tonumber(pL, 1);

	// se elimina el valor de la pila
	lua_pop(pL, 1);

	return val;
}

//--------------------------- PopLuaString ------------------------------------
//-----------------------------------------------------------------------------
inline std::string PopLuaString(lua_State* pL, const char* name)
{
	lua_settop(pL, 0);

	lua_getglobal(pL, name);

	// Chequeamos que la variable es del tipo correcto. Sino, se lanza una excepción
	if (!lua_isstring(pL, 1))
	{
		std::string err("<PopLuaString> Cannot retrieve: ");

		throw std::runtime_error(err + name);
	}

	// Tomamos el valor, lo convertimos al tipo correcto y lo devolvemos
	std::string s = lua_tostring(pL, 1);

	// se elimina el valor de la pila
	lua_pop(pL, 1);

	return s;
}

//--------------------------- PopLuaBool ------------------------------------
//-----------------------------------------------------------------------------
inline bool PopLuaBool(lua_State* pL, const char* name)
{
	lua_settop(pL, 0);

	lua_getglobal(pL, name);

	// Chequeamos que la variable es del tipo correcto. Sino, se lanza una excepción
	if (!lua_isstring(pL, 1))
	{
		std::string err("<PopLuaBool> Cannot retrieve: ");

		throw std::runtime_error(err + name);
	}

	// Tomamos el valor, lo convertimos al tipo correcto y lo devolvemos
	bool b = lua_toboolean(pL, 1);

	// se elimina el valor de la pila
	lua_pop(pL, 1);

	return b;
}

//------------------------- LuaPopStringFieldFromTable ------------------------
//-----------------------------------------------------------------------------
inline std::string LuaPopStringFieldFromTable(lua_State* pL, const char* key)
{

	// ponemos la key en la pila 
	lua_pushstring(pL, key);

	// La tabla es ahora en -2 (key es en -1). lua_gettable quita la llave de la pila 
	// y pone los datos encontrados en la localizació de la key en la pila 
	lua_gettable(pL, -2);

	// Chequeamos que la variable es del tipo correcto. Sino, se lanza una excepción
	if (!lua_isstring(pL, -1))
	{
		std::string err("<LuaPopStringFieldFromTable> Cannot retrieve: ");

		throw std::runtime_error(err + key);
	}

	// Tomamos los datos
	std::string s = lua_tostring(pL, -1);

	lua_pop(pL, 1);

	return s;
}

//----------------------------- LuaPopNumberFieldFromTable --------------------
//-----------------------------------------------------------------------------
template <class T>
inline T LuaPopNumberFieldFromTable(lua_State* pL, const char* key)
{
	// ponemos la key en la pila 
	lua_pushstring(pL, key);

	// La tabla es ahora en -2 (key es en -1). lua_gettable quita la llave de la pila 
	// y pone los datos encontrados en la localizació de la key en la pila 
	lua_gettable(pL, -2);

	// Chequeamos que la variable es del tipo correcto. Sino, se lanza una excepción
	if (!lua_isnumber(pL, -1))
	{
		std::string err("<LuaPopNumberFieldFromTable> Cannot retrieve: ");

		throw std::runtime_error(err + key);
	}

	// Tomamos los datos
	T val = (T)lua_tonumber(pL, -1);

	lua_pop(pL, 1);

	return val;
}

#endif __LUA_HELPER_FUNCS__	