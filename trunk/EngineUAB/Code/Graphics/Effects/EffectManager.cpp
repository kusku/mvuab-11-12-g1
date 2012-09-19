#include "EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Vertexs\VertexType.h"
#include "Logger\Logger.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


// ------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------
CEffectManager::CEffectManager( void )
	: m_StaticMeshTechnique		(NULL)
	, m_AnimatedModelTechnique	(NULL)
	, m_Filename				("")
{
}

CEffectManager::~CEffectManager( void )
{
	CleanUp();
}

// ------------------------------------------
//			  MÈTODES PRINCIPALS
// ------------------------------------------
void CEffectManager::CleanUp( void )
{
	m_DefaultTechniqueEffectMap.clear();
	m_Effects.Destroy();
	Destroy();
}

bool CEffectManager::Load ( const std::string &_Filename )
{
	m_Filename = _Filename;
	return LoadXML();
}

bool CEffectManager::Reload	( void )
{	
	LOGGER->AddNewLog(ELL_INFORMATION, "CEffectManager::Reload->Reloading effects.");
	CleanUp();
	return LoadXML();
}

bool CEffectManager::LoadXML ( void )
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CEffectManager::Load->Cargando los efectos i techniques.");


	CXMLTreeNode newFile;
	if ( !newFile.LoadFile( m_Filename.c_str() ) )
	{
		std::string msg_error = "CEffectManager::Load->Error al intentar leer el archivo de efectos: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
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
				CEffect* l_Effect = new CEffect( l_Effects(i) );
				l_Effect->Load();

				std::string l_Name = l_Effect->GetEffectName();
				m_Effects.AddResource( l_Effect->GetEffectName(), l_Effect );
				l_Effect = NULL;

				m_EffectsNames.push_back( l_Name );
			}
			else if ( l_Type == "technique" )
			{
				CEffectTechnique* l_Technique = new CEffectTechnique( l_Effects(i) );
				AddResource( l_Technique->GetTechniqueName(), l_Technique );

				if( l_Technique->GetTechniqueName() == "Cal3DTechnique" )
				{
					SetAnimatedModelTechnique( l_Technique );
				}

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

	AssignDefaultTechniques();
	return true;
}

bool CEffectManager::ReloadShaders ( void )
{
	bool l_IsOK = false;
	//Effects
	for(uint16 i=0; i<m_EffectsNames.size(); ++i)
	{
		CEffect *l_Effect = m_Effects.GetResource(m_EffectsNames[i]);
		l_IsOK = l_Effect->Reload();
	}

	//Techniques
	std::map<std::string, CEffectTechnique*>::iterator l_It = m_Resources.begin();
	std::map<std::string, CEffectTechnique*>::iterator l_End = m_Resources.end();
	for(; l_It != l_End; ++l_It)
	{
		l_IsOK = l_IsOK & l_It->second->Refresh();
	}

	LOGGER->AddNewLog(ELL_INFORMATION, "CEffectManager::Reload->Shaders reloaded.\n");
	return l_IsOK;
}

void CEffectManager::ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix, const Vect3f &CameraEye, const Mat44f &PrevViewMatrix, const Mat44f &PrevProjectionMatrix)
{
	m_ViewMatrix = ViewMatrix;
	m_ProjectionMatrix = ProjectionMatrix;

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	//////////////////////////////////

	m_PrevProjectionMatrix = PrevProjectionMatrix;
	m_PrevViewMatrix = PrevViewMatrix;

	m_PrevViewProjectionMatrix = m_PrevProjectionMatrix * m_PrevViewMatrix;

	//////////////////////////////////

	m_ViewInverseMatrix = m_ViewMatrix.GetInverted();
	m_ProjInverseMatrix	= m_ProjectionMatrix.GetInverted();
	
	//////////////////////////////////

	m_CameraEye = CameraEye;
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

void CEffectManager::AssignDefaultTechniques()
{
	TDefaultTechniqueEffectMap::iterator l_End = m_DefaultTechniqueEffectMap.end();

	for(TDefaultTechniqueEffectMap::iterator l_It = m_DefaultTechniqueEffectMap.begin(); l_It != l_End; ++l_It)
	{
		if( CAL3D_HW_VERTEX_BT::GetVertexType() == l_It->first ) //Modelo animado
		{
			m_AnimatedModelTechnique = GetResource(l_It->second);
		}
		else if( TNORMALTEXTURE1_VERTEX::GetVertexType() == l_It->first ) //Malla estática
		{
			m_StaticMeshTechnique = GetResource(l_It->second);
		}

	}
}

// ------------------------------------------
//				PROPIETATS 
// ------------------------------------------

