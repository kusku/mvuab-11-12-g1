#include "InstanceMesh.h"
#include "StaticMesh.h"
#include "StaticMeshManager.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"
#include "Math\Matrix44.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Lights\LightManager.h"
#include "Utils\PhysXObjManager.h"
#include "Utils\PhysXObjDefs.h"

////----PhysX Includes-------------
#undef min
#undef max
/*#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
#include "NxActor.h"*/
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysicTriggerReport.h"
#include "PhysicCookingMesh.h"
#include "PhysicUserData.h"
////--------------------------------

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName)
{
	SetName(Name);
	m_StaticMesh = CORE->GetStaticMeshManager()->GetResource(CoreName);

#if defined(_DEBUG)
	if( m_StaticMesh == NULL )
	{
		LOGGER->AddNewLog(ELL_WARNING, "CInstanceMesh::CInstanceMesh->No se ha encontrado la instancia a la static mesh: %s", CoreName.c_str() );
	}
#endif
}

CInstanceMesh::CInstanceMesh(CXMLTreeNode &Node)
{
	m_Name = Node.GetPszProperty("name", "");
	std::string l_Core = Node.GetPszProperty("core", "");
	m_StaticMesh = CORE->GetStaticMeshManager()->GetResource(l_Core);

	m_Position	= Node.GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f));
	m_fYaw		= Node.GetFloatProperty("yaw", 0.0f);
	m_fPitch	= Node.GetFloatProperty("pitch", 0.0f);
	m_fRoll		= Node.GetFloatProperty("roll", 0.0f);
	m_Scale		= Node.GetVect3fProperty("scale", Vect3f(1.0f, 1.0f, 1.0f));
	m_Visible	= Node.GetBoolProperty("visible", true, false);

	//TODO: Faltar leer create_physics y physics_type
	//if ( Node.GetBoolProperty("create_physics")  )
	//	CreateASEMesh ( "./Data/Chapter1/Models/" + m_Name +".ASE", m_Name);

	bool		addPhyX		= Node.GetBoolProperty("create_physics", false, false);
	bool		usePhyXObj	= Node.GetBoolProperty("use_physx_obj", false, addPhyX);
	std::string	physicsGroupName	= Node.GetPszProperty("physics_group", "ECG_ESCENE", false);
	std::string	phyXObjName	= Node.GetPszProperty("physx_obj_name", "", usePhyXObj);
	bool		usePhyXObjPos = Node.GetBoolProperty("use_physx_obj_pos", false, usePhyXObj);
	std::string	typePhyX	= Node.GetPszProperty("physics_type", "", addPhyX && !usePhyXObj);
	
	if (addPhyX)
	{
		int physicsGroup = CORE->GetPhysicsManager()->GetCollisionGroup(physicsGroupName);

		CreatePhysics(m_Name, typePhyX, usePhyXObj, phyXObjName, usePhyXObjPos, physicsGroup);
	}
}

CInstanceMesh::~CInstanceMesh()
{
	m_StaticMesh = NULL;
}

void CInstanceMesh::Render(CRenderManager *RM)
{
	if( m_StaticMesh != NULL )
	{
		if(this->GetVisible())
		{
			bool draw = false;

			CFrustum* frus = NULL;

			if(CORE->IsDrawingShadows())
			{
				frus = CORE->GetLightManager()->GetCurrentFrustum();

				if(frus == NULL)
				{
					draw = true;
				}
			}
			else
			{
				frus = CORE->GetRenderManager()->GetFrustum();
			}

			if(frus != NULL)
			{
				D3DXMATRIX WorldMatrix = GetTransform().GetD3DXMatrix();
				TBoundingBox bb = m_StaticMesh->GetBoundingBox();

				draw = frus->BoxVisible(bb, WorldMatrix);
			}

			if(draw)
			{
				//CORE->GetRenderManager()->DrawAABB(m_StaticMesh->GetBoundingBox(), GetTransform());

				CORE->GetEffectManager()->SetWorldMatrix(GetTransform());
				m_StaticMesh->Render(RM);
			}
		}
	}
}

void CInstanceMesh::CreateASEMesh ( const std::string &_Filename, const std::string &_Name  )
{
	CPhysicUserData* l_pPhysicUserDataASEMesh;
	CPhysicActor*	 l_AseMeshActor;
	
	CPhysicCookingMesh* l_pCM = CORE->GetPhysicsManager()->GetCookingMesh();
	if ( l_pCM->CreateMeshFromASE ( _Filename, _Name ) )
	{
		l_pPhysicUserDataASEMesh = new CPhysicUserData( _Name );
		l_pPhysicUserDataASEMesh->SetPaint (true);
		l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );
		l_AseMeshActor->AddMeshShape ( l_pCM->GetPhysicMesh(_Name), Vect3f (0.f, 0.f, 0.f ) );
		//m_AseMeshActor->CreateBody ( 10.f );
		CORE->GetPhysicsManager()->AddPhysicActor ( l_AseMeshActor );
	}

	l_pCM						= NULL;
	l_pPhysicUserDataASEMesh	= NULL;
	l_AseMeshActor				= NULL;
}

// TODO:: Otra manera de meter la física a partir de los ficheros binarios que creamos desde MAX.
void CInstanceMesh::CreatePhysics(const std::string &_Name, const std::string &typePhysic, bool usePXObj, const std::string &pxObjName, bool usePXObjPos, int physicsGroup)
{
	if(usePXObj)
	{
		TPhysXObj* pxObj = CORE->GetPhysXObjManager()->GetResource(pxObjName);

		assert(pxObj);

		if(pxObj->m_Type == PHYSX_OBJ_BOX)
		{
			TPhysXObjBox* pxBox = static_cast<TPhysXObjBox*>(pxObj);

			Vect3f size = pxBox->m_Dimensions / 2;

			CPhysicUserData* l_pPhysicUserDataMesh = new CPhysicUserData( _Name  );

			CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataMesh);
			l_pPhysicUserDataMesh->SetPaint (true);

			Vect3f pos = v3fZERO;
			Vect3f localPos = v3fZERO;
			Vect3f rotationVect = v3fZERO;


			if(usePXObjPos)
			{
				rotationVect.x = mathUtils::Deg2Rad(pxBox->GetPitch());
				rotationVect.y = mathUtils::Deg2Rad(pxBox->GetYaw());
				rotationVect.z = mathUtils::Deg2Rad(pxBox->GetRoll());

				//TBoundingBox bb = m_StaticMesh->GetBoundingBox();
				//localPos = -(bb.m_MinPos + ( (bb.m_MaxPos - bb.m_MinPos) / 2));

				physicsGroup = pxBox->m_Group;

				pos = pxBox->GetPosition();
			}
			else
			{
				rotationVect.x = mathUtils::Deg2Rad(GetPitch());
				rotationVect.y = mathUtils::Deg2Rad(GetYaw());
				rotationVect.z = mathUtils::Deg2Rad(GetRoll());

				Vect3f scale = GetScale();
				//size = size * scale;

				TBoundingBox bb = m_StaticMesh->GetBoundingBox();
				//localPos = -(bb.m_MinPos + ( (bb.m_MaxPos - bb.m_MinPos) / 2));

				//pos = (bb.m_MinPos + ( (bb.m_MaxPos - bb.m_MinPos) / 2));

				pos += m_Position;
			}

			l_MeshActor->AddBoxSphape(size, pos, localPos, rotationVect, NULL, (uint32)physicsGroup);

			CORE->GetPhysicsManager()->AddPhysicActor(l_MeshActor);
		}
	}
	else
	{
		if(typePhysic == "bounding_box")
		{
			CPhysicUserData* l_pPhysicUserDataMesh = new CPhysicUserData( _Name  );
			
			Vect3f rotationVect = v3fZERO;
			
			rotationVect.x = mathUtils::Deg2Rad(GetPitch());
			rotationVect.y = mathUtils::Deg2Rad(GetYaw());
			rotationVect.z = mathUtils::Deg2Rad(GetRoll());

			TBoundingBox bb = m_StaticMesh->GetBoundingBox();

			Vect3f size = (bb.m_MaxPos - bb.m_MinPos);
			size /= 2;

			Vect3f scale = GetScale();
			size = size * scale;

			CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataMesh);
			l_pPhysicUserDataMesh->SetPaint (true);

			l_MeshActor->AddBoxSphape(size, m_Position, Vect3f(0, 0, 0), rotationVect, NULL, (uint32)physicsGroup);

			CORE->GetPhysicsManager()->AddPhysicActor(l_MeshActor);
		}
		else if(typePhysic == "triangle_mesh")
		{
			CPhysicUserData* l_pPhysicUserDataMesh;
			CPhysicActor*	 l_MeshActor;

			std::vector<Vect3f> vtxBuf = m_StaticMesh->GetVertexBuffer();
			D3DXMATRIX worldMat = GetTransform().GetD3DXMatrix();
			UINT size = vtxBuf.size();

			for (UINT i = 0; i < size; ++i)
			{
				D3DXVECTOR3 pos(vtxBuf[i].x, vtxBuf[i].y, vtxBuf[i].z);
				D3DXVECTOR4 newPos(0, 0, 0, 0);

				D3DXVec3Transform(&newPos, &pos, &worldMat);

				vtxBuf[i].x = newPos.x;
				vtxBuf[i].y = newPos.y;
				vtxBuf[i].z = newPos.z;
			}

			CPhysicCookingMesh* l_pCM = CORE->GetPhysicsManager()->GetCookingMesh();
			if ( l_pCM->CreatePhysicMesh( vtxBuf, m_StaticMesh->GetFacesBuffer(), _Name ) )
			{
				l_pPhysicUserDataMesh = new CPhysicUserData( _Name  );
				l_pPhysicUserDataMesh->SetPaint( true );
				l_MeshActor = new CPhysicActor( l_pPhysicUserDataMesh );
				l_MeshActor->AddMeshShape( l_pCM->GetPhysicMesh(_Name), v3fZERO, v3fZERO, NULL, (uint32)physicsGroup);
				//m_AseMeshActor->CreateBody ( 10.f );
				CORE->GetPhysicsManager()->AddPhysicActor( l_MeshActor );
			}

			l_pCM					= NULL;
			l_pPhysicUserDataMesh	= NULL;
			l_MeshActor				= NULL;
		}
	}
}