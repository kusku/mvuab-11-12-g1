#pragma once

#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include <string>
#include <vector>
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class CScriptManager
{
public:
	CScriptManager();
	~CScriptManager();

	void Initialize();
	void Destroy();

	void RunCode	(const std::string &Code) const;
	void RunFile	(const std::string &FileName) const;

	bool Load		(const std::string &XMLFile);
	bool Reload		();

	lua_State * GetLuaState() const { return m_LS; }

private:
	void RegisterLUAFunctions	();
	bool LoadFile				();

	lua_State			*m_LS;
	std::string			m_FileName;
};

#endif