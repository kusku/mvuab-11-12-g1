#include "ScriptManager.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "ScriptingDefs.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include <luabind/return_reference_to_policy.hpp>

using namespace luabind;

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) { luabind::module(m_LS) [ luabind::def(FunctionName,AddrFunction) ]; }

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
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------

void CScriptManager::Initialize ( void )
{
	m_LS = luaL_newstate();
	luaL_openlibs ( m_LS );

	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA
	lua_register ( m_LS, "_ALERT", Alert );
	luabind::open(m_LS);
	
	RegisterLUAMethods(CORE_SCRIPT);
	RegisterLUAMethods(GRAPHICS_SCRIPT);
	RegisterLUAMethods(GUI_SCRIPT);
	RegisterLUAMethods(DEBUG_GUI_SCRIPT);
	RegisterLUAMethods(INPUT_SCRIPT);
	RegisterLUAMethods(LOGIC_SCRIPT);
	RegisterLUAMethods(PHYSICS_SCRIPT);
	RegisterLUAMethods(SOUND_SCRIPT);
	RegisterLUAMethods(MATH_SCRIPT);
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

//Para desinicializar el motor de LUA
void CScriptManager::Destroy ( void )
{
	lua_close ( m_LS );
}

// Per carregar un fitxer XML amb tots els scrits
bool CScriptManager::Load ( const std::string &_XMLFile )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CScriptManager::Load-->Loading scripts." );
	m_FileName = _XMLFile;
	return LoadXML();
}

//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode ( const std::string &_Code ) const
{
	if ( luaL_dostring ( m_LS, _Code.c_str() ) )
	{
		const char *l_Str = lua_tostring ( m_LS, -1 );
		LOGGER->AddNewLog ( ELL_ERROR, l_Str );
	}
}

//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
	if ( luaL_dofile ( m_LS, FileName.c_str() ) )
	{
		const char *l_Str = lua_tostring ( m_LS, -1 );
		LOGGER->AddNewLog ( ELL_ERROR, l_Str );
	}
}


// -----------------------------------------
//					MÈTODES
// -----------------------------------------

// Per recarregar el XML
bool CScriptManager::Reload ( void )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CScriptManager::Reload->Reloading scripts." );
	//Destroy ();
	return LoadXML ();
}

bool CScriptManager::LoadXML ( void ) 
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CScriptManager::LoadXML --> Loading Scripting Files..." );
	CXMLTreeNode l_File;
	
	if ( l_File.LoadFile ( m_FileName.c_str () ) )
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
					LOGGER->AddNewLog ( ELL_WARNING, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't contain any tag different form <SCRIPT>.", m_FileName ); 
				}
			}
		}
		else
		{
			LOGGER->AddNewLog ( ELL_ERROR, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't contain tag <SCRIPTS>.", m_FileName ); 
			return false;
		}
	}
	else 
	{
		LOGGER->AddNewLog ( ELL_ERROR, "CScriptManager::LoadXML --> Error loading file %s. The file doesn't exist or contain sintaxis errors.", m_FileName ); 
		return false;
	}

	return true;
}

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
void CScriptManager::RegisterLUAMethods(ERegisterMethods type)
{
	switch(type)
	{
	case CORE_SCRIPT:
		{
			RegisterCoreMethods();
			break;
		}
	case GRAPHICS_SCRIPT:
		{
			RegisterGraphicsMethods();
			break;
		}
	case GUI_SCRIPT:
		{
			RegisterGUIMethods();
			break;
		}
	case DEBUG_GUI_SCRIPT:
		{
			RegisterDebugGUIMethods();
			break;
		}
	case INPUT_SCRIPT:
		{
			RegisterInputMethods();
			break;
		}
	case LOGIC_SCRIPT:
		{
			RegisterLogicMethods();
			break;
		}
	case NETWORK_SCRIPT:
		{
			RegisterNetworkMethods();
			break;
		}
	case PHYSICS_SCRIPT:
		{
			RegisterPhysicMethods();
			break;
		}
	case SOUND_SCRIPT:
		{
			RegisterSoundMethods();
			break;
		}
	case MATH_SCRIPT:
		{
			RegisterMathMethods();
			break;
		}
	case TRIGGER_SCRIPT:
		{
			RegisterTriggerMethods();
			break;
		}
	}
}

//----------------------------------------------------------------------------
// RegisterCoreMethods: Registramos los métodos del Core
//----------------------------------------------------------------------------
void CScriptManager::RegisterCoreMethods()
{
	REGISTER_LUA_FUNCTION("print_logger", PrintLogger);

	module(m_LS) [
		class_<CSingleton<CCore>>("CSingleton_CCore")
			.scope 
			[
				def("get_singleton", &CSingleton<CCore>::GetSingletonPtr)
			]
	];

	module(m_LS) [
		class_<CCore>("CCore")
			.def("reload_all", &CCore::Reload)
			.def("reload_fonts", &CCore::ReloadTTFs)
			.def("reload_languages", &CCore::ReloadLanguages)
			.def("reload_inputs", &CCore::ReloadInputs)
			.def("reload_render_commands", &CCore::ReloadSceneRendererCommandManager)
			.def("reload_renderable_objects_layers", &CCore::ReloadRenderableObjectsLayersManager)
			.def("reload_effects", &CCore::ReloadEffects)
			.def("reload_meshes", &CCore::ReloadMeshes)
			.def("reload_pools", &CCore::ReloadPools)
			.def("reload_scripts", &CCore::ReloadScripts)
			.def("reload_lights", &CCore::ReloadLights)
			.def("reload_physics", &CCore::ReloadPhysics)
			.def("reload_billboards", &CCore::ReloadBillboards)
			.def("reload_particles", &CCore::ReloadParticles)
			.def("reload_triggers", &CCore::ReloadTriggers)
			.def("reload_gui", &CCore::ReloadGUI)
			.def("reload_sounds", &CCore::ReloadSounds)
			.def("get_debug_gui_manager", &CCore::GetDebugGUIManager)
			.def("get_stadistics", &CCore::GetStadistics)
	];
}

//----------------------------------------------------------------------------
// RegisterGraphicsMethods: Registramos todos los métodos de gráficos
//----------------------------------------------------------------------------
void CScriptManager::RegisterGraphicsMethods()
{
}

//----------------------------------------------------------------------------
// RegisterGUIMethods: Registramos todos los métodos de la GUI
//----------------------------------------------------------------------------
void CScriptManager::RegisterGUIMethods( void )
{
	module(m_LS) [
		class_<CGUIManager>("CGUIManager")
			.def("active_windows", &CGUIManager::ActiveWindows)							// Activa la ventana pasada
			.def("active_windows_with_effect", &CGUIManager::ActiveWindowsWithEffect)	// Activa la ventana pasada con effecto
			.def("get_state_slider", &CGUIManager::GetStateSlider)						// Obtiene el valor del Slider indicado
			.def("set_state_slider", &CGUIManager::SetStateSlider)						// Coloca el valor del Slider indicado
			.def("push_windows", &CGUIManager::PushWindows)								// Almacena la ventana padre i activa la pasada
			.def("pop_windows", &CGUIManager::PopWindows)								// Retorna a la ventana anterior padre
			.def("play_image", &CGUIManager::PlayImage)									// Executa animacions d'imatges
	];


	module(m_LS) [
		class_<CGUIAnimatedImage>("CGUIAnimatedImage")
			.def("play_animation", &CGUIAnimatedImage::PlayAnimation)						// Executa animacions d'imatges
	];
}

//----------------------------------------------------------------------------
// RegisterDebugGUIMethods: Registramos todos los métodos de debug de la GUI
//----------------------------------------------------------------------------
void CScriptManager::RegisterDebugGUIMethods()
{
	module(m_LS) [
		class_<CDebugGUIManager>("CDebugGUIManager")
			.def("get_console",&CDebugGUIManager::GetConsole)
			.def("get_debug_render", &CDebugGUIManager::GetDebugRender)
			.def("get_debug_options", &CDebugGUIManager::GetDebugOptions)
	];

	module(m_LS) [
		class_<CConsole>("CConsole")
			.def("toggle", &CConsole::Toggle)
			.def("set_active", &CConsole::SetActive)
			.def("is_active", &CConsole::IsActive)
	];

	module(m_LS) [
		class_<CStadistics>("CStadistics")
			.def("get_draw_calls", &CStadistics::GetNumOfDrawCalls)
			.def("get_draw_debug_lines", &CStadistics::GetNumOfDebugLines)
			.def("get_vertices_in_frustum", &CStadistics::GetNumOfVerticesInFrustum)
			.def("get_triangles_in_frustum", &CStadistics::GetNumOfTrianglesInFrustum)
	];

	module(m_LS) [
		class_<CDebugOptions>("CDebugOptions")
			.def("set_bool", &CDebugOptions::SetBool)
			.def("set_int", &CDebugOptions::SetInt)
			.def("set_float", &CDebugOptions::SetFloat)
			.def("reload", &CDebugOptions::Reload)
	];

	module(m_LS) [
		class_<CDebugRender>("CDebugRender")
			.def("is_fps_visible", &CDebugRender::GetFPSVisible)
			.def("is_delta_time_visible", &CDebugRender::GetDeltaTimeVisible)
			.def("is_gamepad_visible", &CDebugRender::GetGamePadVisible)
			.def("toggle_fps", &CDebugRender::ToggleFPS)
			.def("toggle_delta_time", &CDebugRender::ToggleDeltaTime)
			.def("toggle_gamepad", &CDebugRender::ToggleGamePad)
	];
}

//----------------------------------------------------------------------------
// RegisterInputMethods: Registramos todos los métodos de input
//----------------------------------------------------------------------------
void CScriptManager::RegisterInputMethods()
{
}

//----------------------------------------------------------------------------
// RegisterLogicMethods: Registramos todos los métodos de lógica
//----------------------------------------------------------------------------
void CScriptManager::RegisterLogicMethods( void )
{
	module(m_LS) [
		class_<CTriggersManager>("CTriggersManager")
			.def("exist_fisic_trigger", &CTriggersManager::ExistFisicTrigger)			// Retorna si existe un trigger ya cargado
			.def("exist_trigger", &CTriggersManager::ExistTrigger)						// Retorna si existe un físic trigger asociado al trigger ya cargado
			.def("get_trigger", &CTriggersManager::GetTrigger)							// Obtiene el trigger del mapa de triggers
	];
}

//----------------------------------------------------------------------------
// RegisterNetworkMethods: Registramos todos los métodos de redes
//----------------------------------------------------------------------------
void CScriptManager::RegisterNetworkMethods()
{
}

//----------------------------------------------------------------------------
// RegisterPhysicsMethods: Registramos todos los métodos de física
//----------------------------------------------------------------------------
void CScriptManager::RegisterPhysicMethods()
{
}

//----------------------------------------------------------------------------
// RegisterSoundMethods: Registramos todos los métodos de sonido
//----------------------------------------------------------------------------
void CScriptManager::RegisterSoundMethods( void )
{
	module(m_LS) [
		class_<CSoundManager>("CSoundManager")
			.def("set_volume_value", &CSoundManager::SetSoundVolume)					// Coloca el valor del volumen de sonido per un test
			.def("get_volume_value", &CSoundManager::GetSoundVolume)					// Obtiene el valor del volumen de sonido per un test

			.def("Init", &CSoundManager::Init)											// Inicializa el device y contexto
			.def("Done", &CSoundManager::Done)											// Release
			.def("IsOk", &CSoundManager::IsOk)											// Dice si todo está correctamente inicializado

			.def("load", &CSoundManager::Load)											// Carga un fichero xml de sonidos
			.def("update", &CSoundManager::Update)										// Actualiza los sonidos, fades in/out
			.def("render", &CSoundManager::Render)										// Renderiza si se desea en modo debug para saber donde está cada elemento

			.def("load_sounds", &CSoundManager::LoadSounds)								// Carga los sonidos del XML predefinido en el método Load
			.def("reset", &CSoundManager::Reset)										// Para todos los sonidos y limpia buffers(sonidos) i sources (altavoces)
			.def("pause", &CSoundManager::Pause)										// Determina si hace pausa/play de todos los sonidos segun la variable global "m_bPause"
			.def("stop", &CSoundManager::Stop)											// Para todos los sonidos de los altavoces
			.def("sound_on", &CSoundManager::SoundOn)									// Setea el sonido activo
			.def("load_sounds", &CSoundManager::SoundOff)								// Setea mute
			.def("set_gain", &CSoundManager::SetGain)									// Setea el volumen general
			.def("get_gain", &CSoundManager::GetGain)									// Obtiene el valor del volumen de sonido 

			.def("play_action_2D", &CSoundManager::PlayAction2D)						// Ejecuta sonidos en 2D
			.def("play_action_3D", &CSoundManager::PlayAction3D)						// Ejecuta sonidos en 3D

			.def("create_source", &CSoundManager::CreateSource)							// Crea un source (altavoz)
			.def("delete_source", &CSoundManager::DeleteSource)							// Elimina un source (altavoz)
			.def("play_source2D", &CSoundManager::PlaySource2D)							// Ejecuta sonidos en 2D de un altavoz concreto
			.def("play_source3D", &CSoundManager::PlaySource3D)							// Ejecuta sonidos en 3D de un altavoz concreto
			.def("pause_source", &CSoundManager::PauseSource)							// Ejecuta un altavoz
			.def("stop_source", &CSoundManager::StopSource)								// Para un altavoz
			.def("set_sourceGain", &CSoundManager::SetSourceGain)						// Da volumen a un altavoz
			.def("get_sourceGain", &CSoundManager::GetSourceGain)						// Obtiene el volumen de un altavoz
			.def("set_sourcePosition", &CSoundManager::SetSourcePosition)				// Coloca la posición del altavoz
			.def("get_sourcePosition", &CSoundManager::GetSourcePosition)				// Obtiene la posición del altavoz
			.def("set_source_velocity", &CSoundManager::SetSourceVelocity)				// Coloca la velocidad del altavoz
			.def("get_source_velocity", &CSoundManager::GetSourceVelocity)				// Obtiene la velocidad del altavoz
			.def("fade_in_source", &CSoundManager::FadeInSource)						// Realiza una entrada del sonido
			.def("fade_out_source", &CSoundManager::FadeOutSource)						// Realiza una salida del sonido

			.def("set_listener_position", &CSoundManager::SetListenerPosition)			// Coloca la posición del listener
			.def("get_listener_position", &CSoundManager::GetListenerPosition)			// Obtiene la posición del listener
			.def("set_listener_velocity", &CSoundManager::SetListenerVelocity)			// Coloca la velocidad del listener
			.def("get_listener_velocity", &CSoundManager::GetListenerVelocity)			// Obtiene la velocidad del listener
			.def("set_listener_orientation", &CSoundManager::SetListenerOrientation)	// Coloca la orientacion del listener
			.def("get_listener_orientation", &CSoundManager::GetListenerOrientation)	// Obtiene la orientacion del listener	
	];
}

//----------------------------------------------------------------------------
// RegisterMathLUAFunctions: Registrem totes les funcions de Matemàtiques
//----------------------------------------------------------------------------
void CScriptManager::RegisterMathMethods()
{
	module(m_LS) [
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

	module(m_LS) [
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

	module(m_LS) [
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

	module(m_LS) [
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

	module(m_LS) [
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

	module(m_LS) [
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

	module(m_LS) [
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

//----------------------------------------------------------------------------
// RegisterSoundFunctions: Registrem totes les funcions de GUI
//----------------------------------------------------------------------------
void CScriptManager::RegisterTriggerMethods( void )
{
	module(m_LS) [
		class_<CTriggersManager>("CTriggersManager")
			.def("exist_fisic_trigger", &CTriggersManager::ExistFisicTrigger)			// Retorna si existe un trigger ya cargado
			.def("exist_trigger", &CTriggersManager::ExistTrigger)						// Retorna si existe un físic trigger asociado al trigger ya cargado
			.def("get_trigger", &CTriggersManager::GetTrigger)							// Obtiene el trigger del mapa de triggers
			.def("reload", &CTriggersManager::Reload)									// Recarga todos los triggers del XML
	];
}