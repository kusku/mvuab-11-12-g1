#include <assert.h>

#include "BeginRenderSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------	
CBeginSceneRendererCommand::CBeginSceneRendererCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CBeginSceneRendererCommand::Execute( CRenderManager &_RM )
{	
	HRESULT l_HR = _RM.GetDevice()->BeginScene();
	_RM.GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	_RM.GetDevice()->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	_RM.GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	_RM.GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	_RM.GetDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	assert( SUCCEEDED( l_HR ) );
}
