#include "WeaponManager.h"
#include "Types\MeleeWeapon.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "Base.h"
#include "Core.h"
#include <assert.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

//-----------------------------
CWeaponManager::CWeaponManager()
	: m_pCurrentWeapon(NULL)
	, m_Filename("")
{

}

//-----------------------------
CWeaponManager::~CWeaponManager()
{
	CleanUp();
}

//-----------------------------
void CWeaponManager::CleanUp()
{
	Destroy();
	m_pCurrentWeapon = NULL;
}

//-----------------------------
bool CWeaponManager::Load( const std::string & _Filename )
{
	m_Filename = _Filename;
	return LoadFile();
}

//-----------------------------
bool CWeaponManager::Reload()
{
	CleanUp();
	return LoadFile();
}

//-----------------------------
bool CWeaponManager::LoadFile()
{
	/* File Format
	<Weapons>
		<weapon name="hoces" type="melee" left_weapon="melee_left_weapon" right_weapon="melee_right_weapon" right_hand="CHR_CAP R Hand" left_hand="CHR_CAP L Hand" animated_model_instance="caperucita1" layer="solid" layer_attach="solid" />
	</Weapons>
	*/

	CXMLTreeNode newFile;
	if ( !newFile.LoadFile ( m_Filename.c_str ( ) ) )
	{
		std::string msg_error = "CWeaponManager::LoadFile->Error al leer el fichero de armas: " + m_Filename;
		LOGGER->AddNewLog( ELL_ERROR, msg_error.c_str() );
		return false;
	}

	CXMLTreeNode l_RootNode = newFile ["Weapons"];
	if ( l_RootNode.Exists() )
	{
		uint16 l_TotalNodes = l_RootNode.GetNumChildren ();
		for ( uint16 i = 0; i < l_TotalNodes; ++i )
		{
			CXMLTreeNode l_WeaponNode = l_RootNode(i);

			std::string l_Node = l_RootNode(i).GetName();
			if ( l_Node == "weapon" ) 
			{
				std::string l_Name			= l_WeaponNode.GetPszProperty("name","");
				std::string l_TypeName		= l_WeaponNode.GetPszProperty("type", "");
				std::string l_ROLeft		= l_WeaponNode.GetPszProperty("left_weapon", "");
				std::string l_RORight		= l_WeaponNode.GetPszProperty("right_weapon", "");
				std::string l_LeftHand		= l_WeaponNode.GetPszProperty("left_hand", "");
				std::string l_RightHand		= l_WeaponNode.GetPszProperty("right_hand", "");
				std::string l_AMInstance	= l_WeaponNode.GetPszProperty("animated_model_instance", "");
				std::string l_Layer			= l_WeaponNode.GetPszProperty("layer", "");
				std::string l_LayerAttach	= l_WeaponNode.GetPszProperty("layer_attach", "");

				CWeapon *l_pWeapon = NULL;

				//Mira que tipo de arma es y la crea
				EWeaponType l_Type = String2WeaponType(l_TypeName);
				switch( l_Type )
				{
				case MELEE_TYPE:
					l_pWeapon = new CMeleeWeapon();
					break;
				case DISTANCE_TYPE:
					break;
				}

				//Asigna los valores leídos
				if( l_pWeapon != NULL )
				{
					EWeaponHand l_Hand = String2WeaponHands(l_LeftHand, l_RightHand);

					l_pWeapon->SetName(l_Name);
					l_pWeapon->SetWeaponType(l_Type);
					l_pWeapon->SetLeftHandName(l_LeftHand);
					l_pWeapon->SetRightHandName(l_RightHand);
					l_pWeapon->SetHandType(l_Hand);
					
					//busca el modelo de la arma y lo asigna
					CRenderableObjectsManager *l_pROM = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager(l_Layer);

					assert(l_pROM != NULL);
					if( l_Hand ==  LEFT_HAND  || l_Hand == BOTH_HAND )
					{
						CObject3D *l_pObject = l_pROM->GetInstance(l_ROLeft);
						assert(l_pObject != NULL);

						l_pWeapon->SetWeaponLeft(l_pObject);
					}

					if(l_Hand ==  RIGHT_HAND || l_Hand == BOTH_HAND )
					{
						CObject3D *l_pObject = l_pROM->GetInstance(l_RORight);
						assert(l_pObject != NULL);

						l_pWeapon->SetWeaponRight(l_pObject);
					}
					

					//Busca el modelo animado y lo asigna
					l_pROM = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager(l_LayerAttach);

					assert(l_pROM != NULL);
					CAnimatedInstanceModel *l_pAnimated = static_cast<CAnimatedInstanceModel*>(l_pROM->GetInstance(l_AMInstance));

					assert(l_pAnimated != NULL);
					l_pWeapon->SetAnimatedModel(l_pAnimated);

					AddResource(l_Name, l_pWeapon);
				}
				else
				{
					//Rompe la iteración del bucle si el tipo no es correcto
					break;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

//-----------------------------
void CWeaponManager::Update( float _ElapsedTime )
{
	if ( m_pCurrentWeapon != NULL )
	{
		m_pCurrentWeapon->Update(_ElapsedTime);
	}
}

//-----------------------------
void CWeaponManager::ChangeCurrentWeapon( const std::string _Name )
{
	m_pCurrentWeapon = GetResource(_Name);
}

//-----------------------------
void CWeaponManager::ChangeCurrentWeapon( CWeapon *_pWeapon )
{
	m_pCurrentWeapon = _pWeapon;
}

//-----------------------------
EWeaponType CWeaponManager::String2WeaponType( const std::string & _Type )
{
	if ( _Type == "melee" )
	{
		return MELEE_TYPE;
	} 
	else if( _Type == "distance" )
	{
		return DISTANCE_TYPE;
	}

	return NONE_TYPE;
};

//-----------------------------
EWeaponHand CWeaponManager::String2WeaponHands( const std::string & _Left, const std::string & _Right )
{
	if ( _Left != "" && _Right != "" )
	{
		return BOTH_HAND;
	}
	else if( _Left != "" && _Right == "" )
	{
		return LEFT_HAND;
	}
	else if( _Left == "" && _Right != "" )
	{
		return RIGHT_HAND;
	}

	return NONE_HAND;
};