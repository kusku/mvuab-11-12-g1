
#include <assert.h>
#include <string>

#include "DrawQuadRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Logger\Logger.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Base.h"
#include "Core.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Vertexs\VertexType.h"
#include "Math\Matrix44.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &Node)
{
	uint16 l_Count = Node.GetNumChildren();
	for(uint16 i=0; i<l_Count; ++i)
	{
		std::string l_Type = Node(i).GetName();
		if( l_Type == "texture" )
		{
			int			l_Stage			= Node(i).GetIntProperty("stage_id", 0);
			std::string l_File			= Node(i).GetPszProperty("file", "");
			bool		l_LoadFromFile	= Node(i).GetBoolProperty("load_file", false, false);
			CTexture*	l_Texture		= NULL;

			assert(l_File != "");			

			if( l_LoadFromFile )
			{
				l_Texture = CORE->GetTextureManager()->GetTexture(l_File);

				if( l_Texture )
				{
					AddStageTexture(l_Stage, l_Texture);
				}
				else
				{
					std::string errmsg = "CDrawQuadRendererCommand::CDrawQuadRendererCommand->No se ha podido crear la textura: " + l_File;
					LOGGER->AddNewLog(ELL_ERROR, errmsg.c_str());
				}
			}
			else
			{
				l_Texture = CORE->GetTextureManager()->GetResource(l_File);

				if( l_Texture )
				{
					AddStageTexture(l_Stage, l_Texture);
				}
				else
				{
					std::string errmsg = "CDrawQuadRendererCommand::CDrawQuadRendererCommand->No se ha encontrado la textura: " + l_File;
					LOGGER->AddNewLog(ELL_ERROR, errmsg.c_str());
				}
			}
		}
		else
		{
			std::string errmsg = "CDrawQuadRendererCommand::CDrawQuadRendererCommand->Comando no encontrado: " + l_Type;
			LOGGER->AddNewLog(ELL_WARNING, errmsg.c_str());
		}
	}
}

void CDrawQuadRendererCommand::Execute(CRenderManager &RM)
{
	this->ActivateTextures();

	std::string technique = CORE->GetROTManager()->GetRenderableObjectTechniqueNameByVertexType(TCOLOREDTEXTURE1_VERTEX::GetVertexType());

	RM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( CORE->GetROTManager()->GetResource(technique)->GetEffectTechnique(), colRED);													
}
