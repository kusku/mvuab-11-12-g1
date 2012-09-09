#include "Light.h"
#include "Effects\Effect.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "Base.h"
#include "Textures\TextureManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Logger\Logger.h"
#include "Effects\EffectManager.h"
#include "Cameras\Frustum.h"
#include "Lights\LightManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CLight::CLight()
	: m_Color(colWHITE)
	, m_Type(OMNI)
	, m_StartRangeAttenuation(0.0f)
	, m_EndRangeAttenuation(0.0f)
	, m_GenerateDynamicShadowMap(false)
	, m_GenerateStaticShadowMap(false)
	, m_MustUpdateStaticShadowMap(false)
	, m_pStaticShadowMap(NULL)
	, m_pDynamicShadowMap(NULL)
	, m_pShadowMaskTexture(NULL)
	, m_DynamicDepthStencil(NULL)
	, m_StaticDepthStencil(NULL)
	, m_ViewShadowMap(m44fIDENTITY)
	, m_ProjectionShadowMap(m44fIDENTITY)
	, m_LightLinNearFar(0, 0)
	, m_MultiSamples(0)
	, m_LightFrustum(NULL)
{
}

CLight::~CLight()
{
	CHECKED_DELETE(m_LightFrustum);
}

void CLight::BeginRenderEffectManagerShadowMap(CEffect *Effect)
{
}

void CLight::GenerateShadowMap(CRenderManager *RM)
{
	if(!m_GenerateStaticShadowMap && !m_GenerateDynamicShadowMap)
	{
		return;
	}

	SetShadowMap();

	CORE->GetLightManager()->SetCurrentFrustum(m_LightFrustum);

	if( m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap )
	{
		m_pStaticShadowMap->SetAsRenderTarget(0);
		m_StaticDepthStencil->SetAsDepthStencil();

		//RM->ClearTarget(colTRANSPARENT);
		RM->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0 );

		CORE->GetEffectManager()->SetLightShadowLinNearFar(m_LightLinNearFar);

		for(size_t i=0; i<m_StaticShadowMapRenderableObjectsManagers.size(); ++i)
		{
			m_StaticShadowMapRenderableObjectsManagers[i]->Render(RM);
		}

		m_MustUpdateStaticShadowMap = false;
		
		m_pStaticShadowMap->UnsetAsRenderTarget(0);
		m_StaticDepthStencil->UnsetAsDepthStencil();
	}

	if( m_DynamicShadowMapRenderableObjectsManagers.size() > 0)
	{
		m_pDynamicShadowMap->SetAsRenderTarget(0);
		m_DynamicDepthStencil->SetAsDepthStencil();
		
		//RM->ClearTarget(colTRANSPARENT);
		RM->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0 );

		CORE->GetEffectManager()->SetLightShadowLinNearFar(m_LightLinNearFar);

		for(size_t i=0; i<m_DynamicShadowMapRenderableObjectsManagers.size(); ++i)
		{
			m_DynamicShadowMapRenderableObjectsManagers[i]->Render(RM);
		}

		m_pDynamicShadowMap->UnsetAsRenderTarget(0);
		m_DynamicDepthStencil->UnsetAsDepthStencil();
	}

	CORE->GetLightManager()->SetCurrentFrustum(NULL);
}

void CLight::ExtractCommonLightInfo(CXMLTreeNode &XMLNode)
{
	CTextureManager *l_pTextureManager = CORE->GetTextureManager();

	//Common Info por all Lights

	//Name and Position
	m_Name = XMLNode.GetPszProperty("name", "");
	m_Position = XMLNode.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f));

	//Attenuation
	m_StartRangeAttenuation = XMLNode.GetFloatProperty("att_start_range", 0.0f);
	m_EndRangeAttenuation = XMLNode.GetFloatProperty("att_end_range", 0.0f);

	//Color
	Vect3f l_Color = XMLNode.GetVect3fProperty("color", Vect3f(0.0f, 0.0f, 0.0f));
	m_Color = CColor( l_Color.x, l_Color.y, l_Color.z );

	//Intensity
	m_Intensity = XMLNode.GetFloatProperty("intensity", 1.0f);

	//Shadows
	m_GenerateDynamicShadowMap = XMLNode.GetBoolProperty("generate_shadow_map", false, false);
	m_GenerateStaticShadowMap = XMLNode.GetBoolProperty("generate_static_shadow_map", false, false);

	//MultiSamples
	m_MultiSamples = XMLNode.GetBoolProperty("multi_samples", false, false);

	if( m_GenerateDynamicShadowMap )
	{
		std::string l_DynamicType = XMLNode.GetPszProperty("shadow_map_format_type", "");
		uint32 l_WidthDynamicShadowMap = XMLNode.GetIntProperty("shadow_map_width", 512);
		uint32 l_HeightDynamicShadowMap = XMLNode.GetIntProperty("shadow_map_height", 512);

		//Elimina la textura si ya existía
		std::string l_TextureName = m_Name + "_dynamic";
		l_pTextureManager->RemoveResource( l_TextureName );

		m_pDynamicShadowMap = new CTexture();
		CTexture::TFormatType l_Format = m_pDynamicShadowMap->GetFormatTypeFromString(l_DynamicType);
		m_pDynamicShadowMap->Create(l_TextureName, l_WidthDynamicShadowMap, l_HeightDynamicShadowMap, 1, 
			CTexture::RENDERTARGET, CTexture::DEFAULT, l_Format);

		l_pTextureManager->AddResource(m_pDynamicShadowMap->GetName(), m_pDynamicShadowMap);

		//////Depth Stencil

		//Elimina la textura si ya existía
		std::string l_TextureNameDS = m_Name + "_dynamic_ds";
		l_pTextureManager->RemoveResource( l_TextureNameDS );

		m_DynamicDepthStencil = new CTexture();
		m_DynamicDepthStencil->CreateDepthStencil(l_WidthDynamicShadowMap, l_HeightDynamicShadowMap, CTexture::D24S8, static_cast<D3DMULTISAMPLE_TYPE>(m_MultiSamples));
		l_pTextureManager->AddResource(l_TextureNameDS, m_DynamicDepthStencil);
	}

	if( m_GenerateStaticShadowMap )
	{
		std::string l_StaticType = XMLNode.GetPszProperty("static_shadow_map_format_type", "");
		uint32 l_WidthStaticShadowMap = XMLNode.GetIntProperty("static_shadow_map_width", 512);
		uint32 l_HeightStaticShadowMap = XMLNode.GetIntProperty("static_shadow_map_height", 512);

		//Elimina la textura si ya existía
		std::string l_TextureName = m_Name + "_static";
		l_pTextureManager->RemoveResource( l_TextureName );

		m_pStaticShadowMap = new CTexture();
		CTexture::TFormatType l_Format = m_pStaticShadowMap->GetFormatTypeFromString(l_StaticType);
		m_pStaticShadowMap->Create(l_TextureName, l_WidthStaticShadowMap, l_HeightStaticShadowMap, 1, 
			CTexture::RENDERTARGET, CTexture::DEFAULT, l_Format);

		l_pTextureManager->AddResource(m_pStaticShadowMap->GetName(), m_pStaticShadowMap);

		m_MustUpdateStaticShadowMap = true;

		//////Depth Stencil

		//Elimina la textura si ya existía
		std::string l_TextureNameDS = m_Name + "_static_ds";
		l_pTextureManager->RemoveResource( l_TextureNameDS );

		m_StaticDepthStencil = new CTexture();
		m_StaticDepthStencil->CreateDepthStencil(l_WidthStaticShadowMap, l_HeightStaticShadowMap, CTexture::D24S8, static_cast<D3DMULTISAMPLE_TYPE>(m_MultiSamples));
		l_pTextureManager->AddResource(l_TextureNameDS, m_StaticDepthStencil);
	}

	if(m_GenerateDynamicShadowMap || m_GenerateStaticShadowMap)
	{
		m_LightFrustum = new CFrustum();

		uint32 numChild = XMLNode.GetNumChildren();

		for(uint32 i = 0; i < numChild; ++i)
		{
			std::string type = XMLNode(i).GetName();

			if(type == "dynamic")
			{
				std::string layer = XMLNode(i).GetPszProperty("renderable_objects_layer", "");

				if(layer != "")
				{
					CRenderableObjectsLayersManager* roml = CORE->GetRenderableObjectsLayersManager();
					CRenderableObjectsManager* rom = CORE->GetRenderableObjectsLayersManager()->GetResource(layer);

					if(rom != NULL)
					{
						m_DynamicShadowMapRenderableObjectsManagers.push_back(rom);
					}
					else
					{
						std::string msgerr = "CLight::ExtractCommonLightInfo No se encontro Layer de ROM: " + layer;
						LOGGER->AddNewLog(ELL_WARNING, msgerr.c_str());
					}
				}
				else
				{
					std::string msgerr = "CLight::ExtractCommonLightInfo opcion renderable_objects_layer no encontrada";
					LOGGER->AddNewLog(ELL_WARNING, msgerr.c_str());
				}
			}
			else if(type == "static")
			{
				std::string layer = XMLNode(i).GetPszProperty("renderable_objects_layer", "");

				if(layer != "")
				{
					CRenderableObjectsManager* rom = CORE->GetRenderableObjectsLayersManager()->GetResource(layer);

					if(rom != NULL)
					{
						m_StaticShadowMapRenderableObjectsManagers.push_back(rom);
					}
					else
					{
						std::string msgerr = "CLight::ExtractCommonLightInfo No se encontro Layer de ROM: " + layer;
						LOGGER->AddNewLog(ELL_WARNING, msgerr.c_str());
					}
				}
				else
				{
					std::string msgerr = "CLight::ExtractCommonLightInfo opcion renderable_objects_layer no encontrada";
					LOGGER->AddNewLog(ELL_WARNING, msgerr.c_str());
				}
			}
			else if(type != "comment") //If it is anything else than comment
			{
				assert(!"Not a valid suboption CLight::ExtractCommonLightInfo");
			}
		}
	}
}