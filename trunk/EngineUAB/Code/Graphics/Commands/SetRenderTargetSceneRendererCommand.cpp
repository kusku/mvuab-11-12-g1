#include <assert.h>

#include "SetRenderTargetSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &Node)
{
	//<set_render_target name="deferred_multiple_render_target">
	m_Name = Node.GetPszProperty("name", "");

	uint16 l_Count = Node.GetNumChildren();
	for( uint16 i = 0; i < l_Count; ++i )
	{
		std::string l_Type = Node(i).GetName();

		//<dynamic_texture stage_id="0" name="DiffuseMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8"/>
		if( l_Type == "dynamic_texture" )
		{
			CTexture* texture = NULL;
			uint32 stage_id = Node(i).GetIntProperty("stage_id", 0);
			std::string name = Node(i).GetPszProperty("name", "");

			texture = CORE->GetTextureManager()->GetResource(name);

			if(texture == NULL)
			{
				bool frameBuffer = Node(i).GetBoolProperty("texture_width_as_frame_buffer", false);
				std::string format_type = Node(i).GetPszProperty("format_type", "A8R8G8B8");
				uint32 mipmaps = Node(i).GetIntProperty("mipmaps", 1);

					texture = new CTexture();
			
				uint32 width = CORE->GetRenderManager()->GetScreenSize().x;
				uint32 height = CORE->GetRenderManager()->GetScreenSize().y;

				texture->Create(name, width, height, mipmaps, CTexture::RENDERTARGET, CTexture::DEFAULT, texture->GetFormatTypeFromString(format_type));

				CORE->GetTextureManager()->AddResource(texture->GetName(), texture);
			}

			this->AddStageTexture(stage_id, texture);
		}
		else if( l_Type != "comment" ) //If not a complement than not yet supported
		{
			assert(!"Not implemented => CSetRenderTargetSceneRendererCommand");
		}
	}
}

void CSetRenderTargetSceneRendererCommand::Execute(CRenderManager &RM)
{
	this->ActivateTexturesRenderTarget();
}

void CSetRenderTargetSceneRendererCommand::UnsetRenderTaget()
{
	this->DeactivateTexturesRenderTarget();
}