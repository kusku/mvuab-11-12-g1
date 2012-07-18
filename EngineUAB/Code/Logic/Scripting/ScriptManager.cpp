#include "ScriptManager.h"

#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// Para llamar desde LUA a una función de C++ primero debemos crear otra función en C++ igual pasandole el Lua State y devolviendo el int
int Alert(lua_State * State);

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CScriptManager::CScriptManager()
	: m_FileName("")
{	
	Initialize();
}

CScriptManager::~CScriptManager()
{
	Destroy();
	m_FileNamesVector.clear();
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------

void CScriptManager::Initialize()
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs ( m_pLuaState );

	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA. Esto sin LuaBind.
	lua_register ( m_pLuaState, "_ALERT", Alert );		// Así registramos una función de C++ para acceder desde Lua. Ejemplo de "Exposing a C/C++ Function to Lua"
	luabind::open(m_pLuaState);
	
	RegisterLUAFunctions();
}

// Wrapper para llamar la función de alerta de Lua y meter la info en nuestro Alert de C++, és decir, el LOGGER
// Ejemplo de "Exposing a C/C++ Function to Lua"
int Alert( lua_State * _pState )
{
	std::string l_Text;
	int n = lua_gettop( _pState );				// Obtengo el top element de la pila
	int i;
	lua_getglobal( _pState , "tostring" );		
	for ( i = 1; i <= n; i++ ) 
	{
		const char *s;
		lua_pushvalue( _pState, -1 );
		lua_pushvalue( _pState, i );
		lua_call( _pState, 1, 1 );				// Llamamos a la función con el Lua State, el nº de parámetros y el nº de resultados devueltos
		s = lua_tostring( _pState, -1 );
		if ( s == NULL )
			return luaL_error( _pState, "`tostring' must return a string to `print'" );

		if (i>1) l_Text += '\t';
		l_Text += s;
		lua_pop( _pState, 1 );
	}
	l_Text += '\n';
	LOGGER->AddNewLog( ELL_ERROR, l_Text.c_str() );
	return true;
}

//Para desinicializar el motor de LUA
void CScriptManager::Destroy()
{
	lua_close( m_pLuaState );
}

// Per carregar un fitxer XML amb tots els scrits
bool CScriptManager::Load( const std::string &_XMLFile )
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CScriptManager::Load-->Loading scripts." );
	m_FileName = _XMLFile;
	m_FileNamesVector.push_back(m_FileName);

	return LoadXML();
}

//Para ejecutar un fragmento de código LUA. Esto es un ejemplo de "Accessing a Lua Function from within C++"
void CScriptManager::RunCode ( const std::string &_Code ) const
{
	// Compila y ejecuta el string
	if ( luaL_dostring( m_pLuaState, _Code.c_str() ) )
	{
		const char *l_Str = lua_tostring( m_pLuaState, -1 );
		LOGGER->AddNewLog( ELL_ERROR, l_Str );
	}
}

//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
	// Compila y ejecuta un fichero
	if ( luaL_dofile( m_pLuaState, FileName.c_str() ) )
	{
		const char *l_Str = lua_tostring( m_pLuaState, -1 );
 		LOGGER->AddNewLog( ELL_ERROR, l_Str );
	}
}

// Per recarregar el XML
bool CScriptManager::Reload()
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CScriptManager::Reload->Reloading scripts." );
	//Destroy ();
	//Initialize();

	for(uint16 i=0; i<m_FileNamesVector.size(); ++i)
	{
		m_FileName = m_FileNamesVector[i];
		if( !LoadXML() )
		{
			return false;
		}
	}

	return true;
}

bool CScriptManager::LoadXML() 
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CScriptManager::LoadXML --> Loading Scripting Files..." );
	CXMLTreeNode l_File;
	
	if ( l_File.LoadFile( m_FileName.c_str () ) )
	{
		CXMLTreeNode l_Scripts = l_File["scripts"];
		if ( l_Scripts.Exists ( ) )
		{
			unsigned int l_NumChilds = l_Scripts.GetNumChildren ( );
			for ( unsigned int i = 0; i < l_NumChilds; i ++ )
			{
				std::string l_Type = l_Scripts(i).GetName();
				if ( l_Type == "script" ) 
				{
					std::string l_FileName = l_Scripts(i).GetPszProperty ( "filename" );		// Obtenemos la ruta del fichero lua
					RunFile ( l_FileName );														// Ejecutamos el fichero lua
				}
				else if ( l_Type != "comment" ) 
				{
					LOGGER->AddNewLog( ELL_WARNING, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't contain any tag different form <SCRIPT>.", m_FileName.c_str() ); 
				}
			}
		}
		else 
		{
			LOGGER->AddNewLog( ELL_ERROR, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't exist.", m_FileName.c_str() ); 
			return false;
		}
	}
	else 
	{
		LOGGER->AddNewLog( ELL_ERROR, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't exist or contain sintaxis errors.", m_FileName.c_str() ); 
		return false;
	}

	return true;
}

struct my_exception
{};

void translate_my_exception(lua_State* L, my_exception const&)
{
    lua_pushstring(L, "my_exception");
}


void my_exception()
{
    //lua_pushstring(L, "my_exception");
}

//----------------------------------------------------------------------------------------
// RegisterLUAFunctions: Switch de los diferentes métodos a registrar
//----------------------------------------------------------------------------------------
void CScriptManager::RegisterLUAFunctions()
{
	REGISTER_LUA_FUNCTION("print_logger", Base::Logger::PrintLogger);
}