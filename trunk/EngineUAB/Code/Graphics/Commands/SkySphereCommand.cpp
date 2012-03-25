#include "SkySphereCommand.h"
#include "XML\XMLTreeNode.h"
#include "Sky Covers\SkySphere.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSkySphereCommand::CSkySphereCommand(CXMLTreeNode &Node)
{
	m_SkySphere = new CSkySphere(Node);
}

void CSkySphereCommand::Execute(CRenderManager &RM)
{
	m_SkySphere->Render();
}
