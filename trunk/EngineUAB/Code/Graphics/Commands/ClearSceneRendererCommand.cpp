#include "ClearSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &Node)
{
	//<clear_scene color="true" depth="true" stencil="true"/>
	m_Color = Node.GetBoolProperty("color", true);
	m_Stencil = Node.GetBoolProperty("depth", true);
	m_Depth = Node.GetBoolProperty("stencil", true);
}

void CClearSceneRendererCommand::Execute(CRenderManager &RM)
{
	DWORD clearBuffersOpts = 0x0;

	if(m_Color == true)
	{
		clearBuffersOpts |= D3DCLEAR_TARGET;
	}

	if(m_Stencil == true)
	{
		clearBuffersOpts |=  D3DCLEAR_STENCIL;
	}

	if(m_Depth == true)
	{
		clearBuffersOpts |=  D3DCLEAR_ZBUFFER;
	}

#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
	uint32 red		= (uint32) (RM.GetColorDebug().GetRed() * 255);
	uint32 green	= (uint32) (RM.GetColorDebug().GetGreen() * 255);
	uint32 blue		= (uint32) (RM.GetColorDebug().GetBlue()* 255);
#else // Clear the backbuffer to a black color in a Release mode
	uint32 red		= (uint32) (RM.GetColorRelease().GetRed() * 255);
	uint32 green	= (uint32) (RM.GetColorRelease().GetGreen() * 255);
	uint32 blue		= (uint32) (RM.GetColorRelease().GetBlue()* 255);
#endif
	
	RM.GetDevice()->Clear( 0, NULL, clearBuffersOpts, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
}
