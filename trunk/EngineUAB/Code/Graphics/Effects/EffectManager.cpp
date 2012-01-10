#include "EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffectManager::CEffectManager()
	: m_StaticMeshTechnique(NULL)
	, m_AnimatedModelTechnique(NULL)
{
}


CEffectManager::~CEffectManager()
{
	CleanUp();
}

void CEffectManager::CleanUp()
{
	m_DefaultTechniqueEffectMap.clear();
	m_Effects.Destroy();
	Destroy();
}

void CEffectManager::Load(const std::string &Filename)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(Filename.c_str()))
	{
		std::string msg_error = "CEffectManager::Load->Error al intentar leer el archivo de efectos: " + Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	CXMLTreeNode l_Effects = newFile["effects"];
	if ( l_Effects.Exists() )
	{
		uint16 l_Count = l_Effects.GetNumChildren();
		for( uint16 i = 0; i < l_Count; ++i )
		{
			std::string l_Type = l_Effects(i).GetName();

			if( l_Type == "effect" )
			{
				std::string l_EffectName = l_Effects(i).GetPszProperty("name", "");
				std::string l_EffectPath = l_Effects(i).GetPszProperty("file", "");

				CEffect* l_Effect = new CEffect();
				l_Effect->Load( l_EffectPath );

				m_Effects.AddResource( l_EffectName, l_Effect );

				l_Effect = NULL;
			}
			else if ( l_Type == "technique" )
			{
				//TODO: Falta asignar el valor l_CameraPos

				std::string l_TechniqueName = l_Effects(i).GetPszProperty("name", "");
				std::string l_TechniqueEffect = l_Effects(i).GetPszProperty("effect", "");
				bool l_WorldMatrix = l_Effects(i).GetBoolProperty("use_world_matrix", false);
				bool l_ViewProjectionMatrix = l_Effects(i).GetBoolProperty("use_world_view_projection_matrix", false);
				bool l_ProjectionMatrix = l_Effects(i).GetBoolProperty("use_projection_matrix", false);
				bool l_ViewMatrix = l_Effects(i).GetBoolProperty("use_world_view_matrix", false);
				bool l_ViewToLightProjectionMatrix = l_Effects(i).GetBoolProperty("use_view_to_light_projection_matrix", false);
				bool l_CameraPos = l_Effects(i).GetBoolProperty("use_camera_position", false);
				bool l_Lights = l_Effects(i).GetBoolProperty("use_lights", false);
				uint32 l_NumLights = static_cast<uint32>( l_Effects(i).GetIntProperty("num_of_lights", 0) );

				CEffectTechnique* l_Technique = new CEffectTechnique();
				l_Technique->SetTechniqueName( l_TechniqueName );
				l_Technique->SetUseWorldMatrix( l_WorldMatrix );
				l_Technique->SetUseViewProjectionMatrix( l_ViewProjectionMatrix );
				l_Technique->SetUseProjMatrix( l_ProjectionMatrix );
				l_Technique->SetUseViewMatrix( l_ViewMatrix );
				l_Technique->SetUseViewToLightProjectionMatrix( l_ViewToLightProjectionMatrix );
				l_Technique->SetUseLights( l_Lights );
				l_Technique->SetNumOfLights( l_NumLights );

				CEffect *l_Effect = m_Effects.GetResource( l_TechniqueEffect );
				l_Technique->SetEffect( l_Effect );
				if( l_Effect == NULL )
				{
					std::string msg_warning = "CEffectManager::Load->Efecto: "+ l_TechniqueEffect +" de la Technique: " + l_TechniqueName + " no encontrado.";
					LOGGER->AddNewLog(ELL_WARNING, msg_warning.c_str());
				}

				AddResource( l_TechniqueName, l_Technique );

				l_Technique = NULL;
				l_Effect = NULL;
			}
			else if( l_Type == "default_technique" )
			{
				int l_VertexType = l_Effects(i).GetIntProperty("vertex_type", 0);
				std::string l_TechniqueName = l_Effects(i).GetPszProperty("technique", "");

				m_DefaultTechniqueEffectMap[l_VertexType] = l_TechniqueName;
			}
		}
	}
}

void CEffectManager::Reload()
{
}

void CEffectManager::ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix, const Vect3f &CameraEye)
{
}

std::string CEffectManager::GetTechniqueEffectNameByVertexDefault(uint16 VertexType)
{
	return "";
}

size_t CEffectManager::GetMaxLights() const
{
	return 0;
}