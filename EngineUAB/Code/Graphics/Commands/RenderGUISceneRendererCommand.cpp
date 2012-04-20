#include "RenderGUISceneRendererCommand.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "GUIManager.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &Node)
{
	m_Active = Node.GetBoolProperty("active", false);
}

void CRenderGUISceneRendererCommand ::Execute(CRenderManager &RM)
{	
	if( GetActive() )
	{
		CORE->GetGUIManager()->Render( &RM, CORE->GetFontManager() );
	}
}
