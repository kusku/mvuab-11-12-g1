#include "ScriptManager.h"

#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// Esto registra una classe de C++ usando Luabind y ser expuesta para ser llamada desde LUA
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) { luabind::module(m_pLuaState) [ luabind::def(FunctionName,AddrFunction) ]; }

// Para llamar desde LUA a una función de C++ primero debemos crear otra función en C++ igual pasandole el Lua State y devolviendo el int
int Alert(lua_State * State);

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CScriptManager::CScriptManager ( void )
	: m_FileName("")
{	
	Initialize();
}

CScriptManager::~CScriptManager ( void )
{
	Destroy();
	m_FileNamesVector.clear();
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------

void CScriptManager::Initialize ( void )
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs ( m_pLuaState );

	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA. Esto sin LuaBind.
	lua_register ( m_pLuaState, "_ALERT", Alert );		// Así registramos una función de C++ para acceder desde Lua. Ejemplo de "Exposing a C/C++ Function to Lua"
	luabind::open(m_pLuaState);
	
	RegisterLUAMethods();
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
void CScriptManager::Destroy ( void )
{
	lua_close( m_pLuaState );
}

// Per carregar un fitxer XML amb tots els scrits
bool CScriptManager::Load ( const std::string &_XMLFile )
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
		const char *l_Str = lua_tostring ( m_pLuaState, -1 );
		LOGGER->AddNewLog ( ELL_ERROR, l_Str );
	}
}

//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
	// Compila y ejecuta un fichero
	if ( luaL_dofile( m_pLuaState, FileName.c_str() ) )
	{
		const char *l_Str = lua_tostring ( m_pLuaState, -1 );
		LOGGER->AddNewLog ( ELL_ERROR, l_Str );
	}
}


// -----------------------------------------
//					MÈTODES
// -----------------------------------------

// Per recarregar el XML
bool CScriptManager::Reload ( void )
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

bool CScriptManager::LoadXML ( void ) 
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
				else
				{
					LOGGER->AddNewLog( ELL_WARNING, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't contain any tag different form <SCRIPT>.", m_FileName ); 
				}
			}
		}
		else
		{
			LOGGER->AddNewLog( ELL_ERROR, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't contain tag <SCRIPTS>.", m_FileName ); 
			return false;
		}
	}
	else 
	{
		LOGGER->AddNewLog( ELL_ERROR, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't exist or contain sintaxis errors.", m_FileName ); 
		return false;
	}

	return true;
}

// Ejemplo de "Exposing a C/C++ Function to Lua"
void PrintLogger(int Level, const std::string &Msg)
{
	switch(Level)
	{
	case 0:
		LOGGER->AddNewLog(ELL_INFORMATION, Msg.c_str() );
		break;
	case 1:
		LOGGER->AddNewLog(ELL_WARNING, Msg.c_str() );
		break;
	case 2:
		LOGGER->AddNewLog(ELL_ERROR, Msg.c_str() );
		break;
	default:
		LOGGER->AddNewLog(ELL_INFORMATION, Msg.c_str() );
		break;
	}
}


//----------------------------------------------------------------------------------------
// RegisterLUAFunctions: Switch de los diferentes métodos a registrar
//----------------------------------------------------------------------------------------
void CScriptManager::RegisterLUAMethods()
{
	REGISTER_LUA_FUNCTION("print_logger", PrintLogger);

	module(m_pLuaState) [
		class_<CSingleton<CCore>>("CSingleton_CCore")
			.scope 
			[
				def("get_singleton", &CSingleton<CCore>::GetSingletonPtr)
			]
	];

	RegisterMathMethods();
}

//----------------------------------------------------------------------------
// RegisterMathLUAFunctions: Registrem totes les funcions de Matemàtiques
//----------------------------------------------------------------------------
void CScriptManager::RegisterMathMethods()
{
	module(m_pLuaState) [
		class_<Vect2f>("Vect2f")
			.def(constructor<float, float>())
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self + float())
			.def(const_self - float())
			.def(const_self * float())
			.def(const_self / float())
			.def(const_self == const_self)
			.def("normalize", &Vect2f::Normalize)
			.def("length", &Vect2f::Length)
			.def("distance", &Vect2f::Distance)
			.def("zero", &Vect2f::SetZero)
			.def_readwrite("x", &Vect2f::x)
			.def_readwrite("y", &Vect2f::y)
	];

	module(m_pLuaState) [
		class_<Vect3f>("Vect3f")
			.def(constructor<float, float, float>())
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self + float())
			.def(const_self - float())
			.def(const_self * float())
			.def(const_self / float())
			.def(const_self == const_self)
			.def("normalize", &Vect3f::Normalize)
			.def("length", &Vect3f::Length)
			.def("distance", &Vect3f::Distance)
			.def("zero", &Vect3f::SetZero)
			.def_readwrite("x", &Vect3f::x)
			.def_readwrite("y", &Vect3f::y)
			.def_readwrite("z", &Vect3f::z)
	];

	module(m_pLuaState) [
		class_<Vect4f>("Vect4f")
			.def(constructor<float, float, float, float>())
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self + float())
			.def(const_self - float())
			.def(const_self * float())
			.def(const_self / float())
			.def(const_self == const_self)
			.def("normalize", &Vect4f::Normalize)
			.def("length", &Vect4f::Length)
			.def("zero", &Vect4f::SetZero)
			.def_readwrite("x", &Vect4f::x)
			.def_readwrite("y", &Vect4f::y)
			.def_readwrite("z", &Vect4f::z)
			.def_readwrite("w", &Vect4f::w)
	];

	module(m_pLuaState) [
		class_<Vect2i>("Vect2i")
			.def(constructor<int, int>())
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self + int())
			.def(const_self - int())
			.def(const_self * int())
			.def(const_self / int())
			.def(const_self == const_self)
			.def("zero", &Vect2i::SetZero)
			.def_readwrite("x", &Vect2i::x)
			.def_readwrite("y", &Vect2i::y)
	];

	module(m_pLuaState) [
		class_<Vect3i>("Vect3i")
			.def(constructor<int, int, int>())
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self + int())
			.def(const_self - int())
			.def(const_self * int())
			.def(const_self / int())
			.def(const_self == const_self)
			.def("zero", &Vect3i::SetZero)
			.def_readwrite("x", &Vect3i::x)
			.def_readwrite("y", &Vect3i::y)
			.def_readwrite("z", &Vect3i::z)
	];

	module(m_pLuaState) [
		class_<Vect4i>("Vect4i")
			.def(constructor<int, int, int, int>())
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self + int())
			.def(const_self - int())
			.def(const_self * int())
			.def(const_self / int())
			.def(const_self == const_self)
			.def("zero", &Vect4i::SetZero)
			.def_readwrite("x", &Vect4i::x)
			.def_readwrite("y", &Vect4i::y)
			.def_readwrite("z", &Vect4i::z)
			.def_readwrite("w", &Vect4i::w)
	];

	module(m_pLuaState) [
		class_<Mat33f>("Mat33f")
			.def(constructor<float, float, float, float, float, float, float, float, float>())
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self * float())
			.def(const_self / float())
			.def(const_self == const_self)
			.def("identity", &Mat33f::SetIdentity)
			.def("zero", &Mat33f::SetZero)
			.def("reset_rotation", &Mat33f::ResetRotation)
			.def("reset_scale", &Mat33f::ResetScale)
			.def("set_from_angle_x", &Mat33f::SetFromAngleX)
			.def("set_from_angle_y", &Mat33f::SetFromAngleY)
			.def("set_from_angle_z", &Mat33f::SetFromAngleZ)
			.def("set_from_angles_xz", &Mat33f::SetFromAnglesXZ)
			.def("set_from_angles_yxz", &Mat33f::SetFromAnglesYXZ)
			.def("set_rot_by_angle_x", &Mat33f::SetRotByAngleX)
			.def("set_rot_by_angle_y", &Mat33f::SetRotByAngleY)
			.def("set_rot_by_angle_z", &Mat33f::SetRotByAngleZ)
			.def("set_rot_by_angles_xz", &Mat33f::SetRotByAnglesXZ)
			.def("set_rot_by_angles_yxz", &Mat33f::SetRotByAnglesYXZ)
			.def("get_scale", &Mat33f::GetScale)
			.def("get_scale_x", &Mat33f::GetScaleX)
			.def("get_scale_y", &Mat33f::GetScaleY)
			.def("get_scale_z", &Mat33f::GetScaleZ)
			.def("get_angle_x", &Mat33f::GetAngleX)
			.def("get_angle_y", &Mat33f::GetAngleY)
			.def("get_angle_z", &Mat33f::GetAngleZ)
			.def("get_yaw", &Mat33f::GetYaw)
			.def("get_pitch", &Mat33f::GetPitch)
			.def("get_roll", &Mat33f::GetRoll)
			.def("get_pitch_roll_yaw", &Mat33f::GetPitchRollYaw)
			.def("set_pitch_roll_yaw", &Mat33f::SetPitchRollYaw)
			.def("invert", &Mat33f::Invert)
			.def("transpose", &Mat33f::Transpose)
			.def_readwrite("m00", &Mat33f::m00)
			.def_readwrite("m01", &Mat33f::m01)
			.def_readwrite("m02", &Mat33f::m02)
			.def_readwrite("m10", &Mat33f::m10)
			.def_readwrite("m11", &Mat33f::m11)
			.def_readwrite("m12", &Mat33f::m12)
			.def_readwrite("m20", &Mat33f::m20)
			.def_readwrite("m21", &Mat33f::m21)
			.def_readwrite("m22", &Mat33f::m22)	
	];
}