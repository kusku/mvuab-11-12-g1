#include "EndRenderSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------	
CEndRenderSceneRendererCommand::CEndRenderSceneRendererCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CEndRenderSceneRendererCommand::Execute( CRenderManager &_RM )
{
	_RM.GetDevice()->EndScene();
}
