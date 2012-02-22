#include "ScriptManager.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
using namespace luabind;

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) { luabind::module(m_LS) [ luabind::def(FunctionName,AddrFunction) ]; }

int Alert(lua_State * State);

CScriptManager::CScriptManager()
	: m_FileName("")
{	
	Initialize();
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

bool CScriptManager::Load(const std::string &XMLFile)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CScriptManager::Load->Cargando los scripts.");
	m_FileName = XMLFile;
	return LoadFile();
}

bool CScriptManager::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CScriptManager::Reload->Reload de los scripts.");
	return LoadFile();
}

bool CScriptManager::LoadFile()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CScriptManager::LoadFile->Error al intentar leer el archivo de scripts: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_Node = newFile["scripts"];
	if( l_Node.Exists() )
	{
		uint16 l_Count = l_Node.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_Node(i).GetName();
			if( l_Type == "script" )
			{
				std::string l_Path = l_Node(i).GetPszProperty("filename", "");
				RunFile(l_Path);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

//void SetSpeedPlayer(int Speed);
void SetSpeedPlayer(int Speed)
{
	int PlayerSpeed = Speed;
}

void PrintLogger(const std::string &Msg)
{
	LOGGER->AddNewLog(ELL_INFORMATION, Msg.c_str() );
}

void CScriptManager::RegisterLUAFunctions()
{
	REGISTER_LUA_FUNCTION("set_speed_player", SetSpeedPlayer);
	REGISTER_LUA_FUNCTION("print_logger", PrintLogger);
}