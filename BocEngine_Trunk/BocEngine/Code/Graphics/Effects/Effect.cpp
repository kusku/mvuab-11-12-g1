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
#include "EffectParameter.h"
#include "EffectTechnique.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffect::CEffect(CXMLTreeNode *XMLNode)
	: m_Effect(NULL)
	, m_CurrentTechnique(NULL)
{
	m_EffectName = XMLNode->GetPszProperty("name", "");
	m_FileName = XMLNode->GetPszProperty("file", "");
}

CEffect::~CEffect()
{
	Unload();
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

	HRESULT l_HR;
	
	HRDX(D3DXCreateEffectFromFile(CORE->GetRenderManager()->GetDevice(), m_FileName.c_str(), NULL,	
			NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, NULL, &m_Effect, &l_ErrorBuffer), l_HR);
	
	if(FAILED(l_HR) || l_ErrorBuffer)
	{
		std::string msg_error = "CEffect::LoadEffect-> Error al cargar el efecto " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		CHECKED_RELEASE(l_ErrorBuffer);
		return false;
	}

	if(!LoadTechniquesToMemory())
	{
		std::string msg_error = "CEffect::LoadEffect-> Error al cargar las techniques a memoria " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	if(!LoadParametersToMemory())
	{
		std::string msg_error = "CEffect::LoadEffect-> Error al cargar los parametros a memoria " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	//Set Current Technique
	D3DXHANDLE currentTechnique = m_Effect->GetCurrentTechnique();
	D3DXTECHNIQUE_DESC desc;
	HRDX(m_Effect->GetTechniqueDesc(currentTechnique, &desc), l_HR);
	m_CurrentTechnique = m_TechniquesCollection[desc.Name];

 	return true;
}

bool CEffect::LoadTechniquesToMemory()
{
	if(m_Effect == NULL)
	{
		return false;
	}

	//Reset collection to zero
	m_TechniquesCollection.Destroy();

	D3DXTECHNIQUE_DESC desc;
	D3DXHANDLE handle;
	HRESULT hr = D3D_OK;

	CEffectTechnique* technique = NULL;
	std::string name = "";

	for(int i = 0; i < EFFECT_MAX_NUM_TECHNIQUES; i++)
	{
		handle = m_Effect->GetTechnique(i);

		if(handle == NULL) //Are there any more techniques?
		{
			break;
		}

		HRDX(m_Effect->GetTechniqueDesc(handle, &desc), hr);

		if(FAILED(hr))
		{
			std::string msg_error = "CEffect::LoadTechniquesToMemory-> Error al cargar techniques a memoria";
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			continue;
		}

		technique = new CEffectTechnique(this, handle, desc.Name);

		m_TechniquesCollection.AddResource(technique->GetTechniqueName(), technique);
	}

	return true;
}

bool CEffect::LoadParametersToMemory()
{
	if(m_Effect == NULL)
	{
		return false;
	}

	//Reset collection to zero
	m_ParametersCollection.Destroy();

	D3DXPARAMETER_DESC desc;
	D3DXHANDLE handle;
	HRESULT hr = D3D_OK;

	CEffectParameter* parameter = NULL;
	std::string name = "";
	std::string semantic = "";

	for(int i = 0; i < EFFECT_MAX_NUM_PARAMETERS; i++)
	{
		handle = m_Effect->GetParameter(NULL, i);

		if(handle == NULL) //Are there any more parametes?
		{
			break;
		}

		HRDX(m_Effect->GetParameterDesc(handle, &desc), hr);

		if(FAILED(hr))
		{
			std::string msg_error = "CEffect::LoadParametersToMemory-> Error al cargar parametros a memoria";
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			continue;
		}

		parameter = new CEffectParameter(this, handle, desc.Name, desc.Semantic);

		m_ParametersCollection.AddResource(parameter->GetName(), parameter);
	}

	return true;
}

void CEffect::Unload()
{
	CHECKED_RELEASE( m_Effect );		
}

bool CEffect::Begin(uint32& passes, EFFECT_FLAGS flags)
{
	HRESULT l_hr;

	HRDX(m_Effect->Begin(&passes,  flags), l_hr);

	return (SUCCEEDED(l_hr));
}

bool CEffect::End()
{
	HRESULT l_hr;

	HRDX(m_Effect->End(), l_hr);

	return (SUCCEEDED(l_hr));
}

bool CEffect::BeginPass(uint32 pass)
{
	HRESULT l_hr;

	HRDX(m_Effect->BeginPass(pass), l_hr);

	return (SUCCEEDED(l_hr));
}

bool CEffect::EndPass()
{
	HRESULT l_hr;

	HRDX(m_Effect->EndPass(), l_hr);

	return (SUCCEEDED(l_hr));
}

bool CEffect::SetCurrentTechnique (CEffectTechnique* effectTechnique)
{
	if(m_Effect == NULL)
	{
		return false;
	}

	HRESULT hr = D3D_OK;
	D3DXHANDLE handle = effectTechnique->GetD3DXTechnique();

	HRDX(m_Effect->SetTechnique(handle), hr);

	if(SUCCEEDED(hr))
	{
		m_CurrentTechnique = effectTechnique;
		return true;
	}

	return false;
}

CEffectTechnique* CEffect::GetTechnique( const std::string& TechniqueName )
{
	return m_TechniquesCollection.GetResource(TechniqueName);
}

CEffectParameter* CEffect::GetParameter( const std::string& ParameterName )
{
	return m_ParametersCollection.GetResource(ParameterName);
}

CEffectParameter* CEffect::GetParameterBySemantic( const std::string& SemanticName )
{
	return m_ParametersCollection.GetParameterBySemantic(SemanticName);
}