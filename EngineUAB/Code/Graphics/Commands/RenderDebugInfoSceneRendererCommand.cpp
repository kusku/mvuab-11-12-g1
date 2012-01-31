#include "RenderDebugInfoSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Core.h"
#include "Base.h"
#include "Fonts\FontManager.h"
#include "Utils\Timer.h"
#include "InfoRender\DebugRender.h"
#include "InfoRender\LogRender.h"

CRenderDebugInfoSceneRendererCommand::CRenderDebugInfoSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CRenderDebugInfoSceneRendererCommand ::Execute(CRenderManager &RM)
{	
	CFontManager *fontManager = CORE->GetFontManager();
	CORE->GetDebugRender()->Render( &RM, fontManager, CORE->GetTimer() );
	CORE->GetLogRender()->Render( &RM, fontManager );
}
