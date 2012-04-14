#include "ParticlesSystemManager.h"
#include "ParticlesEmitterInstance.h"
#include "ParticlesEmitterCore.h"

#include <string>
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "base.h"
#include "Core.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

CParticlesSystemManager::CParticlesSystemManager( void )
	: m_szFilename	( "" )
{
}

CParticlesSystemManager::~CParticlesSystemManager( void )
{
	Destroy();
}


// -----------------------------------------
//			 MÈTODES PRINCIPALS
// -----------------------------------------

void CParticlesSystemManager::Destroy ( void )
{
	CTemplatedVectorMapManager<CParticlesEmitterInstance>::Destroy ( );
	m_EmittersCores.Destroy();
}

bool CParticlesSystemManager::Load ( const std::string &_Filename )
{
	m_szFilename = _Filename;
	return LoadXML();
}

bool CParticlesSystemManager::Reload ( void )
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CParticleSystemManager::Reload --> Reloading particles system." );
	Destroy();
	return LoadXML();
}

// Actualitzem totes les instancies d'emissors
void CParticlesSystemManager::Update ( float _ElapsedTime )
{
	TVectorResources &l_EmittedVector = GetResourcesVector();
	uint16 l_Count = l_EmittedVector.size();
	
	// Per cada instancia actualitzem totes les seves partícules
	for ( uint16 i = 0; i < l_Count; ++i )
	{
		if ( l_EmittedVector[i]->IsALive() )
			l_EmittedVector[i]->Update( _ElapsedTime );
	}
}

void CParticlesSystemManager::Render ( CRenderManager &_RM )
{
	TVectorResources &l_EmittedVector = GetResourcesVector();
	uint16 l_Count = l_EmittedVector.size();
	for ( uint16 i = 0; i < l_Count; ++i )
	{
		if ( l_EmittedVector[i]->IsALive() )
			l_EmittedVector[i]->Render( _RM );
	}
}

// -----------------------------------------
//				MÈTODES 
// -----------------------------------------

// Carreguem el fitxer d'emissors de partícules
bool CParticlesSystemManager::LoadXML ( void )
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CParticlesSystemManager::LoadXML --> Loading Particles System." );
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_szFilename.c_str() ) )
	{
		std::string msg_error = "CParticlesSystemManager::LoadXML->Error when trying to load the particles objects file: " + m_szFilename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	CXMLTreeNode l_NodePare = newFile ["ParticleManager"];
	if ( l_NodePare.Exists() )
	{
		uint16 l_TotalNodes = l_NodePare.GetNumChildren();
		// Recorro els emissors i les instancies
		for ( uint16 i = 0; i < l_TotalNodes; ++i )
		{
			std::string l_Node = l_NodePare(i).GetName();
			// Carreguem tots els emissors
			if ( l_Node == "ParticleEmitters" ) 
			{
				CXMLTreeNode l_ParticlesEmitterNode = l_NodePare(i);
				uint16 l_TotalParticlesEmitterNodes = l_ParticlesEmitterNode.GetNumChildren();
				for ( uint16 j = 0; j < l_TotalParticlesEmitterNodes; ++j )
				{
					// Per cada emissor CORE que trobi 
					std::string l_EmitterNodeName = l_ParticlesEmitterNode(j).GetName();
					if ( l_EmitterNodeName == "ParticleEmitter" ) 
					{
						CXMLTreeNode l_EmitterNode = l_ParticlesEmitterNode(j);
						std::string l_NomCore = l_EmitterNode.GetPszProperty( "name", "" );

						CParticlesEmitterCore * l_EmitterCore = new CParticlesEmitterCore( l_EmitterNode );
						m_EmittersCores.AddResource( l_NomCore, l_EmitterCore );
						l_EmitterCore = 0;
					}
				}
			}
			// Carreguem instancies dels emissors de partícules
			else if( l_Node == "Instances" )
			{
				CXMLTreeNode l_InstancesNode = l_NodePare(i);
				uint16 l_TotalInstancesNodes = l_InstancesNode.GetNumChildren();
				for ( uint16 j = 0; j < l_TotalInstancesNodes; ++j )
				{
					// Per cada instancia que trobi 
					std::string l_InstanceNode = l_InstancesNode(j).GetName();
					if ( l_InstanceNode == "Instance" ) 
					{
						// Per cada emissor CORE que trobi
						CXMLTreeNode l_EmitterNode = l_InstancesNode(j);
						std::string l_EmitterInstanceName = l_EmitterNode.GetPszProperty( "name", "" );
						
						CParticlesEmitterInstance* l_EmitterInstance = new CParticlesEmitterInstance( l_EmitterNode );
						AddResource( l_EmitterInstanceName, l_EmitterInstance );
						l_EmitterInstance = NULL;
					}
				}
			}
		}
	}
	return true;
}

// -----------------------------------------
//				 PROPIETATS 
// -----------------------------------------


CParticlesEmitterInstance * CParticlesSystemManager::GetParticlesEmitterInstance ( const std::string &_Name )
{
	return GetResource( _Name );
}
	

CParticlesEmitterCore *	CParticlesSystemManager::GetParticlesEmitterCore ( const std::string &_Name )
{
	return m_EmittersCores.GetResource( _Name );
}
	
