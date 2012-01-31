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
	RM.GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	RM.GetDevice()->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	RM.GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	RM.GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	RM.GetDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	assert( SUCCEEDED( hr ) );
}
