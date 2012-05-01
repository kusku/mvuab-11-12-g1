#include "RenderDebugSoundsSceneRendererCommand.h"
#include "SoundManager.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"

#if defined( _DEBUG )
	#include "Memory\MemLeaks.h"
#endif //defined(_DEBUG)

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------
CRenderDebugSoundsSceneRendererCommand::CRenderDebugSoundsSceneRendererCommand ( CXMLTreeNode &_atts )
	: CSceneRendererCommand ( _atts )
{
}

// -----------------------------------------	
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CRenderDebugSoundsSceneRendererCommand::Execute ( CRenderManager &_RM )
{
	if( GetActive() )
	{
		CORE->GetSoundManager()->Render( _RM );
	}

}
