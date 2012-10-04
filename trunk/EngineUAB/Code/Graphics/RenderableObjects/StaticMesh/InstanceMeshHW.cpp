#include "InstanceMeshHW.h"
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
#include "Vertexs\VertexType.h"
#include "Object3D.h"
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

CInstanceMeshHW::CInstanceMeshHW(const std::string& coreName)
	: m_InstanceBuffer(NULL)
	, m_InstanceVertex(NULL)
	, m_Initialize(false)
	, m_VisibleObjects(0)
{
	m_StaticMesh = CORE->GetStaticMeshManager()->GetResource(coreName);

	assert(m_StaticMesh);
}

CInstanceMeshHW::~CInstanceMeshHW()
{
	m_StaticMesh = NULL;

	ObjectMapIt it = m_ObjectMap.begin();
	ObjectMapIt itEnd = m_ObjectMap.end();

	for (; it != itEnd; ++it)
	{
		CHECKED_DELETE(it->second);
	}

	m_ObjectMap.clear();

	CHECKED_RELEASE(m_InstanceBuffer);

	CHECKED_DELETE_ARRAY(m_InstanceVertex);
}

bool CInstanceMeshHW::AddHWInstance(CXMLTreeNode &Node)
{
	CObject3D* instance = new CObject3D();

	std::string	name		= Node.GetPszProperty("name", "");
	Vect3f		position	= Node.GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f));
	float		yaw			= Node.GetFloatProperty("yaw", 0.0f);
	float		pitch		= Node.GetFloatProperty("pitch", 0.0f);
	float		roll		= Node.GetFloatProperty("roll", 0.0f);
	Vect3f		scale		= Node.GetVect3fProperty("scale", Vect3f(1.0f, 1.0f, 1.0f));
	bool		visible		= Node.GetBoolProperty("visible", true, false);

	bool		addPhyX		= Node.GetBoolProperty("create_physics", false, false);
	bool		usePhyXObj	= Node.GetBoolProperty("use_physx_obj", false, addPhyX);
	std::string	phyXObjName	= Node.GetPszProperty("physx_obj_name", "", usePhyXObj);
	bool		usePhyXObjPos = Node.GetBoolProperty("use_physx_obj_pos", false, usePhyXObj);
	std::string	typePhyX	= Node.GetPszProperty("physics_type", "", addPhyX && !usePhyXObj);
	std::string	physicsGroupName	= Node.GetPszProperty("physics_group", "ECG_ESCENE", false);

	instance->SetName(name);
	instance->SetPosition(position);
	instance->SetYaw(yaw);
	instance->SetPitch(pitch);
	instance->SetRoll(roll);
	instance->SetScale(scale);
	instance->SetVisible(visible);

	if(m_ObjectMap.find(name) != m_ObjectMap.end())
	{
		assert(!"HW Instance Repetido 2 veces");
		
		CHECKED_DELETE(instance);

		return false;
	}

	m_ObjectMap[name] = instance;

	if(addPhyX)
	{
		int physicsGroup = CORE->GetPhysicsManager()->GetCollisionGroup(physicsGroupName);

		CreatePhysics(instance, name, typePhyX, usePhyXObj, phyXObjName, usePhyXObjPos, physicsGroup);
	}

	m_Initialize = false;

	return true;
}

bool CInstanceMeshHW::BuildInstanceBuffer()
{
	m_Initialize = false;

	CHECKED_RELEASE(m_InstanceBuffer);
	CHECKED_DELETE_ARRAY(m_InstanceVertex);

	uint32 size = (uint32)m_ObjectMap.size();
	void* data = NULL;
	
	m_InstanceVertex = new TINSTANCE_VERTEX[size];
	memset(m_InstanceVertex, 0, sizeof(TINSTANCE_VERTEX) * size);

	LPDIRECT3DDEVICE9 Device = CORE->GetRenderManager()->GetDevice();

	UINT sizeBuffer = sizeof(TINSTANCE_VERTEX) * size;
	DWORD usageVB = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;

	HRESULT hr = Device->CreateVertexBuffer(sizeBuffer, usageVB, NULL, D3DPOOL_DEFAULT, &m_InstanceBuffer, NULL);

	if(hr != D3D_OK)
	{
		CHECKED_DELETE_ARRAY(m_InstanceVertex);
		return false;
	}
	
	m_InstanceBuffer->Lock(0, sizeBuffer, &data, 0);
	memcpy(data, m_InstanceVertex, sizeBuffer);
	m_InstanceBuffer->Unlock();

	m_Initialize = true;

	return true;
}

void CInstanceMeshHW::UpdateBuffer()
{
	if(!m_Initialize)
	{
		BuildInstanceBuffer();
	}

	m_VisibleObjects = 0;
	ObjectMapIt it = m_ObjectMap.begin();
	ObjectMapIt itEnd = m_ObjectMap.end();

	TBoundingBox bb = m_StaticMesh->GetBoundingBox();
	bool draw = true;
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

	for (uint32 i = 0; it != itEnd; ++it)
	{
		CObject3D* instance = it->second;

		if(instance->GetVisible())
		{
			D3DXMATRIX WorldMatrix = instance->GetTransform().GetD3DXMatrix();
			
			if(frus != NULL)
			{
				draw = frus->BoxVisible(bb, WorldMatrix);
			}

			if(draw)
			{
				memcpy(&m_InstanceVertex[i], &WorldMatrix, sizeof(TINSTANCE_VERTEX));

				++i;
				++m_VisibleObjects;

				//CORE->GetRenderManager()->DrawAABB(m_StaticMesh->GetBoundingBox(), instance->GetTransform());
			}
		}
	}

	if(m_VisibleObjects == 0)
	{
		return;
	}

	void* data = NULL;
	m_InstanceBuffer->Lock(0, sizeof(TINSTANCE_VERTEX) * m_VisibleObjects, &data, D3DLOCK_DISCARD);
	memcpy(data, m_InstanceVertex, sizeof(TINSTANCE_VERTEX) * m_VisibleObjects);
	m_InstanceBuffer->Unlock();
}

void CInstanceMeshHW::Render(CRenderManager *RM)
{
	if( m_StaticMesh == NULL )
	{
		return;
	}

	if(!m_Initialize)
	{
		BuildInstanceBuffer();
	}

	UpdateBuffer();

	if(m_VisibleObjects == 0)
	{
		return;
	}

	m_StaticMesh->RenderInstance(RM, m_InstanceBuffer, m_VisibleObjects);
}

CObject3D* CInstanceMeshHW::GetInstance(const std::string& name)
{
	if(m_ObjectMap.find(name) == m_ObjectMap.end())
	{
		return NULL;
	}

	return m_ObjectMap[name];
}

void CInstanceMeshHW::CreatePhysics(CObject3D* instance, const std::string &_Name, const std::string &typePhysic, bool usePXObj, const std::string &pxObjName, bool usePXObjPos, int physicsGroup)
{
	assert(instance);

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
				rotationVect.x = mathUtils::Deg2Rad(instance->GetPitch());
				rotationVect.y = mathUtils::Deg2Rad(instance->GetYaw());
				rotationVect.z = mathUtils::Deg2Rad(instance->GetRoll());

				Vect3f scale = instance->GetScale();
				//size = size * scale;

				TBoundingBox bb = m_StaticMesh->GetBoundingBox();
				//localPos = -(bb.m_MinPos + ( (bb.m_MaxPos - bb.m_MinPos) / 2));

				pos = (bb.m_MinPos + ( (bb.m_MaxPos - bb.m_MinPos) / 2));

				pos += instance->GetPosition();
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

			rotationVect.x = mathUtils::Deg2Rad(instance->GetPitch());
			rotationVect.y = mathUtils::Deg2Rad(instance->GetYaw());
			rotationVect.z = mathUtils::Deg2Rad(instance->GetRoll());

			TBoundingBox bb = m_StaticMesh->GetBoundingBox();

			Vect3f size = (bb.m_MaxPos - bb.m_MinPos);
			size /= 2;

			Vect3f scale = instance->GetScale();
			//size = size * scale;

			CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataMesh);
			l_pPhysicUserDataMesh->SetPaint (true);


			l_MeshActor->AddBoxSphape(size, instance->GetPosition(), Vect3f(0, 0, 0), rotationVect, NULL, (uint32)physicsGroup);

			CORE->GetPhysicsManager()->AddPhysicActor(l_MeshActor);
		}
		else if(typePhysic == "triangle_mesh")
		{
			CPhysicUserData* l_pPhysicUserDataMesh;
			CPhysicActor*	 l_MeshActor;

			CPhysicCookingMesh* l_pCM = CORE->GetPhysicsManager()->GetCookingMesh();
			if ( l_pCM->CreatePhysicMesh( m_StaticMesh->GetVertexBuffer(), m_StaticMesh->GetFacesBuffer(), _Name ) )
			{
				l_pPhysicUserDataMesh = new CPhysicUserData( _Name  );
				l_pPhysicUserDataMesh->SetPaint( true );
				l_MeshActor = new CPhysicActor( l_pPhysicUserDataMesh );
				l_MeshActor->AddMeshShape( l_pCM->GetPhysicMesh(_Name), instance->GetPosition(), v3fZERO, NULL, (uint32)physicsGroup);
				//m_AseMeshActor->CreateBody ( 10.f );
				CORE->GetPhysicsManager()->AddPhysicActor( l_MeshActor );
			}

			l_pCM					= NULL;
			l_pPhysicUserDataMesh	= NULL;
			l_MeshActor				= NULL;
		}
	}
}