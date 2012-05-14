#include "AnimationsStatesManager.h"
#include "AnimationsStates.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"

// ------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------
CAnimationsStatesManager::CAnimationsStatesManager( void )
{
}

CAnimationsStatesManager::~CAnimationsStatesManager( void )
{
	CleanUp();
}

// ------------------------------------------
//			  MÈTODES PRINCIPALS
// ------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
// LoadAnimationsStates : Recibe un nodo con la información de los estados, se recorren y se crea un objeto de estados 
//						que es devuelto
//-----------------------------------------------------------------------------------------------------------------------------
CAnimationsStates* CAnimationsStatesManager::LoadAnimationsStates( const CXMLTreeNode &_Node, const std::string &_Name, const std::string &_CoreName )
{
	CXMLTreeNode l_XMLStatesNode = _Node;
	CAnimationsStates* l_AnimationsStates = NULL;

	// Si pasamos el nombre es un player o enemigo --> metemos valores por defecto
	if ( _Name.compare( "" ) != 0 )
	{
		l_AnimationsStates = GetResource( _CoreName );
	}
	
	// Pero si no hemos asignado nada y las propiedades són nulas creamos un nuevo objeto
	if ( !l_AnimationsStates )
		l_AnimationsStates = new CAnimationsStates();

	l_AnimationsStates->SetCore(_CoreName);

	int l_Count = l_XMLStatesNode.GetNumChildren();
	// Recorremos todas los estados y machacamos las encontradas
	for ( uint16 i = 0; i < l_Count; ++i )
	{
		std::string l_StateType = l_XMLStatesNode(i).GetName();
		if ( l_StateType.compare( "state" ) == 0 )
		{
			std::string l_StateName = l_XMLStatesNode(i).GetPszProperty( "name", "" );
			
			std::vector<uint16>* l_AnimationsVector = new std::vector<uint16>();
			int l_CountAnimations = l_XMLStatesNode(i).GetNumChildren();
			for( int16 j = 0; j < l_CountAnimations; ++j )
			{
				std::string l_Animation = l_XMLStatesNode(i)(j).GetName();
				if( l_Animation.compare( "animation" ) == 0 )
					l_AnimationsVector->push_back( l_XMLStatesNode(i)(j).GetIntProperty( "id", -1, TRUE ) );
				else if ( l_Animation.compare ( "comment" ) != 0 ) 
				{
					std::string msg_error = "CAnimationsStatesManager::LoadAnimationsStates --> Error when trying to load states : " + l_Animation;
					LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
				}
			}
			
			// Si existe el estado lo machacamos con los nuevos valores
			l_AnimationsStates->RemoveResource( l_StateName );
			
			// Si no existe el estado lo añadimos
			l_AnimationsStates->AddResource ( l_StateName, l_AnimationsVector );
		}
		else if ( l_StateType.compare ( "comment" ) != 0 ) 
		{
			std::string msg_error = "CAnimationsStatesManager::LoadAnimationsStates --> Error reading unknow tag when trying to load states : " + l_StateType;
			LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		}
	}
	// Devuelvo las propiedades cargadas o valores por defecto
	return l_AnimationsStates;
}

//-----------------------------------------------------------------------------------------------------------------------------
// LoadDefaultAnimationsStates : Recibe un nodo con "default_animated_states", crea un objeto de estados y lo almacena en el manager.
//						Sus valores serviran para cargar por defecto valores de cualquier instancia de player i enemigos
//-----------------------------------------------------------------------------------------------------------------------------
bool CAnimationsStatesManager::LoadDefaultAnimationsStates( const CXMLTreeNode &_Node )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CAnimationsStatesManager::Reload-->Loading default animations states Nodes." );

	CXMLTreeNode l_MainNode = _Node;

	std::string l_Type = l_MainNode.GetName();		
	if( l_Type == "default_states" )
	{
		std::string l_CoreName = _Node.GetPszProperty("core");
		// Si encuentro ya el corename lo eliminamos y metermos de nuevo las propiedades. Esto permite releer esta única parte...
		if ( GetResource ( l_CoreName ) )
		{
			RemoveResource( l_CoreName );
			/*std::string msg_error = "CAnimationsStatesManager::LoadXML --> Error when trying to load a default property already exist: " + l_CoreName;
			LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			return false;*/
		}
		
		CAnimationsStates* l_DefaultStates = LoadAnimationsStates( _Node, "", l_CoreName );
		// Si ha pillado todos los parámetros correctamente crearemos los estados y los meteremos en el mapa
		if ( l_DefaultStates  )
		{
			// Si ya existen los estados de este core los machacamos
			return AddResource ( l_CoreName, l_DefaultStates );
		}
	}
	else 
	{
		std::string msg_error = "CAnimationsStatesManager::LoadXML --> Error when trying to load a default animations states : " + l_Type;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------------
// LoadPlayerAnimationsStates : Recibe un nodo con "player", crea un objeto de estados y almacena estas segun el XML 
//						devolviendolos para configurar el objeto player
//-----------------------------------------------------------------------------------------------------------------------------
CAnimationsStates* CAnimationsStatesManager::LoadPlayerAnimationsStates( const CXMLTreeNode &_Node )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CAnimationsStatesManager::Reload-->Loading player animations states node." );

	CXMLTreeNode l_MainNode = _Node;
	std::string l_Type = l_MainNode.GetName();		
	if( l_Type == "player" )
	{
		std::string l_Name = _Node.GetPszProperty("name");
		std::string l_CoreName = _Node.GetPszProperty("core");
		/*if ( GetResource ( l_CoreName ) )
		{
			std::string msg_error = "CAnimationsStatesManager::LoadXML --> Error when trying to load a player properties already exist: " + l_CoreName;
			LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			return false;
		}*/

		// devuelvo los estados del player
		return LoadAnimationsStates ( _Node, l_Name, l_CoreName );
	}
	else 
	{
		std::string msg_error = "CAnimationsStatesManager::LoadXML --> Error when trying to load the player states : " + l_Type;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return NULL;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------
// LoadEnemyAnimationsStates : Recibe un nodo con "player", crea un objeto de estados y almacena estas devolviendo las propiedades
//						correspondientes al nodo del jugador
//-----------------------------------------------------------------------------------------------------------------------------
CAnimationsStates* CAnimationsStatesManager::LoadEnemyAnimationsStates( const CXMLTreeNode &_Node )
{
	LOGGER->AddNewLog ( ELL_INFORMATION, "CAnimationsStatesManager::Reload-->Loading enemy animations states node." );

	CXMLTreeNode l_MainNode = _Node;

	std::string l_Type = l_MainNode.GetName();		
	if( l_Type == "enemy" )
	{
		std::string l_Name = l_MainNode.GetPszProperty("name");
		std::string l_CoreName = l_MainNode.GetPszProperty("core");
		/*if ( GetResource ( l_CoreName ) )
		{
			std::string msg_error = "CAnimationsStatesManager::LoadXML --> Error when trying to load a defauld property already exist: " + l_CoreName;
			LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
			return false;
		}*/
		
		// devuelvo los estados del enemigo
		return LoadAnimationsStates ( _Node, l_Name, l_CoreName );
	}
	else 
	{
		std::string msg_error = "CAnimationsStatesManager::LoadXML --> Error when trying to load the default states : " + l_Type;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}
}

void CAnimationsStatesManager::CleanUp( void )
{
	Destroy();
}

// ------------------------------------------
//				PROPIETATS 
// ------------------------------------------

