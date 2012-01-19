#include "AmbientLightMaterial.h"
#include "Effects\EffectParameter.h"
#include "Base.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


CAmbientLightMaterial::CAmbientLightMaterial( CXMLTreeNode *XMLNode )
	: CEffect(XMLNode) 
	, m_AmbientLightColor(colWHITE)
	, m_AmbientLightIntensity(1.0f)
	, m_WorldViewProjectionMatrix(Mat44f().SetIdentity())
	, m_AmbientLightColorParameter(NULL)
	, m_AmbientLightIntensityParameter(NULL)
	, m_WorldViewProjectionMatrixParameter(NULL)
{
}

CAmbientLightMaterial::~CAmbientLightMaterial()
{
}

bool CAmbientLightMaterial::LoadEffect()
{
	//Load the effect with the parent Methods
	if(!CEffect::LoadEffect())
	{
		return false;
	}

	//Load our shader specific parameters to memory
	m_AmbientLightColorParameter = m_ParametersCollection.GetResource("AmbientLightColor");
	if(m_AmbientLightColorParameter == NULL)
	{
		std::string msg_error = "CAmbientLightMaterial::LoadEffect-> Error al cargar el parametro m_AmbientLightColorParameter";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}
	
	m_AmbientLightIntensityParameter = m_ParametersCollection.GetResource("AmbientLightIntensity");
	if(m_AmbientLightIntensityParameter == NULL)
	{
		std::string msg_error = "CAmbientLightMaterial::LoadEffect-> Error al cargar el parametro m_AmbientLightIntensityParameter";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	m_WorldViewProjectionMatrixParameter = m_ParametersCollection.GetParameterBySemantic("WORLDVIEWPROJECTION");
	if(m_WorldViewProjectionMatrixParameter == NULL)
	{
		std::string msg_error = "CAmbientLightMaterial::LoadEffect-> Error al cargar el parametro m_WorldViewProjectionMatrixParameter";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	//Set our defaults to the shader
	SetAmbientLightColor(m_AmbientLightColor);
	SetAmbientLightIntensity(m_AmbientLightIntensity);
	SetWorldViewProjectionMatrix(m_WorldViewProjectionMatrix);

	return true;
}

bool CAmbientLightMaterial::SetAmbientLightColor(CColor color)
{
	if(m_AmbientLightColorParameter == NULL)
	{
		return false;
	}

	if(m_AmbientLightColorParameter->SetColor(color))
	{
		m_AmbientLightColor = color;
		return true;
	}

	return false;
}

bool CAmbientLightMaterial::SetAmbientLightIntensity(float ambientLightIntensity)
{
	if(m_AmbientLightIntensityParameter == NULL)
	{
		return false;
	}

	if(m_AmbientLightIntensityParameter->SetFloat(ambientLightIntensity))
	{
		m_AmbientLightIntensity = ambientLightIntensity;
		return true;
	}

	return false;
}

bool CAmbientLightMaterial::SetWorldViewProjectionMatrix(Mat44f worldViewProjectionMatrix)
{
	if(m_WorldViewProjectionMatrixParameter == NULL)
	{
		return false;
	}

	if(m_WorldViewProjectionMatrixParameter->SetMatrix(worldViewProjectionMatrix))
	{
		m_WorldViewProjectionMatrix = worldViewProjectionMatrix;
		return true;
	}

	return false;
}
