#include "EnableAlphaBlendingSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include <assert.h>

#if defined( _DEBUG )
	#include "Memory\MemLeaks.h"
#endif //defined(_DEBUG)

CEnableAlphaBlendingSceneRendererCommand::CEnableAlphaBlendingSceneRendererCommand ( CXMLTreeNode &_atts )
	: CSceneRendererCommand ( _atts )
{
	assert(!"Ya no es un comando valido");
}

void CEnableAlphaBlendingSceneRendererCommand::Execute ( CRenderManager &_RM )
{
}

