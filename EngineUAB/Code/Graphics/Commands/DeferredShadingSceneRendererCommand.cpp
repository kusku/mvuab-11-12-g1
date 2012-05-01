#include <assert.h>
#include <vector>

#include "DeferredShadingSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Logger\Logger.h"
#include "Base.h"
#include "Core.h"
#include "Textures\TextureManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"	
#include "Vertexs\VertexType.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand( CXMLTreeNode &Node )
	: CStagedTexturedRendererCommand ( Node )
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
	
	std::string l_Technique = CORE->GetROTManager()->GetRenderableObjectTechniqueNameByVertexType(TCOLOREDTEXTURE1_VERTEX::GetVertexType());

	m_pRenderableObjectTechnique = CORE->GetROTManager()->GetResource(l_Technique);
}

// --------------------------------------------
//			   MÈTODES PRINCIPALS
// --------------------------------------------

void CDeferredShadingSceneRendererCommand::Execute( CRenderManager &_RM )
{
	this->ActivateTextures();

	this->SetLightsData( _RM );
}

void CDeferredShadingSceneRendererCommand::SetLightsData( CRenderManager &_RM )
{
	std::vector<CLight*> lights = CORE->GetLightManager()->GetResourcesVector();
	uint32 numLights = lights.size();

	for ( uint32 i = 0; i < numLights; i++ )
	{
		CEffectTechnique* technique = m_pRenderableObjectTechnique->GetEffectTechnique();
		CEffect* effect = technique->GetEffect();
		
		if ( effect->SetLight(lights[i]) )
		{ 
			lights[i]->SetShadowMap();
			//RM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique(m_pRenderableObjectTechnique->GetEffectTechnique(), colRED);
			_RM.DrawQuad2DTexturedInPixelsInFullScreen( m_pRenderableObjectTechnique->GetEffectTechnique() );
		}
	}
}
