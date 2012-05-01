#include "CaptureFrameBufferSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Base.h"
#include "Core.h"
#include <assert.h>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------	
CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &Node)
	: CStagedTexturedRendererCommand ( Node )
{
	uint16 l_Count = Node.GetNumChildren();
	for(uint16 i=0; i<l_Count; ++i)
	{
		std::string l_Type = Node(i).GetName();
		if( l_Type == "dynamic_texture" )
		{
			int l_StageID = Node(i).GetIntProperty("stage_id", 0);
			std::string l_Name = Node(i).GetPszProperty("name", "");
			bool l_TextureSizeAsFrameBuffer = Node(i).GetBoolProperty("texture_width_as_frame_buffer", true);
			std::string l_FormatType = Node(i).GetPszProperty("format_type", "");
			uint32 l_MipMaps = Node(i).GetIntProperty("mipmaps", 3);

			assert(l_Name != "" );

			CTextureManager *l_TextureManager = CORE->GetTextureManager();
			CTexture *l_Tex = l_TextureManager->GetResource(l_Name);
			if( l_Tex == NULL )
			{
				l_Tex = new CTexture();
				l_Tex->SetName(l_Name);
				
				uint32 width;
				uint32 height;

				if( l_TextureSizeAsFrameBuffer )
				{
					width = CORE->GetRenderManager()->GetScreenSize().x;
					height = CORE->GetRenderManager()->GetScreenSize().y;
				}

				l_Tex->Create(l_Name, width, height, l_MipMaps, CTexture::RENDERTARGET, CTexture::DEFAULT, l_Tex->GetFormatTypeFromString(l_FormatType));

				l_TextureManager->AddResource(l_Name, l_Tex);
				CORE->GetRenderManager()->SetRenderTargetSize( Vect2f(static_cast<float>(width),static_cast<float>(height))  );
			}

			AddStageTexture(l_StageID, l_Tex);
		}
	}
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CCaptureFrameBufferSceneRendererCommand::Execute(CRenderManager &RM)
{
	CaptureFrameBuffer();
}
