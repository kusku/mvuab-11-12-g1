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
	, m_Filename("")
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
	m_Filename = Filename;

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
				CEffect* l_Effect = new CEffect( &l_Effects(i) );
				l_Effect->Load();

				m_Effects.AddResource( l_Effect->GetEffectName(), l_Effect );
				l_Effect = NULL;
			}
			else if ( l_Type == "technique" )
			{
				CEffectTechnique* l_Technique = new CEffectTechnique(&l_Effects(i));
				AddResource( l_Technique->GetTechniqueName(), l_Technique );
				l_Technique = NULL;
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
	CleanUp();
	Load(m_Filename);
}

void CEffectManager::ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix, const Vect3f &CameraEye)
{
	m_ViewMatrix = ViewMatrix;
	m_ProjectionMatrix = ProjectionMatrix;
	m_CameraEye = CameraEye;

	m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
}

std::string CEffectManager::GetTechniqueEffectNameByVertexDefault(uint16 VertexType)
{
	TDefaultTechniqueEffectMap::iterator l_EffectName = m_DefaultTechniqueEffectMap.find(VertexType);
	if( l_EffectName != m_DefaultTechniqueEffectMap.end() )
	{
		return l_EffectName->second; 
	}
	return "";
}

size_t CEffectManager::GetMaxLights() const
{
	return MAX_LIGHTS_BY_SHADER;
}