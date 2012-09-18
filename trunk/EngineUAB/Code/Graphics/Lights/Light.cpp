#include "Light.h"
#include "Effects\Effect.h"
#include "RenderManager.h"
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

	RenderShadows(RM);
}

void CLight::ExtractCommonLightInfo(CXMLTreeNode &XMLNode)
{
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

	if(m_GenerateDynamicShadowMap || m_GenerateStaticShadowMap)
	{
		//MultiSamples
		m_MultiSamples = XMLNode.GetIntProperty("multi_samples", 0);

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
		
		CTexture::TFormatType dynamicFormat = CTexture::A8R8G8B8;
		uint32 l_WidthDynamicShadowMap = 0;
		uint32 l_HeightDynamicShadowMap = 0;

		CTexture::TFormatType staticFormat = CTexture::A8R8G8B8;
		uint32 l_WidthStaticShadowMap = 0;
		uint32 l_HeightStaticShadowMap = 0;

		if (m_GenerateDynamicShadowMap)
		{
			std::string l_DynamicType = XMLNode.GetPszProperty("shadow_map_format_type", "");
			l_WidthDynamicShadowMap = XMLNode.GetIntProperty("shadow_map_width", 512);
			l_HeightDynamicShadowMap = XMLNode.GetIntProperty("shadow_map_height", 512);

			dynamicFormat = m_pDynamicShadowMap->GetFormatTypeFromString(l_DynamicType);
		}


		if(m_GenerateStaticShadowMap)
		{
			std::string l_StaticType = XMLNode.GetPszProperty("static_shadow_map_format_type", "");
			l_WidthStaticShadowMap = XMLNode.GetIntProperty("static_shadow_map_width", 512);
			l_HeightStaticShadowMap = XMLNode.GetIntProperty("static_shadow_map_height", 512);

			staticFormat = m_pStaticShadowMap->GetFormatTypeFromString(l_StaticType);
		}

		CreateShadowTextures(m_GenerateStaticShadowMap, staticFormat, l_WidthStaticShadowMap, l_HeightStaticShadowMap, m_GenerateDynamicShadowMap, dynamicFormat, l_WidthDynamicShadowMap, l_HeightDynamicShadowMap);		
	}
}