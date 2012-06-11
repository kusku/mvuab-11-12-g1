#include "RenderBillboardsSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Billboard\BillboardManager.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------
CRenderBillboardsSceneRendererCommand::CRenderBillboardsSceneRendererCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CRenderBillboardsSceneRendererCommand ::Execute( CRenderManager &_RM )
{	
	CORE->GetBillboardManager()->Render(_RM);
}
