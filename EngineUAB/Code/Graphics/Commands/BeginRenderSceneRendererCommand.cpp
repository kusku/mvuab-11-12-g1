#include <assert.h>

#include "BeginRenderSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CBeginSceneRendererCommand::CBeginSceneRendererCommand(CXMLTreeNode &Node)

{
}

void CBeginSceneRendererCommand::Execute(CRenderManager &RM)
{	
	HRESULT hr = RM.GetDevice()->BeginScene();

	assert( SUCCEEDED( hr ) );
}
