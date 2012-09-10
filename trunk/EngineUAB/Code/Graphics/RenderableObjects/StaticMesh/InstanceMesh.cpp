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

	bool		addPhyX		= Node.GetBoolProperty("create_physics", true, false);
	std::string	typePhyX	= Node.GetPszProperty("physics_type", "", addPhyX);

	if (addPhyX)
	{
		CrearPhysicMesh(m_Name, typePhyX);
	}
}

CInstanceMesh::~CInstanceMesh()
{
	m_StaticMesh = NULL;
}

//void CInstanceMesh::Render(CRenderManager *RM)
//{
//	if( m_StaticMesh != NULL )
//	{
//		Mat44f mat, rotYaw, rotPitch, rotRoll;
//		
//		mat.SetIdentity();
//		rotYaw.SetIdentity();
//		rotPitch.SetIdentity();
//		rotRoll.SetIdentity();
//
//		mat.Translate( GetPosition() );
//		
//		rotPitch.SetRotByAngleX( mathUtils::Deg2Rad<float>(GetPitch()) );
//		rotYaw.SetRotByAngleY( mathUtils::Deg2Rad<float>(GetYaw()) );
//		rotRoll.SetRotByAngleZ( mathUtils::Deg2Rad<float>(GetRoll()) );
//		
//		mat = mat * rotYaw * rotPitch * rotRoll;
//		
//		RM->SetTransform(mat);
//
//		m_StaticMesh->Render(RM);
//	}
//}

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
void CInstanceMesh::CrearPhysicMesh ( const std::string &_Name, const std::string &typePhysic )
{

	if(typePhysic == "bounding_box")
	{
		CPhysicUserData* l_pPhysicUserDataMesh = new CPhysicUserData( _Name  );

		CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataMesh);
		l_pPhysicUserDataMesh->SetPaint (true);
		TBoundingBox bb = m_StaticMesh->GetBoundingBox();

		Vect3f size = (bb.m_MaxPos - bb.m_MinPos);
		size = Vect3f(abs(size.x), abs(size.y), abs(size.z));
		size /= 4;
		l_MeshActor->AddBoxSphape(size, m_Position, Vect3f(0, size.y * 1.5f, 0));

		//l_MeshActor->CreateBody(1);

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
			l_MeshActor->AddMeshShape( l_pCM->GetPhysicMesh(_Name), m_Position );
			//m_AseMeshActor->CreateBody ( 10.f );
			CORE->GetPhysicsManager()->AddPhysicActor( l_MeshActor );
		}

		l_pCM					= NULL;
		l_pPhysicUserDataMesh	= NULL;
		l_MeshActor				= NULL;
	}
}