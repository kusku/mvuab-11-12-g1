#pragma once

#ifndef __SCRIPT_MANAGER_CLASS_H__
#define __SCRIPT_MANAGER_CLASS_H__

#include <string>
#include <vector>

#include "ScriptingDefs.h"
#include "Utils\Singleton.h"
#include "LuaHelperFunctions.h"

// Esto registra una classe de C++ usando Luabind y ser expuesta para ser llamada desde LUA
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) { luabind::module(m_pLuaState) [ luabind::def(FunctionName,AddrFunction) ]; }

class CScriptManager : public CSingleton<CScriptManager>
{
public:
	//--- Init and End protocols------------------------------------------
				CScriptManager				();
	virtual		~CScriptManager				();

	//---- Main Functions ---------------------------------------
	void		Initialize					();
	void		Destroy						();
	void		RunCode						( const std::string &_Code ) const;
	void		RunFile						( const std::string &_FileName ) const;
	bool		Load						( const std::string &_XMLFile );

	//---- Functions ---------------------------------------
	bool		LoadXML						();
	bool		Reload						();

	//---- Register Methods -------------------------------------
	void		RegisterLUAFunctions		();
 	
	//---- Properties ( get & Set )---------------------------------------
	lua_State * GetLuaState					() const				{ return m_pLuaState; }
	
	int			GetInt						( char* _VariableName )		{ return Logic::LuaHelper::PopLuaNumber<int> ( m_pLuaState, _VariableName ); }
    double		GetFloat					( char* _VariableName )		{ return Logic::LuaHelper::PopLuaNumber<float> ( m_pLuaState, _VariableName ); }
	double		GetDouble					( char* _VariableName )		{ return Logic::LuaHelper::PopLuaNumber<double> ( m_pLuaState, _VariableName ); }
	std::string GetString					( char* _VariableName )		{ return Logic::LuaHelper::PopLuaString ( m_pLuaState, _VariableName ); }
	bool		GetBool						( char* _VariableName )		{ return Logic::LuaHelper::PopLuaBool ( m_pLuaState, _VariableName ); }

protected:
	lua_State	  * m_pLuaState;			// Estat del lua
	std::string		m_FileName;				// Fitxer xml on trobaré els scripts a registrar
	std::vector<std::string> m_FileNamesVector;
};

#endif __SCRIPT_MANAGER_CLASS_H__