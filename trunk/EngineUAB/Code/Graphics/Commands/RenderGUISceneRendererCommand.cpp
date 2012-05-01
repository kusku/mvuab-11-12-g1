#include "RenderGUISceneRendererCommand.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "GUIManager.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{}


// -----------------------------------------	
//			  MÈTODES PRINCIPALS
// -----------------------------------------

void CRenderGUISceneRendererCommand ::Execute( CRenderManager &_RM )
{	
	if( GetActive() )
	{
		CORE->GetGUIManager()->Render( &_RM, CORE->GetFontManager() );
	}
}
