#include "DisableAlphaBlendingSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include <assert.h>

#if defined( _DEBUG )
	#include "Memory\MemLeaks.h"
#endif //defined(_DEBUG)


// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------
CDisableAlphaBlendingSceneRendererCommand::CDisableAlphaBlendingSceneRendererCommand ( CXMLTreeNode &_atts )
	: CSceneRendererCommand ( _atts )
{
	assert(!"Ya no es un comando valido");
}

// --------------------------------------------
//			   MÈTODES PRINCIPALS
// --------------------------------------------
void CDisableAlphaBlendingSceneRendererCommand::Execute ( CRenderManager &_RM )
{
}

