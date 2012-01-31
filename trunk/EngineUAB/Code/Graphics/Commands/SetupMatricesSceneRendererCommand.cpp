#include "SetupMatricesSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"

CSetupMatricesSceneRendererCommand::CSetupMatricesSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CSetupMatricesSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.SetupMatrices( CORE->GetCamera() );
}