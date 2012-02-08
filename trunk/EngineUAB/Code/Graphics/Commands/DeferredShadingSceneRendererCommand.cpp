#include <assert.h>
#include <vector>

#include "DeferredShadingSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Textures\TextureManager.h"
#include "Logger\Logger.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Vertexs\VertexType.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &Node)
{
	uint16 l_Count = Node.GetNumChildren();
	for(uint16 i=0; i<l_Count; ++i)
	{
		std::string l_Type = Node(i).GetName();
		if( l_Type == "texture" )
		{
			int			l_Stage			= Node(i).GetIntProperty("stage_id", 0);
			std::string l_File			= Node(i).GetPszProperty("file", "");
			CTexture*	l_Texture		= NULL;

			assert(l_File != "");		
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
		else
		{
			std::string errmsg = "CDrawQuadRendererCommand::CDrawQuadRendererCommand->Comando no encontrado: " + l_Type;
			LOGGER->AddNewLog(ELL_WARNING, errmsg.c_str());
		}
	}
}

void CDeferredShadingSceneRendererCommand::Execute(CRenderManager &RM)
{
	this->ActivateTextures();

	this->SetLightsData(RM);

	std::string technique = CORE->GetROTManager()->GetRenderableObjectTechniqueNameByVertexType(TCOLOREDTEXTURE1_VERTEX::GetVertexType());

	RM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( CORE->GetROTManager()->GetResource(technique)->GetEffectTechnique(), colRED);
}

void CDeferredShadingSceneRendererCommand::SetLightsData(CRenderManager &RM)
{/*
	std::vector<CLight*> lights = CORE->GetLightManager()->GetResourcesVector();
	uint32 numLights = lights.size();

	for(uint32 i = 0; i < numLights; i++)
	{
		lights[i]
	}*/
}
