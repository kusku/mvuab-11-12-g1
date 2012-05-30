#include "Effect.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "Lights\DirectionalLight.h"
#include "Lights\OmniLight.h"
#include "Lights\SpotLight.h"
#include "Math\Color.h"
#include "Base.h"
#include "Core.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include <sstream>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffect::CEffect(CXMLTreeNode &XMLNode)
	: m_Effect(NULL)
	, m_WorldMatrixParameter(NULL)
	, m_ViewMatrixParameter(NULL)
	, m_ProjectionMatrixParameter(NULL)
	, m_WorldViewMatrixParameter(NULL)
	, m_ViewProjectionMatrixParameter(NULL)
	, m_WorldViewProjectionMatrixParameter(NULL)
	, m_ShadowViewProjectionMatrixParameter(NULL)
	, m_ShadowWorldViewProjectionMatrixParameter(NULL)
	, m_ShadowCameraPositionParameter(NULL)
	, m_ViewInverseMatrixParameter(NULL)
	, m_WorldInverseMatrixParameter(NULL)
	, m_ProjInverseMatrixParameter(NULL)
	, m_LightsTypeParameter(NULL)
	, m_LightsPositionParameter(NULL)
	, m_LightsDirectionParameter(NULL)
	, m_LightsAngleParameter(NULL)
	, m_LightsColorParameter(NULL)
	, m_LightsFallOffParameter(NULL)
	, m_LightsStartRangeAttenuationParameter(NULL)
	, m_LightsEndRangeAttenuationParameter(NULL)
	, m_CameraPositionParameter(NULL)
	, m_BonesParameter(NULL)
	, m_TimeParameter(NULL)
	, m_NumLightsParameter(NULL)
	, m_ViewProjectionInverseMatrixParameter(NULL)
	, m_HalfPixelParameter(NULL)
	, m_RenderTargetSizeParameter(NULL)
	, m_LightShadowStaticEnableParameter(NULL)
	, m_LightShadowDynamicEnableParameter(NULL)
	, m_ActiveLights(0)
{
	m_EffectName = XMLNode.GetPszProperty("name", "");
	m_FileName = XMLNode.GetPszProperty("file", "");

	for(uint16 i=0; i<MAX_PARAMS_BY_EFFECT; ++i)
	{
		m_Parameters.push_back(NULL);
	}

	memset(m_StaticShadowMapSamplerParameter, 0, sizeof(D3DXHANDLE) * MAX_LIGHTS_BY_SHADER);
	memset(m_DynamicShadowMapSamplerParameter, 0, sizeof(D3DXHANDLE) * MAX_LIGHTS_BY_SHADER);

	memset(m_LightShadowViewProjection, 0, sizeof(Mat44f) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsType, 0, sizeof(int) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsAngle, 0, sizeof(float) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsFallOff, 0, sizeof(float) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsStartRangeAttenuation, 0, sizeof(float) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsEndRangeAttenuation, 0, sizeof(float) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsPosition, 0, sizeof(Vect3f) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsDirection, 0, sizeof(Vect3f) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsColor, 0, sizeof(Vect3f) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsDynamicShadowMap, 0, sizeof(CTexture*) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsStaticShadowMap, 0, sizeof(CTexture*) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsDynamicShadowMapEnable, 0, sizeof(bool) * MAX_LIGHTS_BY_SHADER);
	memset(m_LightsStaticShadowMapEnable, 0, sizeof(bool) * MAX_LIGHTS_BY_SHADER);
}

CEffect::~CEffect()
{
	Unload();
}

bool CEffect::Load()
{
	return LoadEffect();
}

bool CEffect::Load(const std::string &Filename)
{
	m_FileName = Filename;
	return LoadEffect();
}

bool CEffect::Reload()
{
	Unload();
	return LoadEffect();
}

bool CEffect::LoadEffect()
{
	LPD3DXBUFFER l_ErrorBuffer=NULL;
	HRESULT l_HR=D3DXCreateEffectFromFile(CORE->GetRenderManager()->GetDevice(), m_FileName.c_str(), NULL,
		NULL, D3DXSHADER_DEBUG, NULL, &m_Effect, &l_ErrorBuffer);
	
	if(FAILED(l_HR) || l_ErrorBuffer)
	{
		std::string l_Err = "";
		if( l_ErrorBuffer != NULL )
		{
			LPVOID l_ErrPointer = l_ErrorBuffer->GetBufferPointer();
			l_Err = static_cast<char*>( l_ErrPointer );
		}
		else
		{
			l_Err = "Error al cargar el efecto: " + m_FileName;
		}
		
		std::string msg_error = "CEffect::LoadEffect->" + l_Err;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		CHECKED_RELEASE(l_ErrorBuffer);

		return false;
	}

	//Matrices
	GetParameterBySemantic("PROJECTION", m_ProjectionMatrixParameter, false);
	GetParameterBySemantic("VIEW", m_ViewMatrixParameter, false);
	GetParameterBySemantic("WORLD", m_WorldMatrixParameter, false);
	GetParameterBySemantic("WORLDVIEWPROJECTION", m_WorldViewProjectionMatrixParameter, false);
	GetParameterBySemantic("VIEWINVERSE", m_ViewInverseMatrixParameter, false);
	GetParameterBySemantic("PROJECTIONINVERSE", m_ProjInverseMatrixParameter, false);
	GetParameterBySemantic("VIEWPROJECTIONINVERSE", m_ViewProjectionInverseMatrixParameter, false);

	//Camera
	GetParameterBySemantic("CAMERA_POSITION", m_CameraPositionParameter, false);

	//Lights
	GetParameterBySemantic("Num_Lights", m_NumLightsParameter, false);
	GetParameterBySemantic("Lights_Type", m_LightsTypeParameter, false);
	GetParameterBySemantic("Lights_Position", m_LightsPositionParameter, false);
	GetParameterBySemantic("Lights_Direction", m_LightsDirectionParameter, false);
	GetParameterBySemantic("Lights_Color", m_LightsColorParameter, false);
	GetParameterBySemantic("Lights_StartAtt", m_LightsStartRangeAttenuationParameter, false);
	GetParameterBySemantic("Lights_EndAtt", m_LightsEndRangeAttenuationParameter, false);
	GetParameterBySemantic("Lights_Angle", m_LightsAngleParameter, false);
	GetParameterBySemantic("Lights_FallOff", m_LightsFallOffParameter, false);

	//Time
	GetParameterBySemantic("TIME", m_TimeParameter, false);

	//Bones
	GetParameterBySemantic("BONES", m_BonesParameter, false);

	//Shadows
	GetParameterBySemantic("SHADOW_VIEWPROJECTION", m_ShadowViewProjectionMatrixParameter, false);
	GetParameterBySemantic("SHADOW_CAMERA_POSITION", m_ShadowCameraPositionParameter, false);
	GetParameterBySemantic("SHADOW_WORLDVIEWPROJECTION", m_ShadowWorldViewProjectionMatrixParameter, false);
	GetParameterBySemantic("Lights_Shadow_Static_Enable", m_LightShadowStaticEnableParameter, false);
	GetParameterBySemantic("Lights_Shadow_Dynamic_Enable", m_LightShadowDynamicEnableParameter, false);

	GetParameterBySemantic("STATIC_SHADOW_MAP_1", m_StaticShadowMapSamplerParameter[0], false);
	GetParameterBySemantic("STATIC_SHADOW_MAP_2", m_StaticShadowMapSamplerParameter[1], false);
	GetParameterBySemantic("STATIC_SHADOW_MAP_3", m_StaticShadowMapSamplerParameter[2], false);
	GetParameterBySemantic("STATIC_SHADOW_MAP_4", m_StaticShadowMapSamplerParameter[3], false);

	GetParameterBySemantic("DYNAMIC_SHADOW_MAP_1", m_DynamicShadowMapSamplerParameter[0], false);
	GetParameterBySemantic("DYNAMIC_SHADOW_MAP_2", m_DynamicShadowMapSamplerParameter[1], false);
	GetParameterBySemantic("DYNAMIC_SHADOW_MAP_3", m_DynamicShadowMapSamplerParameter[2], false);
	GetParameterBySemantic("DYNAMIC_SHADOW_MAP_4", m_DynamicShadowMapSamplerParameter[3], false);

	//Misc
	GetParameterBySemantic("HALFPIXEL", m_HalfPixelParameter, false);
	GetParameterBySemantic("RENDER_TARGET_SIZE", m_RenderTargetSizeParameter, false);

	//Parameters
	for(uint16 i=0; i<MAX_PARAMS_BY_EFFECT; ++i)
	{
		std::stringstream out;
		out << i;
		std::string l_Name = "PARAMETER" + out.str();
		D3DXHANDLE l_Handle = NULL;
		GetParameterBySemantic(l_Name, l_Handle, false);
		m_Parameters[i] = l_Handle;
	}

 	return true;
}

void CEffect::Unload()
{
	SetNullParameters();
	CHECKED_RELEASE( m_Effect );		
}

D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName)
{
	if( m_Effect != NULL )
	{
		D3DXHANDLE l_EffectTechnique = m_Effect->GetTechniqueByName(TechniqueName.c_str());
		return l_EffectTechnique;
	}

	return NULL;
}

void CEffect::SetNullParameters()
{
	m_WorldMatrixParameter						= NULL;
	m_ViewMatrixParameter						= NULL;
	m_ProjectionMatrixParameter					= NULL;
	m_WorldViewMatrixParameter					= NULL;
	m_ViewProjectionMatrixParameter				= NULL;
	m_WorldViewProjectionMatrixParameter		= NULL;
	m_ShadowViewProjectionMatrixParameter		= NULL;
	m_ShadowCameraPositionParameter				= NULL;
	m_ShadowWorldViewProjectionMatrixParameter	= NULL;
	m_ViewInverseMatrixParameter				= NULL;
	m_WorldInverseMatrixParameter				= NULL;
	m_ProjInverseMatrixParameter				= NULL;
	m_LightsTypeParameter						= NULL;
	m_LightsPositionParameter					= NULL;
	m_LightsDirectionParameter					= NULL;
	m_LightsAngleParameter						= NULL;
	m_LightsColorParameter						= NULL;
	m_LightsFallOffParameter					= NULL;
	m_LightsStartRangeAttenuationParameter		= NULL;
	m_LightsEndRangeAttenuationParameter		= NULL;
	m_CameraPositionParameter					= NULL;
	m_BonesParameter							= NULL;
	m_TimeParameter								= NULL;
	m_NumLightsParameter						= NULL;
	m_ViewProjectionInverseMatrixParameter		= NULL;
	m_HalfPixelParameter						= NULL;
	m_RenderTargetSizeParameter					= NULL;
	m_LightShadowStaticEnableParameter			= NULL;
	m_LightShadowDynamicEnableParameter			= NULL;

	memset(m_StaticShadowMapSamplerParameter, 0, sizeof(D3DXHANDLE) * MAX_LIGHTS_BY_SHADER);
	memset(m_DynamicShadowMapSamplerParameter, 0, sizeof(D3DXHANDLE) * MAX_LIGHTS_BY_SHADER);

	uint16 l_Count = m_Parameters.size();
	for(uint16 i=0; i<l_Count; ++i)
	{
		m_Parameters[i] = NULL;
	}
}

void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle, bool Warning)
{
	l_Handle=m_Effect->GetParameterBySemantic(NULL,SemanticName.c_str());
	if(l_Handle==NULL && Warning)
	{
		std::string msg_error = "CEffect::GetParameterBySemantic->Parámetro por semática " + SemanticName + " no ha encontrado el efecto " + m_FileName;
		LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
	}
}

D3DXHANDLE CEffect::GetParameterById(uint16 id)
{
	if( id < m_Parameters.size() )
	{
		return m_Parameters[id];
	}

	return NULL;
}

bool CEffect::SetLights(size_t NumOfLights)
{
	CLightManager *l_Lights = CORE->GetLightManager();

	uint32 lightCount = 0;
	for(size_t i=0; i<NumOfLights && i < MAX_LIGHTS && i < l_Lights->GetResourcesVector().size(); ++i, ++lightCount)
	{
		CLight* l_Light = l_Lights->GetResourcesVector().at(i);

		if(!l_Light->GetVisible())
		{
			lightCount--;
			continue;
		}

		CLight::TLightType l_LightType = l_Light->GetType();
		m_LightsType[lightCount] = static_cast<int>(l_LightType);

		m_LightsStartRangeAttenuation[lightCount] = l_Light->GetStartRangeAttenuation();
		m_LightsEndRangeAttenuation[lightCount] = l_Light->GetEndRangeAttenuation();
		m_LightsPosition[lightCount] = l_Light->GetPosition();

		CColor l_Color = l_Light->GetColor();
		m_LightsColor[lightCount] = Vect3f(l_Color.GetRed()/255.0f, l_Color.GetGreen()/255.0f, l_Color.GetBlue()/255.0f);

		if( l_LightType == CLight::DIRECTIONAL )
		{
			CDirectionalLight* l_DirLight = static_cast<CDirectionalLight*>(l_Light);
			m_LightsDirection[lightCount] = l_DirLight->GetDirection();
		}
		else if( l_LightType == CLight::SPOT )
		{
			CSpotLight* l_SpotLight = static_cast<CSpotLight*>(l_Light);
			m_LightsDirection[lightCount] = l_SpotLight->GetDirection();
			m_LightsAngle[lightCount] = l_SpotLight->GetAngle();
			m_LightsFallOff[lightCount] = l_SpotLight->GetFallOff();
		}

		m_LightShadowViewProjection[lightCount] = l_Light->GetProjectionShadowMap() * l_Light->GetViewShadowMap();

		if(l_Light->GetGenerateDynamicShadowMap()) 
		{
			m_LightsDynamicShadowMap[lightCount] = l_Light->GetDynamicShadowMap();
			m_LightsDynamicShadowMapEnable[lightCount] = true;
		}
		else
		{
			m_LightsDynamicShadowMap[lightCount] = NULL;
			m_LightsDynamicShadowMapEnable[lightCount] = false;
		}

		if(l_Light->GetGenerateStaticShadowMap())
		{
			m_LightsStaticShadowMap[lightCount] = l_Light->GetStaticShadowMap();
			m_LightsStaticShadowMapEnable[lightCount] = true;
		}
		else
		{
			m_LightsStaticShadowMap[lightCount] = NULL;
			m_LightsStaticShadowMapEnable[lightCount] = false;
		}
	}

	m_ActiveLights = lightCount;

	return true;
}

bool CEffect::SetLight(CLight* light)
{
	if(!light->GetVisible())
	{
		return false;
	}

	//CLight::TLightType l_LightType = CLight::OMNI;
	CLight::TLightType l_LightType = light->GetType();

	m_LightsType[0] = static_cast<int>(l_LightType);

	m_LightsStartRangeAttenuation[0] = light->GetStartRangeAttenuation();
	m_LightsEndRangeAttenuation[0] = light->GetEndRangeAttenuation();
	m_LightsPosition[0] = light->GetPosition();

	CColor l_Color = light->GetColor();
	m_LightsColor[0] = Vect3f(l_Color.GetRed()/255.0f, l_Color.GetGreen()/255.0f, l_Color.GetBlue()/255.0f);

	if( l_LightType == CLight::DIRECTIONAL )
	{
		CDirectionalLight* l_DirLight = static_cast<CDirectionalLight*>(light);
		m_LightsDirection[0] = l_DirLight->GetDirection();
	}
	else if( l_LightType == CLight::SPOT )
	{
		CSpotLight* l_SpotLight = static_cast<CSpotLight*>(light);
		m_LightsDirection[0] = l_SpotLight->GetDirection();
		m_LightsAngle[0] = l_SpotLight->GetAngle();
		m_LightsFallOff[0] = l_SpotLight->GetFallOff();
	}

	m_LightShadowViewProjection[0] = light->GetProjectionShadowMap() * light->GetViewShadowMap();

	if(light->GetGenerateDynamicShadowMap()) 
	{
		m_LightsDynamicShadowMap[0] = light->GetDynamicShadowMap();
		m_LightsDynamicShadowMapEnable[0] = true;
	}
	else
	{
		m_LightsDynamicShadowMap[0] = NULL;
		m_LightsDynamicShadowMapEnable[0] = false;
	}

	if(light->GetGenerateStaticShadowMap())
	{
		m_LightsStaticShadowMap[0] = light->GetStaticShadowMap();
		m_LightsStaticShadowMapEnable[0] = true;
	}
	else
	{
		m_LightsStaticShadowMap[0] = NULL;
		m_LightsStaticShadowMapEnable[0] = false;
	}

	return true;
}