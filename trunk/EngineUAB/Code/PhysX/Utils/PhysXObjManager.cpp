#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "PhysXObjManager.h"
#include "PhysicsManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CPhysXObjManager::CPhysXObjManager()
	: m_Filename("")
{

}

CPhysXObjManager::~CPhysXObjManager()
{
	CleanUp();
}

void CPhysXObjManager::CleanUp()
{
	this->Destroy();
}


bool CPhysXObjManager::Load( const std::string& filename )
{
	m_Filename = filename;

	return Reload();
}

bool CPhysXObjManager::Reload()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "CPhysXObjManager::Load->Error al intentar leer el archivo xml: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_xml = newFile["physx_objs"];
	if( l_xml.Exists() )
	{
		uint16 l_Count = l_xml.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_xml(i).GetName();

			if( l_Type == "physx_obj" )
			{
				std::string name = l_xml(i).GetPszProperty("name", "", true);
				int type = l_xml(i).GetIntProperty("type", 0, true);
				Vect3f pos = l_xml(i).GetVect3fProperty("pos", Vect3f(0, 0, 0), true);
				float yaw = l_xml(i).GetFloatProperty("yaw", 0.0f, true);
				float pitch = l_xml(i).GetFloatProperty("pitch", 0.0f, true);
				float roll = l_xml(i).GetFloatProperty("roll", 0.0f, true);
				std::string groupName = l_xml(i).GetPszProperty("group", "ECG_ESCENE", true);

				TPhysXObj* pxObj = NULL;
				
				if(type == PHYSX_OBJ_BOX)
				{
					TPhysXObjBox* pxBox = new TPhysXObjBox();

					pxBox->m_Dimensions = l_xml(i).GetVect3fProperty("dimension", Vect3f(0, 0, 0), true);

					pxObj = pxBox;
				}

				assert(pxObj);

				pxObj->m_Type = type;
				pxObj->m_Group = CORE->GetPhysicsManager()->GetCollisionGroup(groupName);
				pxObj->SetName(name);
				pxObj->SetPosition(pos);
				pxObj->SetYaw(yaw);
				pxObj->SetPitch(pitch);
				pxObj->SetRoll(roll);

				bool isOk = this->AddResource(name, pxObj);
				assert(isOk);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}