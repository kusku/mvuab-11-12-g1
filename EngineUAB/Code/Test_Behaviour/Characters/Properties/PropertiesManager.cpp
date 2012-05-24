#include "PropertiesManager.h"
//#include "Properties.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"

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
		l_Properties = new CProperties();

	l_Properties->SetName( _Name );
	l_Properties->SetCore( _CoreName );
	l_Properties->SetAnimationInstance( _AnimatedInstance );
	
	int l_Count = l_XMLPropertiesNode.GetNumChildren();
	// Recorremos todas las propiedades y machacamos las encontradas
	for( uint16 i = 0; i < l_Count; ++i )
	{
		std::string l_PropertyField = l_XMLPropertiesNode(i).GetName();
		if( l_PropertyField == "life" )
			l_Properties->SetLife( l_XMLPropertiesNode(i).GetIntKeyword( "life", 0 ) );

		else if( l_PropertyField == "strong" )
			l_Properties->SetStrong( l_XMLPropertiesNode(i).GetIntKeyword( "strong", 0 ) );

		else if( l_PropertyField == "speed" )
			l_Properties->SetSpeed( l_XMLPropertiesNode(i).GetIntKeyword( "speed", 0 ) );

		else if( l_PropertyField == "direction" )
		{
			Vect3f l_Vect = l_XMLPropertiesNode(i).GetVect3fKeyword ("direction");
			l_Properties->SetDirection( l_Vect );
		}
		// Esta és una prueba para cojer elementos vect3f
		else if( l_PropertyField == "RespawnPosition" )
		{
			Vect3f l_Vect = l_XMLPropertiesNode(i).GetVect3fKeyword ("RespawnPosition");
			l_Properties->SetRespawnPosition ( l_Vect );
		}
		else if( l_PropertyField == "Position" )
		{
			Vect3f l_Vect = l_XMLPropertiesNode(i).GetVect3fKeyword ("Position");
			l_Properties->SetPosition( l_Vect );
		}
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
	LOGGER->AddNewLog ( ELL_INFORMATION, "CPropertiesManager::Reload-->Loading default properties Nodes." );

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
		std::string msg_error = "CPropertiesManager::LoadXML --> Error when trying to load a default property : " + l_Type;
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
	LOGGER->AddNewLog ( ELL_INFORMATION, "CPropertiesManager::Reload-->Loading player properties node." );

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
		std::string msg_error = "CPropertiesManager::LoadXML --> Error when trying to load a player properties : " + l_Type;
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
	LOGGER->AddNewLog ( ELL_INFORMATION, "CPropertiesManager::Reload-->Loading enemy properties node." );

	CXMLTreeNode l_MainNode = _Node;

	std::string l_Type = l_MainNode.GetName();		// Les propietats van primer
	if( l_Type == "enemy" )
	{
		std::string l_Name = l_MainNode.GetPszProperty( "name", "", false );
		std::string l_CoreName = l_MainNode.GetPszProperty( "core", "", false );
		std::string l_AnimatedInstance = l_MainNode.GetPszProperty( "animated_instace_name", "", false ); 
	
		/*if ( GetResource ( l_CoreName ) )
		{
			std::string msg_error = "CPropertiesManager::LoadXML --> Error when trying to load a defauld property already exist: " + l_CoreName;
			LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			return false;
		}*/
		
		// devuelvo las propiedades del enemigo
		return LoadProperties ( l_MainNode, l_Name, l_CoreName, l_AnimatedInstance );
	}
	else 
	{
		std::string msg_error = "CPropertiesManager::LoadXML --> Error when trying to load a default property : " + l_Type;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}
}

void CPropertiesManager::CleanUp( void )
{
	Destroy();
}

// ------------------------------------------
//				PROPIETATS 
// ------------------------------------------

