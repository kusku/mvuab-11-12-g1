#include "PropertiesManager.h"
#include "VideogameDefs.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "Scripting\ScriptManager.h"
#include "Base.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// ------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------
CPropertiesManager::CPropertiesManager( void )
{
}

CPropertiesManager::~CPropertiesManager( void )
{
	CleanUp();
}

// ------------------------------------------
//			  MÈTODES PRINCIPALS
// ------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
// LoadProperties : Recibe un nodo con la información de propiedades, se recorren y se crea un objeto de propiedades
//						que es devuelto
//-----------------------------------------------------------------------------------------------------------------------------
CProperties* CPropertiesManager::LoadProperties( const CXMLTreeNode &_Node, const std::string &_Name, const std::string &_CoreName, const std::string &_AnimatedInstance )
{
	CXMLTreeNode l_XMLPropertiesNode = _Node;
	CProperties* l_Properties = NULL;

	// Si pasamos el nombre es un player o enemigo --> metemos valores por defecto
	if ( _Name.compare( "" ) != 0 )
	{
		l_Properties = GetResource( _CoreName );
	}
	
	// Pero si no hemos asignado nada y las propiedades són nulas creamos un nuevo objeto
	if ( !l_Properties )
	{
		l_Properties = new CProperties();
	}

	l_Properties->SetName( _Name );
	l_Properties->SetCore( _CoreName );
	l_Properties->SetAnimationInstance( _AnimatedInstance );
	
	int l_Count = l_XMLPropertiesNode.GetNumChildren();
	// Recorremos todas las propiedades y machacamos las encontradas
	for( uint16 i = 0; i < l_Count; ++i )
	{
		std::string l_PropertyField = l_XMLPropertiesNode(i).GetName();
		
		if( l_PropertyField == "active" )
		{
			l_Properties->SetActive( l_XMLPropertiesNode(i).GetBoolKeyword("active") );
		}
		else if ( l_PropertyField == "life" )
			l_Properties->SetLife( l_XMLPropertiesNode(i).GetIntKeyword( "life", 0 ) );

		else if( l_PropertyField == "strong" )
			l_Properties->SetStrong( l_XMLPropertiesNode(i).GetIntKeyword( "strong", 0 ) );

		else if( l_PropertyField == "mass" )
		{
			l_Properties->SetMass( l_XMLPropertiesNode(i).GetFloatKeyword("mass", 1.f) );
		}
		else if( l_PropertyField == "respawn_position" )
		{
			Vect3f l_Vect = l_XMLPropertiesNode(i).GetVect3fKeyword ("respawn_position");
			l_Properties->SetRespawnPosition ( l_Vect );
		}
		else if( l_PropertyField == "position" )
		{
			Vect3f l_Vect = l_XMLPropertiesNode(i).GetVect3fKeyword ("position");
			l_Properties->SetPosition( l_Vect );
		}

		// --- Propiedades de distancias --- // 
		else if( l_PropertyField == "detection_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("detection_distance");
			l_Properties->SetDetectionDistance( l_distance );
		}
		else if( l_PropertyField == "chase_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("chase_distance");
			l_Properties->SetChaseDistance( l_distance );
		}
		else if( l_PropertyField == "follow_path_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("follow_path_distance");
			l_Properties->SetFollowPathDistance( l_distance );
		}
		else if( l_PropertyField == "tired_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("tired_distance");
			l_Properties->SetTiredDistance( l_distance );
		}
		else if( l_PropertyField == "panic_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("panic_distance");
			l_Properties->SetPanicDistance( l_distance );
		}
		else if( l_PropertyField == "attack_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("attack_distance");
			l_Properties->SetAttackDistance( l_distance );
		}
		else if( l_PropertyField == "impact_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("impact_distance");
			l_Properties->SetImpactDistance( l_distance );
		}
		else if( l_PropertyField == "aproximation_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("aproximation_distance");
			l_Properties->SetAproximationDistance( l_distance );
		}
		else if( l_PropertyField == "prepared_to_attack_distance" )
		{
			float l_distance = l_XMLPropertiesNode(i).GetFloatKeyword ("prepared_to_attack_distance");
			l_Properties->SetPreparedAttackDistance( l_distance );
		}
		else if( l_PropertyField == "height_controller" )
		{
			l_Properties->SetHeightController( l_XMLPropertiesNode(i).GetFloatKeyword("height_controller", 1.0f) );
		}
		else if( l_PropertyField == "width_controller" )
		{
			l_Properties->SetWidthController( l_XMLPropertiesNode(i).GetFloatKeyword("width_controller", 1.0f) );
		}
		else if( l_PropertyField == "slope" )
		{
			l_Properties->SetSlopeController( l_XMLPropertiesNode(i).GetFloatKeyword("slope", 45.0f) );
		}
		else if( l_PropertyField == "skin_width" )
		{
			l_Properties->SetSkinWidthController( l_XMLPropertiesNode(i).GetFloatKeyword("skin_width", 0.1f) );
		}
		else if( l_PropertyField == "step_offset" )
		{
			l_Properties->SetStepOffsetController( l_XMLPropertiesNode(i).GetFloatKeyword("step_offset", 0.5f) );
		}
		else if( l_PropertyField == "animation_offset" )
		{
			l_Properties->SetAnimationOffset( l_XMLPropertiesNode(i).GetFloatKeyword("animation_offset", 0.5f) );
		}

		// --- Propiedades de IA --- //
		else if( l_PropertyField == "bounding_radius" )
		{
			l_Properties->SetBoundingRadious( l_XMLPropertiesNode(i).GetFloatKeyword("bounding_radius", 10.f) );
		}
		else if( l_PropertyField == "velocity" )
		{
			Vect3f l_Vect = l_XMLPropertiesNode(i).GetVect3fKeyword ("velocity", Vect3f(0.f,0.f,0.f));
			l_Properties->SetVelocity( l_Vect );
		}
		else if( l_PropertyField == "speed" )
		{
			l_Properties->SetSpeed( l_XMLPropertiesNode(i).GetFloatKeyword ("speed", 1.f ) );
		}
		else if( l_PropertyField == "max_rotation_speed" )
		{
			l_Properties->SetMaxRotationSpeed( l_XMLPropertiesNode(i).GetFloatKeyword( "max_rotation_speed", ROTACION_MAXIMA ) );
		}
		else if( l_PropertyField == "min_speed" )
		{
			l_Properties->SetMinSpeed( l_XMLPropertiesNode(i).GetFloatKeyword("min_speed", VELOCIDAD_MINIMA) );
		}
		else if( l_PropertyField == "max_speed" )
		{
			l_Properties->SetMaxSpeed( l_XMLPropertiesNode(i).GetFloatKeyword("max_speed", VELOCIDAD_MAXIMA) );
		}
		else if( l_PropertyField == "max_acceleration" )
		{
			l_Properties->SetMaxAcceleration( l_XMLPropertiesNode(i).GetFloatKeyword("max_acceleration", ACELERACION_MAXIMA) );
		}
		else if( l_PropertyField == "max_force" )
		{
			l_Properties->SetMaxForce( l_XMLPropertiesNode(i).GetFloatKeyword("max_force", FUERZA_MAXIMA) );
		}
		// --- Fi temas de IA --- //
		else if ( l_PropertyField != "comment" ) 
		{
			std::string msg_error = "CPropertiesManager::LoadXML --> Error reading a unknow tag when trying to load properties : " + l_PropertyField;
			LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		}
	}
	// Devuelvo las propiedades cargadas o valores por defecto
	return l_Properties;
}

//-----------------------------------------------------------------------------------------------------------------------------
// LoadDefaultProperties : Recibe un nodo con "default_properties", crea un objeto de propiedades y lo almacena en el manager.
//						Sus valores serviran para cargar por defecto valores de cualquier instancia de player i enemigos
//-----------------------------------------------------------------------------------------------------------------------------
bool CPropertiesManager::LoadDefaultProperties( const CXMLTreeNode &_Node )
{
	//LOGGER->AddNewLog ( ELL_INFORMATION, "CPropertiesManager::LoadDefaultProperties-->Loading default properties Nodes." );

	CXMLTreeNode l_MainNode = _Node;

	std::string l_Type = l_MainNode.GetName();		// Les propietats van primer
	if( l_Type == "default_properties" )
	{
		std::string l_CoreName = _Node.GetPszProperty("core");
		// Si encuentro ya el corename lo eliminamos y metermos de nuevo las propiedades. Esto permite releer esta única parte...
		if ( GetResource ( l_CoreName ) )
		{
			RemoveResource( l_CoreName );
			/*std::string msg_error = "CPropertiesManager::LoadXML --> Error when trying to load a default property already exist: " + l_CoreName;
			LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			return false;*/
		}
		
		CProperties* l_DefaultProperties = LoadProperties( _Node, "", l_CoreName );
		// Si ha pillado todos los parámetros correctamente crearemos las propietades y las meteremos en el mapa
		if ( l_DefaultProperties  )
		{
			// Si ya existen las propiedades de este core las machacamos
			return AddResource ( l_CoreName, l_DefaultProperties );
		}
	}
	else 
	{
		std::string msg_error = "CPropertiesManager::LoadDefaultProperties --> Error when trying to load a default property : " + l_Type;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------------
// LoadPlayerProperties : Recibe un nodo con "player", crea un objeto de propiedades y almacena estas segun el XML devolviendolas
//						al para configurar el objeto player
//-----------------------------------------------------------------------------------------------------------------------------
CProperties* CPropertiesManager::LoadPlayerProperties( const CXMLTreeNode &_Node )
{
	//LOGGER->AddNewLog ( ELL_INFORMATION, "CPropertiesManager::LoadPlayerProperties-->Loading player properties node." );

	CXMLTreeNode l_MainNode = _Node;
	std::string l_Type = l_MainNode.GetName();		// Les propietats van primer
	if( l_Type == "player" )
	{
		std::string l_Name = l_MainNode.GetPszProperty( "name", "", false );
		std::string l_CoreName = l_MainNode.GetPszProperty( "core", "", false );
		std::string l_AnimatedInstance = l_MainNode.GetPszProperty( "animated_instace_name", "", false ); 
	
		/*if ( GetResource ( l_CoreName ) )
		{
			std::string msg_error = "CPropertiesManager::LoadXML --> Error when trying to load a player properties already exist: " + l_CoreName;
			LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			return false;
		}*/

		// devuelvo las propiedades del player
		return LoadProperties ( l_MainNode, l_Name, l_CoreName, l_AnimatedInstance );
	}
	else 
	{
		std::string msg_error = "CPropertiesManager::LoadPlayerProperties --> Error when trying to load a player properties : " + l_Type;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return NULL;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------
// LoadEnemyProperties : Recibe un nodo con "player", crea un objeto de propiedades y almacena estas devolviendo las propiedades
//						correspondientes al nodo del jugador
//-----------------------------------------------------------------------------------------------------------------------------
CProperties* CPropertiesManager::LoadEnemyProperties( const CXMLTreeNode &_Node )
{
	//LOGGER->AddNewLog( ELL_INFORMATION, "CPropertiesManager::LoadEnemyProperties-->Loading enemy properties node." );

	CXMLTreeNode l_MainNode = _Node;

	std::string l_Type = l_MainNode.GetName();		// Les propietats van primer
	if( l_Type == "enemy" )
	{
		std::string l_Name = l_MainNode.GetPszProperty( "name", "", false );
		std::string l_CoreName = l_MainNode.GetPszProperty( "core", "", false );
		std::string l_AnimatedInstance = l_MainNode.GetPszProperty( "animated_instace_name", "", false ); 
	
		// devuelvo las propiedades del enemigo
		return LoadProperties( l_MainNode, l_Name, l_CoreName, l_AnimatedInstance );
	}

	std::string msg_error = "CPropertiesManager::LoadEnemyProperties --> Error when trying to load a default property : " + l_Type;
	LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );

	return NULL;
}

void CPropertiesManager::CleanUp( void )
{
	Destroy();
}

// ------------------------------------------
//				PROPIETATS 
// ------------------------------------------

