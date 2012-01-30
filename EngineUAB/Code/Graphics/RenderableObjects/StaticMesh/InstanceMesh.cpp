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

	m_Position = Node.GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f));
	m_fYaw = Node.GetFloatProperty("yaw", 0.0f);
	m_fPitch = Node.GetFloatProperty("pitch", 0.0f);
	m_fRoll = Node.GetFloatProperty("roll", 0.0f);
	m_Scale = Node.GetVect3fProperty("scale", Vect3f(1.0f, 1.0f, 1.0f));
	m_Visible = Node.GetBoolProperty("visible", true);
	//TODO: Faltar leer create_physics y physics_type
}

CInstanceMesh::~CInstanceMesh()
{
	m_StaticMesh = NULL;
}

void CInstanceMesh::Render(CRenderManager *RM)
{
	if( m_StaticMesh != NULL )
	{
		Mat44f mat, rotYaw, rotPitch, rotRoll;
		
		mat.SetIdentity();
		rotYaw.SetIdentity();
		rotPitch.SetIdentity();
		rotRoll.SetIdentity();

		mat.Translate( GetPosition() );
		
		rotPitch.SetRotByAngleX( mathUtils::Deg2Rad<float>(GetPitch()) );
		rotYaw.SetRotByAngleY( mathUtils::Deg2Rad<float>(GetYaw()) );
		rotRoll.SetRotByAngleZ( mathUtils::Deg2Rad<float>(GetRoll()) );
		
		mat = mat * rotYaw * rotPitch * rotRoll;
		
		RM->SetTransform(mat);

		m_StaticMesh->Render(RM);
	}
}

void CInstanceMesh::Render(CRenderManager *RM, CEffectTechnique* technique)
{
	if( m_StaticMesh != NULL )
	{
		CORE->GetEffectManager()->SetWorldMatrix(GetTransform());
		m_StaticMesh->Render(RM, technique);
	}
}