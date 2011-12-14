#include "InstanceMesh.h"
#include "StaticMesh.h"
#include "StaticMeshManager.h"
#include "..\RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Math\Matrix44.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName)
{
	SetName(Name);
	m_StaticMesh = CORE->GetStaticMeshManager()->GetResource(CoreName);
}


CInstanceMesh::~CInstanceMesh()
{
	m_StaticMesh = NULL;
}

void CInstanceMesh::Render(CRenderManager *RM)
{
	Mat44f mat, rotYaw, rotPitch, rotRoll;
	mat.SetIdentity();
	rotYaw.SetIdentity();
	rotPitch.SetIdentity();
	rotRoll.SetIdentity();
	mat.Translate( GetPosition() );
	rotYaw.SetRotByAngleX( GetYaw() );
	rotPitch.SetRotByAngleY( GetPitch() );
	rotRoll.SetRotByAngleZ( GetRoll() );
	mat = mat * rotYaw * rotPitch * rotRoll;
	RM->SetTransform(mat);

	m_StaticMesh->Render(RM);
}