#include "SetupMatricesSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSetupMatricesSceneRendererCommand::CSetupMatricesSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
{
}

void CSetupMatricesSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.SetupMatrices( CORE->GetCamera() );
}