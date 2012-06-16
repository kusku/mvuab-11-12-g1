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

	for (uint32 i = 0; it != itEnd; ++it)
	{
		CObject3D* instance = it->second;

		if(instance->GetVisible())
		{
			D3DXMATRIX dxMatrix = instance->GetTransform().GetD3DXMatrix();

			memcpy(&m_InstanceVertex[i], &dxMatrix, sizeof(TINSTANCE_VERTEX));

			++i;
			++m_VisibleObjects;
		}
	}

	if(m_VisibleObjects == 0)
	{
		return;
	}

	void* data = NULL;
	m_InstanceBuffer->Lock(0, sizeof(TINSTANCE_VERTEX) * m_VisibleObjects, &data, D3DLOCK_NOOVERWRITE);
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

/*
void CInstanceMeshHW::CreateASEMesh ( const std::string &_Filename, const std::string &_Name  )
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
void CInstanceMeshHW::CrearPhysicMesh ( const std::string &_Name )
{
	CPhysicUserData* l_pPhysicUserDataMesh;
	CPhysicActor*	 l_MeshActor;
	
	CPhysicCookingMesh* l_pCM = CORE->GetPhysicsManager()->GetCookingMesh();
	if ( l_pCM->CreatePhysicMesh ( m_StaticMesh->GetVertexBuffer(), m_StaticMesh->GetFacesBuffer(), _Name ) )
	{
		l_pPhysicUserDataMesh = new CPhysicUserData( GetName()  );
		l_pPhysicUserDataMesh->SetPaint (true);
		l_MeshActor = new CPhysicActor( l_pPhysicUserDataMesh );
		l_MeshActor->AddMeshShape ( l_pCM->GetPhysicMesh(GetName()), m_Position );
		//m_AseMeshActor->CreateBody ( 10.f );
		CORE->GetPhysicsManager()->AddPhysicActor ( l_MeshActor );
	}

	l_pCM					= NULL;
	l_pPhysicUserDataMesh	= NULL;
	l_MeshActor				= NULL;
}*/