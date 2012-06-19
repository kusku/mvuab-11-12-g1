#include "RenderGUISceneRendererCommand.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "GUIManager.h"
#include "Core.h"
#include "Base.h"
#include "Graphic States\GraphicStates.h"
#include "RenderManager.h"
#include <d3d9.h>
#include <d3dx9.h>

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
		//CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::AlphaBlend);

		CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		CORE->GetGUIManager()->Render( &_RM, CORE->GetFontManager() );
		CORE->GetRenderManager()->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		//CORE->GetRenderManager()->SetGraphicBlendState(TGraphicBlendStates::DefaultState);
	}
}
