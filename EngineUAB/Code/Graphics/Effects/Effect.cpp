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

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffect::CEffect(CXMLTreeNode *XMLNode)
	: m_Effect(NULL)
	, m_WorldMatrixParameter(NULL)
	, m_ViewMatrixParameter(NULL)
	, m_ProjectionMatrixParameter(NULL)
	, m_WorldViewMatrixParameter(NULL)
	, m_ViewProjectionMatrixParameter(NULL)
	, m_WorldViewProjectionMatrixParameter(NULL)
	, m_ViewToLightProjectionMatrixParameter(NULL)
	, m_ViewInverseMatrixParameter(NULL)
	, m_WorldInverseMatrixParameter(NULL)
	, m_ProjInverseMatrixParameter(NULL)
	, m_LightEnabledParameter(NULL)
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
{
	m_EffectName = XMLNode->GetPszProperty("name", "");
	m_FileName = XMLNode->GetPszProperty("file", "");

	SetLights(MAX_LIGHTS);
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
		NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, NULL, &m_Effect, &l_ErrorBuffer);
	
	if(FAILED(l_HR) || l_ErrorBuffer)
	{
		LPVOID l_ErrPointer = l_ErrorBuffer->GetBufferPointer();
		std::string l_Err = static_cast<char*>( l_ErrPointer );
		std::string msg_error = "CEffect::LoadEffect->" + l_Err;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		CHECKED_RELEASE(l_ErrorBuffer);

		return false;
	}

	//Matrices
	GetParameterBySemantic("PROJECTION", m_ProjectionMatrixParameter);
	GetParameterBySemantic("VIEW", m_ViewMatrixParameter);
	GetParameterBySemantic("WORLD", m_WorldMatrixParameter);
	GetParameterBySemantic("WORLDVIEWPROJECTION", m_WorldViewProjectionMatrixParameter);
	GetParameterBySemantic("VIEWINVERSE", m_ViewInverseMatrixParameter);

	//Camera
	GetParameterBySemantic("CAMERA_POSITION", m_CameraPositionParameter);

	//Lights
	GetParameterBySemantic("Num_Lights", m_NumLightsParameter);
	GetParameterBySemantic("Lights_Enabled", m_LightEnabledParameter);
	GetParameterBySemantic("Lights_Type", m_LightsTypeParameter);
	GetParameterBySemantic("Lights_Position", m_LightsPositionParameter);
	GetParameterBySemantic("Lights_Direction", m_LightsDirectionParameter);
	GetParameterBySemantic("Lights_Color", m_LightsColorParameter);
	GetParameterBySemantic("Lights_StartAtt", m_LightsStartRangeAttenuationParameter);
	GetParameterBySemantic("Lights_EndAtt", m_LightsEndRangeAttenuationParameter);
	GetParameterBySemantic("Lights_Angle", m_LightsAngleParameter);
	GetParameterBySemantic("Lights_FallOff", m_LightsFallOffParameter);

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
	m_WorldMatrixParameter = NULL;
	m_ViewMatrixParameter = NULL;
	m_ProjectionMatrixParameter = NULL;
	m_WorldViewMatrixParameter = NULL;
	m_ViewProjectionMatrixParameter = NULL;
	m_WorldViewProjectionMatrixParameter = NULL;
	m_ViewToLightProjectionMatrixParameter = NULL;
	m_ViewInverseMatrixParameter = NULL;
	m_WorldInverseMatrixParameter = NULL;
	m_ProjInverseMatrixParameter = NULL;
	m_LightEnabledParameter = NULL;
	m_LightsTypeParameter = NULL;
	m_LightsPositionParameter = NULL;
	m_LightsDirectionParameter = NULL;
	m_LightsAngleParameter = NULL;
	m_LightsColorParameter = NULL;
	m_LightsFallOffParameter = NULL;
	m_LightsStartRangeAttenuationParameter = NULL;
	m_LightsEndRangeAttenuationParameter = NULL;
	m_CameraPositionParameter = NULL;
	m_BonesParameter = NULL;
	m_TimeParameter = NULL;
	m_NumLightsParameter = NULL;
}

void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle)
{
	l_Handle=m_Effect->GetParameterBySemantic(NULL,SemanticName.c_str());
	if(l_Handle==NULL)
	{
		std::string msg_error = "CEffect::GetParameterBySemantic->Parámetro por semática " + SemanticName + " no ha encontrado el efecto " + m_FileName;
		LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
	}
}

bool CEffect::SetLights(size_t NumOfLights)
{
	CLightManager *l_Lights = CORE->GetLightManager();
	for(size_t i=0; i<NumOfLights; ++i)
	{
		std::string l_Name = l_Lights->GetLightNameByIndex(static_cast<uint16>(i));
		CLight* l_Light = l_Lights->GetResource(l_Name);

		m_LightsEnabled[i] = l_Light->GetVisible();

		CLight::TLightType l_LightType = l_Light->GetType();
		m_LightsType[i] = static_cast<int>(l_LightType);

		m_LightsStartRangeAttenuation[i] = l_Light->GetStartRangeAttenuation();
		m_LightsEndRangeAttenuation[i] = l_Light->GetEndRangeAttenuation();
		m_LightsPosition[i] = l_Light->GetPosition();

		CColor l_Color = l_Light->GetColor();
		m_LightsColor[i] = Vect3f(l_Color.GetRed(), l_Color.GetGreen(), l_Color.GetBlue());

		if( l_LightType == CLight::DIRECTIONAL )
		{
			CDirectionalLight* l_DirLight = static_cast<CDirectionalLight*>(l_Light);
			m_LightsDirection[i] = l_DirLight->GetDirection();
		}
		else if( l_LightType == CLight::SPOT )
		{
			CSpotLight* l_SpotLight = static_cast<CSpotLight*>(l_Light);
			m_LightsDirection[i] = l_SpotLight->GetDirection();
			m_LightsAngle[i] = l_SpotLight->GetAngle();
			m_LightsFallOff[i] = l_SpotLight->GetFallOff();
		}
	}

	return true;
}