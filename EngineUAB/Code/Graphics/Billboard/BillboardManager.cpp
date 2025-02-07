#include "BillboardManager.h"
#include "Billboard.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "base.h"
#include "core.h"
#include "Logger\Logger.h"
#include "Graphic States\GraphicStates.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

CBillboardManager::CBillboardManager( void )
	: m_szFilename	( "" )
{}

CBillboardManager::~CBillboardManager( void )
{
	Destroy();
}


// -----------------------------------------
//			 M�TODES PRINCIPALS
// -----------------------------------------

void CBillboardManager::Destroy ( void )
{
	std::vector<CBillboardAnimation*>::iterator l_It = m_vBillboardAnimationVectorINSTANCES.begin();
	std::vector<CBillboardAnimation*>::iterator l_End = m_vBillboardAnimationVectorINSTANCES.end();
	for ( ; l_It < l_End; ++l_It )
	{
		CHECKED_DELETE(*l_It);
	}
	m_vBillboardAnimationVectorINSTANCES.clear();

	m_vBillboardAnimationVectorCORE.Destroy();
}

bool CBillboardManager::Load ( const std::string &_Filename )
{
	m_szFilename = _Filename;
	return LoadXML();
}

bool CBillboardManager::Reload ( void )
{
	Destroy();
	return LoadXML();
}

void CBillboardManager::Update ( float _ElapsedTime )
{
	std::vector<CBillboardAnimation*>::iterator l_It = m_vBillboardAnimationVectorINSTANCES.begin();
	std::vector<CBillboardAnimation*>::iterator l_End = m_vBillboardAnimationVectorINSTANCES.end();

	for ( ; l_It < l_End; ++l_It )
	{

		(*l_It)->Update( _ElapsedTime );
	}	
}

void CBillboardManager::Render( CRenderManager &_RM )
{
	std::vector<CBillboardAnimation*>::iterator l_It = m_vBillboardAnimationVectorINSTANCES.begin();
	std::vector<CBillboardAnimation*>::iterator l_End = m_vBillboardAnimationVectorINSTANCES.end();

	CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::NonPremultiplied);

	for ( ; l_It < l_End; ++l_It )
	{
		if( (*l_It)->GetVisible() )
		{
			(*l_It)->Render( _RM );
		}
	}

	CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::DefaultState);
}

// -----------------------------------------
//				M�TODES 
// -----------------------------------------

// Carreguem el fitxer de part�cules
bool CBillboardManager::LoadXML()
{
	LOGGER->AddNewLog( ELL_INFORMATION, "CBillboardManager::LoadXML --> Loading Billboards." );
	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_szFilename.c_str ( ) ) )
	{
		std::string msg_error = "CBillboardManager::LoadXML->Error when trying to load the billboard objects file: " + m_szFilename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
		//throw CException(__FILE__, __LINE__, msg_error);
	}

	CXMLTreeNode l_NodePare = newFile ["BillboardAnimation"];
	if ( l_NodePare.Exists ( ) )
	{
		uint16 l_TotalNodes = l_NodePare.GetNumChildren ();
		for ( uint16 i = 0; i < l_TotalNodes; ++i )
		{
			std::string l_Node = l_NodePare(i).GetName();
			// Carreguem totes les animacions
			if ( l_Node == "Billboard" ) 
			{
				CXMLTreeNode l_BillboardNode = l_NodePare(i);
				uint16 l_TotalBillboardNodes = l_BillboardNode.GetNumChildren ();
				for ( uint16 i = 0; i < l_TotalBillboardNodes; ++i )
				{
					// Per cada animaci� que trobi 
					std::string l_AnimationNode = l_BillboardNode(i).GetName();
					if ( l_AnimationNode == "Animation" ) 
					{
						CBillboardAnimation * l_BillboardAnimation = new CBillboardAnimation( l_BillboardNode(i) );
						std::string l_Name = l_BillboardNode(i).GetPszProperty ( "name", "" );
						if ( !m_vBillboardAnimationVectorCORE.AddResource( l_Name, l_BillboardAnimation ) )
						{
							CHECKED_DELETE ( l_BillboardAnimation );
						}
					}
				}
			}
			// Carreguem instancies de les animacions
			else if( l_Node == "Instances" )
			{
				CXMLTreeNode l_InstancesNode = l_NodePare(i);
				uint16 l_TotalInstancesNodes = l_InstancesNode.GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalInstancesNodes; ++j )
				{
					// Per cada instancia que trobi 
					std::string l_InstanceNode = l_InstancesNode(j).GetName();
					if ( l_InstanceNode == "Instance" ) 
					{
						CBillboardAnimation * l_BillboardAnimation = m_vBillboardAnimationVectorCORE.GetResource ( l_InstancesNode(j).GetPszProperty ( "id", "" ) );
						CBillboardAnimation * l_NewInstanceBillboardAnimation = new CBillboardAnimation( l_BillboardAnimation );

						l_NewInstanceBillboardAnimation->SetPosition( static_cast<Vect3f> (l_InstancesNode(j).GetVect3fProperty ( "pos", Vect3f(0.f,0.f,0.f) ) ) ); 
						m_vBillboardAnimationVectorINSTANCES.push_back(l_NewInstanceBillboardAnimation);
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

//inline CBillboardAnimation CBillboardManager::operator = ( const CBillboardAnimation& _pBillboardAnimation ) const
//{
//   return NULL; //CBillboardAnimation ( _pBillboardAnimation );
//}

CBillboardAnimation * CBillboardManager::GetBillboardInstance ( const std::string &_Name )
{
	std::vector<CBillboardAnimation*>::iterator l_It = m_vBillboardAnimationVectorINSTANCES.begin();
	std::vector<CBillboardAnimation*>::iterator l_ItEnd = m_vBillboardAnimationVectorINSTANCES.end();

	for(; l_It != l_ItEnd; ++l_It)
	{
		if( (*l_It)->GetName() == _Name )
		{
			return *l_It;
		}
	}

	return NULL;
}
	