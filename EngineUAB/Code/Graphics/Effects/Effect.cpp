#include "Effect.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffect::CEffect()
	: m_FileName("")
	, m_Effect(NULL)
	, m_WorldMatrixParameter(NULL)
	, m_ViewMatrixParameter(NULL)
	, m_ProjectionMatrixParameter(NULL)
	, m_WorldViewMatrixParameter(NULL)
	, m_ViewProjectionMatrixParameter(NULL)
	, m_WorldViewProjectionMatrixParameter(NULL)
	, m_ViewToLightProjectionMatrixParameter(NULL)
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
{
}


CEffect::~CEffect()
{
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
	return true;
}

void CEffect::Unload()
{
}

D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName)
{
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
}

void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle)
{
}