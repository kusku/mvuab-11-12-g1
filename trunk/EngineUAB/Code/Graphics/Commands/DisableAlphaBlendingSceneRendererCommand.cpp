#include "DisableAlphaBlendingSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

#if defined( _DEBUG )
	#include "Memory\MemLeaks.h"
#endif //defined(_DEBUG)


// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------
CDisableAlphaBlendingSceneRendererCommand::CDisableAlphaBlendingSceneRendererCommand ( CXMLTreeNode &_atts )
	: CSceneRendererCommand ( _atts )
{
}

// --------------------------------------------
//			   MÈTODES PRINCIPALS
// --------------------------------------------
void CDisableAlphaBlendingSceneRendererCommand::Execute ( CRenderManager &_RM )
{
	_RM.GetDevice()->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );
	_RM.GetDevice()->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	_RM.GetDevice()->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

