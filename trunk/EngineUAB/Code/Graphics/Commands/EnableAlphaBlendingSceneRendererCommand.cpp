#include "EnableAlphaBlendingSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

#if defined( _DEBUG )
	#include "Memory\MemLeaks.h"
#endif //defined(_DEBUG)

CEnableAlphaBlendingSceneRendererCommand::CEnableAlphaBlendingSceneRendererCommand ( CXMLTreeNode &_atts )
	: CSceneRendererCommand ( _atts )
{
}

void CEnableAlphaBlendingSceneRendererCommand::Execute ( CRenderManager &_RM )
{
	_RM.GetDevice()->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	_RM.GetDevice()->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	_RM.GetDevice()->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

