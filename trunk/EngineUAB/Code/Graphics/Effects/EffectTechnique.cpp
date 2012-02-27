#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Effect.h"
#include "Lights\Light.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "Base.h"
#include "Logger\Logger.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "RenderManager.h"
#include "Utils\Timer.h"
#include <sstream>
#include "Lights\LightManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffectTechnique::CEffectTechnique(	CXMLTreeNode &XMLNode )
	: m_D3DTechnique(NULL)
	, m_UseInverseProjMatrix(false)
	, m_UseInverseViewMatrix(false)
	, m_UseInverseWorldMatrix(false)
	, m_UseLightAmbientColor(false)
	, m_UseViewMatrix(false)
	, m_UseViewProjectionMatrix(false)
	, m_UseTime(false)
{
	m_TechniqueName					= XMLNode.GetPszProperty("name", "");
	m_UseWorldMatrix				= XMLNode.GetBoolProperty("use_world_matrix", false, false);
	m_UseViewProjectionMatrix		= XMLNode.GetBoolProperty("use_view_projection_matrix", false, false);
	m_UseInverseViewProjMatrix		= XMLNode.GetBoolProperty("use_view_projection_inverse_matrix", false, false);
	m_UseWorldViewProjectionMatrix	= XMLNode.GetBoolProperty("use_world_view_projection_matrix", false, false);
	m_UseProjMatrix					= XMLNode.GetBoolProperty("use_projection_matrix", false, false);
	m_UseWorldViewMatrix			= XMLNode.GetBoolProperty("use_world_view_matrix", false, false);
	m_UseShadowViewProjectionMatrix = XMLNode.GetBoolProperty("use_shadow_view_projection_matrix", false, false);
	m_UseShadowWorldViewProjMatrix	= XMLNode.GetBoolProperty("use_shadow_world_view_projection_matrix", false, false);
	m_UseShadowCameraPosition		= XMLNode.GetBoolProperty("use_shadow_camera_position", false, false);
	m_UseShadowMaps					= XMLNode.GetBoolProperty("use_shadow_maps", false, false);
	m_UseInverseViewMatrix			= XMLNode.GetBoolProperty("use_view_inverse_matrix", false, false);
	m_UseInverseWorldMatrix			= XMLNode.GetBoolProperty("use_world_inverse_matrix", false, false);
	m_UseInverseProjMatrix			= XMLNode.GetBoolProperty("use_proj_inverse_matrix", false, false);
	m_UseCameraPosition				= XMLNode.GetBoolProperty("use_camera_position", false, false);
	m_UseLights						= XMLNode.GetBoolProperty("use_lights", false, false);
	m_FowardShading					= XMLNode.GetBoolProperty("use_fs", false, false);
	m_NumOfLights					= static_cast<uint32>( XMLNode.GetIntProperty("num_of_lights", 0, false) );
	m_UseHalfPixel					= XMLNode.GetBoolProperty("use_half_pixel", false, false);
	m_UseRenderTargetSize			= XMLNode.GetBoolProperty("use_render_target_size", false, false);
	m_UseTime						= XMLNode.GetBoolProperty("use_time", false, false);

	//Lectura de parámetros
	m_UseParams						= XMLNode.GetBoolProperty("use_params", false, false);
	if( m_UseParams )
	{
		m_NumOfParams					= XMLNode.GetIntProperty("num_of_params", 0, false);
		if( m_NumOfParams > MAX_PARAMS_BY_EFFECT )
		{
			std::string err = "CEffectTechnique::CEffectTechnique->Demasiados parámetros definidos. Sólo se permiten como máximo " + MAX_PARAMS_BY_EFFECT;
			LOGGER->AddNewLog(ELL_WARNING, err.c_str() );
		}
		else
		{
			for(uint16 i=0; i<m_NumOfParams; ++i)
			{
				std::stringstream out;
				out << i;
				std::string l_Name = "parameter_" + out.str();
				float l_Value = XMLNode.GetFloatProperty(l_Name.c_str(), 0.0f);

				m_Params.push_back(l_Value);
			}
		}
	}

	std::string l_EffectName = XMLNode.GetPszProperty("effect", "");
	m_Effect = CORE->GetEffectManager()->GetEffect(l_EffectName);

	Refresh();
}

CEffectTechnique::~CEffectTechnique()
{
	m_Effect = NULL;
}

bool CEffectTechnique::BeginRender()
{
	LPD3DXEFFECT l_Effect = m_Effect->GetD3DEffect();
	if( l_Effect == NULL )
	{
		return false;
	}

	CEffectManager *l_EffectManager = CORE->GetEffectManager();
	
	std::string msg_error = "";

	if( m_UseWorldMatrix )
	{
		l_Effect->SetMatrix( m_Effect->GetWorldMatrix(), &l_EffectManager->GetWorldMatrix().GetD3DXMatrix() );
	}

	if( m_UseViewMatrix )
	{
		l_Effect->SetMatrix( m_Effect->GetViewMatrix(), &l_EffectManager->GetViewMatrix().GetD3DXMatrix() );
	}

	if( m_UseProjMatrix )
	{
		l_Effect->SetMatrix( m_Effect->GetProjectionMatrix(), &l_EffectManager->GetProjectionMatrix().GetD3DXMatrix() );
	}

	if( m_UseWorldViewMatrix )
	{
		Mat44f l_WorldMatrix = l_EffectManager->GetWorldMatrix();
		Mat44f l_ViewMatrix = l_EffectManager->GetViewMatrix();
		l_WorldMatrix = l_WorldMatrix * l_ViewMatrix;

		l_Effect->SetMatrix( m_Effect->GetWorldViewMatrix(), &l_WorldMatrix.GetD3DXMatrix() );

	}

	if( m_UseViewProjectionMatrix )
	{
		l_Effect->SetMatrix( m_Effect->GetViewProjectionMatrix(), &l_EffectManager->GetViewProjectionMatrix().GetD3DXMatrix() );
	}

	if( m_UseWorldViewProjectionMatrix )
	{
		Mat44f l_ViewProjMatrix = l_EffectManager->GetViewProjectionMatrix();
		Mat44f l_WorldMatrix = l_EffectManager->GetWorldMatrix();
		l_WorldMatrix =  l_ViewProjMatrix*l_WorldMatrix;

		l_Effect->SetMatrix( m_Effect->GetWorldViewProjectionMatrix(), &l_WorldMatrix.GetD3DXMatrix() );
	}

	if( m_UseShadowViewProjectionMatrix )
	{
		Mat44f l_ShadowViewProjMatrix = l_EffectManager->GetShadowViewProjMatrix();

		if( FAILED( l_Effect->SetMatrix( m_Effect->GetShadowViewProjectionMatrix(), &l_ShadowViewProjMatrix.GetD3DXMatrix() ) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->l_ShadowViewProjMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}
	}

	if(m_UseShadowWorldViewProjMatrix)
	{
		Mat44f l_ShadowWorldViewProjMatrix = l_EffectManager->GetShadowWorldViewProjMatrix();

		if( FAILED( l_Effect->SetMatrix( m_Effect->GetShadowWorldViewProjectionMatrix(), &l_ShadowWorldViewProjMatrix.GetD3DXMatrix() ) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetShadowWorldViewProjectionMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}
	}

	if( m_UseShadowCameraPosition )
	{
		Vect3f l_CameraEye = l_EffectManager->GetShadowCameraEye();
		float l_Camera[3];
		l_Camera[0] = l_CameraEye.x;
		l_Camera[1] = l_CameraEye.y;
		l_Camera[2] = l_CameraEye.z;

		l_Effect->SetFloatArray( m_Effect->GetShadowCameraPositionMatrix(), l_Camera, 3);
	}

	if( m_UseInverseViewMatrix )
	{
		Mat44f l_ViewInverseMatrix = l_EffectManager->GetViewInverseMatrix();

		if( FAILED( l_Effect->SetMatrix( m_Effect->GetViewInverseMatrix(), &l_ViewInverseMatrix.GetD3DXMatrix() ) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetViewInverseMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}
	}

	if( m_UseInverseProjMatrix )
	{
		Mat44f l_ProjInverseMatrix = l_EffectManager->GetProjInverseMatrix();

		if( FAILED( l_Effect->SetMatrix( m_Effect->GetProjInverseMatrix(), &l_ProjInverseMatrix.GetD3DXMatrix() ) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetProjInverseMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}
	}

	if( m_UseInverseViewProjMatrix )
	{
		Mat44f InverseViewProj = l_EffectManager->GetViewProjectionMatrix().GetInverted();

		if( FAILED( l_Effect->SetMatrix(m_Effect->GetViewProjectionInverseMatrix(), &InverseViewProj.GetD3DXMatrix()) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetViewProjectionInverseMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}
	}

	if( m_UseCameraPosition )
	{
		Vect3f l_CameraEye = l_EffectManager->GetCameraEye();
		float l_Camera[3];
		l_Camera[0] = l_CameraEye.x;
		l_Camera[1] = l_CameraEye.y;
		l_Camera[2] = l_CameraEye.z;

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetCameraPositionMatrix(), l_Camera, 3) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetCameraPositionMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}
	}

	if( m_UseLights )
	{
		if(m_FowardShading)
		{
			m_Effect->SetLights(m_NumOfLights);
		}

		const BOOL *l_Enabled = m_Effect->GetLightEnabled();
		const int *l_Type = m_Effect->GetLightType();
		const float *l_Angle = m_Effect->GetLightAngle();
		const float *l_FallOff = m_Effect->GetLightFallOff();
		const float *l_StartAtt = m_Effect->GetLightStartAtt();
		const float *l_EndAtt = m_Effect->GetLightEndAtt();
		const Vect3f *l_Pos = m_Effect->GetLightPosition();
		const Vect3f *l_Dir = m_Effect->GetLightDirection();
		const Vect3f *l_Color = m_Effect->GetLightColor();

		l_Effect->SetInt( m_Effect->GetNumLights(), m_NumOfLights );

		if( FAILED( l_Effect->SetBoolArray( m_Effect->GetLightEnabledMatrix(), l_Enabled, m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightEnabledMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetIntArray( m_Effect->GetLightsTypeMatrix(), l_Type, m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsTypeMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetLightsAngleMatrix(), l_Angle, m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsAngleMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetLightsFallOffMatrix(), l_FallOff, m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsFallOffMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetLightsStartRangeMatrix(), l_StartAtt, m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsStartRangeMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetLightsEndRangeMatrix(), l_EndAtt, m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsEndRangeMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetLightsPositionMatrix(), (float*)l_Pos, 3*m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsPositionMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetLightsDirectionMatrix(), (float*)l_Dir, 3*m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsDirectionMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetLightsColorMatrix(), (float*)l_Color, 3*m_NumOfLights) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsColorMatrix()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

	}

	if(m_UseShadowMaps)
	{
		CTexture* dynamicMap = m_Effect->GetLightsDynamicShadowMap();
		CTexture* staticMap = m_Effect->GetLightsStaticShadowMap();
		
		if( FAILED( l_Effect->SetTexture( m_Effect->GetLightsDynamicShadowMapParameter(), dynamicMap->GetDXTexture()) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsDynamicShadowMapParameter()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}

		if( FAILED( l_Effect->SetTexture( m_Effect->GetLightsStaticShadowMapParameter(), staticMap->GetDXTexture()) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetLightsStaticShadowMap()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}
	}

	if(m_UseHalfPixel)
	{
		Vect2f hp = CORE->GetRenderManager()->GetHalfPixel();
		float hpA[2] = {hp.x, hp.y};

		if( FAILED( l_Effect->SetFloatArray( m_Effect->GetHalfPixel(), hpA, 2) ) )
		{
			msg_error = "Error al hacer el Set del parametro: m_Effect->GetHalfPixel()";
			LOGGER->AddNewLog(ELL_WARNING,  msg_error.c_str());
		}
	}

	if( m_UseRenderTargetSize )
	{
		Vect2f l_RenderTargetSize = CORE->GetRenderManager()->GetRenderTargetSize();

		float l_RenderTarget[2];
		l_RenderTarget[0] = l_RenderTargetSize.x;
		l_RenderTarget[1] = l_RenderTargetSize.y;
		l_Effect->SetFloatArray( m_Effect->GetRenderTargetSize(), l_RenderTarget, 2);
	}

	if( m_UseParams )
	{
		for(uint16 i=0; i<m_NumOfParams; ++i)
		{
			l_Effect->SetFloat( m_Effect->GetParameterById(i), m_Params[i] );
		}
	}

	if( m_UseLightAmbientColor )
	{
		//TODO: ¿Que se la pasa aquí?
	}

	if( m_UseTime )
	{
		l_Effect->SetFloat( m_Effect->GetTimeMatrix(), CORE->GetTimer()->GetTotalTime() );
	}

	return true;
}

bool CEffectTechnique::Refresh()
{
	D3DXHANDLE l_Technique = m_Effect->GetTechniqueByName( m_TechniqueName );

	if( l_Technique != NULL )
	{
		m_D3DTechnique = l_Technique;
		return true;
	}

	return false;
}

void CEffectTechnique::SetValueFromParam(uint16 id, float value)
{
	if( id < m_Params.size() )
	{
		m_Params[id] = value;
	}
}