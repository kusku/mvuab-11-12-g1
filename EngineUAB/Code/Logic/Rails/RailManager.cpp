#include "RailManager.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

//--------------------------------------------------
CRailManager::CRailManager()
	: m_pCurrentRail(NULL)
	, m_Filename("")
{

}

//--------------------------------------------------
CRailManager::~CRailManager()
{
	CleanUp();
}

//--------------------------------------------------
void CRailManager::CleanUp()
{
	CTemplatedVectorMapManager::Destroy();
}

//--------------------------------------------------
void CRailManager::Update( float _fElapsedTime )
{
	if(m_pCurrentRail != NULL)
	{
		m_pCurrentRail->Update(_fElapsedTime);
	}
}

//--------------------------------------------------
bool CRailManager::Load( const std::string &_Filename )
{
	m_Filename = _Filename;
	return LoadXML();
}

//--------------------------------------------------
bool CRailManager::Reload()
{
	CleanUp();
	return LoadXML();
}

//--------------------------------------------------
bool CRailManager::LoadXML()
{
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_Filename.c_str ( ) ) )
	{
		std::string msg_error = "CRailManager::LoadXML->Error al leer el fichero de raíles: " + m_Filename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	CXMLTreeNode l_RootNode = newFile ["rails"];
	if ( l_RootNode.Exists() )
	{
		uint16 l_TotalNodes = l_RootNode.GetNumChildren();
		for ( uint16 i = 0; i < l_TotalNodes; ++i )
		{
			CXMLTreeNode l_RailNode = l_RootNode(i);

			std::string l_Type = l_RailNode.GetName();
			if ( l_Type == "rail" ) 
			{
				std::string l_Name = l_RailNode.GetPszProperty( "name", "" );

				CRail *l_pRail = new CRail( l_RailNode );
				l_pRail->SetName( l_Name );

				CTemplatedVectorMapManager::AddResource( l_Name, l_pRail );
			}
		}
	}

	return true;
}

//--------------------------------------------------
void CRailManager::SetCurrentRail( const std::string &_Rail )
{
	m_pCurrentRail = GetResource(_Rail);

#ifdef _DEBUG
	if(m_pCurrentRail == NULL)
	{
		LOGGER->AddNewLog(ELL_WARNING, "CRailManager::SetCurrentRail->El raíl %s no se ha encontrado.", _Rail.c_str());
	}
#endif
}

//--------------------------------------------------
void CRailManager::SetObjectToCurrentRail( CObject3D *_Object3D )
{
#ifdef _DEBUG
	assert(m_pCurrentRail);

	if(m_pCurrentRail != NULL)
	{
		m_pCurrentRail->SetObject(_Object3D);
	}

#else
	m_pCurrentRail->SetObject(_Object3D);
#endif
}

//--------------------------------------------------
void CRailManager::StartCurrentRail()
{
#ifdef _DEBUG
	assert( m_pCurrentRail );

	if(m_pCurrentRail != NULL)
	{
		m_pCurrentRail->Start();
	}
#else
	m_pCurrentRail->Start();
#endif
}

//--------------------------------------------------
void CRailManager::PauseCurrentRail()
{
#ifdef _DEBUG
	assert( m_pCurrentRail );

	if(m_pCurrentRail != NULL)
	{
		m_pCurrentRail->Pause();
	}
#else
	m_pCurrentRail->Pause();
#endif
}

//--------------------------------------------------
void CRailManager::StopCurrentRail()
{
#ifdef _DEBUG
	assert( m_pCurrentRail );

	if(m_pCurrentRail != NULL)
	{
		m_pCurrentRail->Stop();
	}
#else
	m_pCurrentRail->Stop();
#endif
}

//--------------------------------------------------
bool CRailManager::HasFinishedCurrentRail()
{
#ifdef _DEBUG
	assert( m_pCurrentRail );

	if(m_pCurrentRail != NULL)
	{
		return m_pCurrentRail->HasFinished();
	}

	return false;
#else
	return m_pCurrentRail->HasFinished();
#endif
}

//--------------------------------------------------
void CRailManager::Render( CRenderManager &_RM )
{
	TVectorResources l_RailVector = GetResourcesVector();
	TVectorResources::iterator l_It = l_RailVector.begin();
	TVectorResources::iterator l_ItEnd = l_RailVector.end();

	for(; l_It != l_ItEnd; ++l_It)
	{
		(*l_It)->Render(_RM);
	}
}