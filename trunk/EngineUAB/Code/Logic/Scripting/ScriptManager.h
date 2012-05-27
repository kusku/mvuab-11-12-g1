#pragma once

#ifndef __SCRIPT_MANAGER_CLASS_H__
#define __SCRIPT_MANAGER_CLASS_H__

#include <string>
#include <vector>

#include "ScriptingDefs.h"
#include "Utils\Singleton.h"
#include "LuaHelperFunctions.h"

class CScriptManager : public CSingleton<CScriptManager>
{
public:
	//--- Init and End protocols------------------------------------------
				CScriptManager				( void );
	virtual		~CScriptManager				( void );

	//---- Main Functions ---------------------------------------
	void		Initialize					( void );
	void		Destroy						( void );
	void		RunCode						( const std::string &_Code ) const;
	void		RunFile						( const std::string &_FileName ) const;
	bool		Load						( const std::string &_XMLFile );

	//---- Functions ---------------------------------------
	bool		LoadXML						( void );
	bool		Reload						( void );

	//---- Register Methods -------------------------------------
	void		RegisterLUAMethods			();
 	
	//---- Properties ( get & Set )---------------------------------------
	lua_State * GetLuaState					( void ) const				{ return m_pLuaState; }
	
	int			GetInt						( char* _VariableName )		{ return PopLuaNumber<int> ( m_pLuaState, _VariableName ); }
    double		GetFloat					( char* _VariableName )		{ return PopLuaNumber<float> ( m_pLuaState, _VariableName ); }
	double		GetDouble					( char* _VariableName )		{ return PopLuaNumber<double> ( m_pLuaState, _VariableName ); }
	std::string GetString					( char* _VariableName )		{ return PopLuaString ( m_pLuaState, _VariableName ); }
	bool		GetBool						( char* _VariableName )		{ return PopLuaBool ( m_pLuaState, _VariableName ); }
	
private:
	void		RegisterMathMethods			( void );

protected:
	lua_State	  * m_pLuaState;			// Estat del lua
	std::string		m_FileName;				// Fitxer xml on trobaré els scripts a registrar
	std::vector<std::string> m_FileNamesVector;
};

#endif __SCRIPT_MANAGER_CLASS_H__