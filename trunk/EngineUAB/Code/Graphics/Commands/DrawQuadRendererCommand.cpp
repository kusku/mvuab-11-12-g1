#include "DrawQuadRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Logger\Logger.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Base.h"
#include "Core.h"
#include <string>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &Node)
{
	bool active = Node.GetBoolProperty("active", false);
	SetActive(active);

	uint16 l_Count = Node.GetNumChildren();
	for(uint16 i=0; i<l_Count; ++i)
	{
		std::string l_Type = Node(i).GetName();
		if( l_Type == "texture" )
		{
			int l_Stage = Node(i).GetIntProperty("stage_id", 0);
			std::string l_Path = Node(i).GetPszProperty("path", "", false);
			std::string l_File = Node(i).GetPszProperty("file", "", false);

			if( l_File == "" && l_Path != "" )
			{
				CTexture *l_Texture = new CTexture();
				bool nerr = l_Texture->Load(l_Path);
				if( nerr )
				{
					AddStageTexture(l_Stage, l_Texture);
					l_Texture = NULL;
				}
				else
				{
					CHECKED_DELETE(l_Texture);
					std::string errmsg = "CDrawQuadRendererCommand::CDrawQuadRendererCommand->No se ha podido crear la textura: " + l_Path;
					LOGGER->AddNewLog(ELL_ERROR, errmsg.c_str());
				}
			}
			else if( l_File != "" && l_Path == "" )
			{
				CTexture *l_Texture = CORE->GetTextureManager()->GetTexture(l_File);
				if( l_Texture != NULL )
				{
					AddStageTexture(l_Stage, l_Texture);
					l_Texture = NULL;
				}
				else
				{
					std::string errmsg = "CDrawQuadRendererCommand::CDrawQuadRendererCommand->No se ha podido encontrar la textura: " + l_File;
					LOGGER->AddNewLog(ELL_ERROR, errmsg.c_str());
				}
			}
			else
			{
				LOGGER->AddNewLog(ELL_ERROR, "CDrawQuadRendererCommand::CDrawQuadRendererCommand->No se encontrado ni path ni nombre de una textura.");
			}

		}
	}
}

void CDrawQuadRendererCommand::Execute(CRenderManager &RM)
{
	
}
