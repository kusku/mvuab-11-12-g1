#include "SkySphereCommand.h"
#include "XML\XMLTreeNode.h"


#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSkySphereCommand::CSkySphereCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
	m_SkySphere.Load(Node);
}

void CSkySphereCommand::Execute(CRenderManager &RM)
{
	m_SkySphere.Render();
}
