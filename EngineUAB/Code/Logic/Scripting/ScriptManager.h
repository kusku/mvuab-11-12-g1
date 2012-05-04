#pragma once

#ifndef __SCRIPT_MANAGER_CLASS_H__
#define __SCRIPT_MANAGER_CLASS_H__

#include <string>
#include <vector>

#include "Utils\Singleton.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class CScriptManager : public CSingleton<CScriptManager>
{
public:
	//--- Init and End protocols------------------------------------------
				CScriptManager			( void );
	virtual		~CScriptManager			( void );

	//---- Main Functions ---------------------------------------
	void		Initialize				( void );
	void		Destroy					( void );
	void		RunCode					( const std::string &Code ) const;
	void		RunFile					( const std::string &FileName ) const;
	bool		Load					( const std::string &_XMLFile );

	//---- Functions ---------------------------------------
	bool		LoadXML						( void );
	bool		Reload						( void );

private:
	void		RegisterLUAFunctions		( void );
	void		RegisterGUIFunctions		( void );
	void		RegisterMathLUAFunctions	( void );
	void		RegisterSoundFunctions		( void );
	void		RegisterTriggerLUAFunctions ( void );
 	
	//---- Properties ( get & Set )---------------------------------------
public:
	lua_State * GetLuaState				( void ) const { return m_LS; }
	
	//---- Members -------------------------------------------------------
private:
	lua_State		*m_LS;			// Estat del lua
	std::string		m_FileName;		// Fitxer xml on trobaré els scripts a registrar
};

#endif __SCRIPT_MANAGER_CLASS_H__