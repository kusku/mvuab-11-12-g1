#include "LightManager.h"
#include "RenderManager.h"
#include "OmniLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "XML\XMLTreeNode.h"
#include "Math\Vector3.h"
#include "Logger\Logger.h"
#include "Cameras\Frustum.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CLightManager::CLightManager()
	: m_FileName("")
	, m_CurrentFrustum(NULL)
{
}

CLightManager::~CLightManager()
{
	CleanUp();
}

void CLightManager::CleanUp()
{
	Destroy();
}

bool CLightManager::Load(const std::string &Filename)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CLightmanager::Load->Cargando las luces.");

	m_FileName = Filename;
	return LoadFile();
}

bool CLightManager::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CLightManager::Reload->Reload de las luces.");
	CleanUp();
	return LoadFile();
}

bool CLightManager::LoadFile()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CLightManager::Load->Error al intentar leer el archivo xml de luces: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}	
	
	CXMLTreeNode l_Lights = newFile["lights"];
	if( l_Lights.Exists() )
	{
		uint16 l_Count = l_Lights.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_TypeLight = l_Lights(i).GetName();
			if( l_TypeLight == "omni" )
			{
				COmniLight *l_Light = new COmniLight( l_Lights(i) );
				std::string l_Name = l_Light->GetName();

				AddResource(l_Name, l_Light);
				l_Light = NULL;
			}
			else if( l_TypeLight == "directional" )
			{
				CDirectionalLight *l_Light = new CDirectionalLight( l_Lights(i) );

				l_Light->CalculateOrientationDebugRender();
				std::string l_Name = l_Light->GetName();

				AddResource(l_Name, l_Light);
				l_Light = NULL;

			}
			else if( l_TypeLight == "spot" )
			{
				CSpotLight *l_Light = new CSpotLight( l_Lights(i) );

				l_Light->CalculateOrientationDebugRender();
				std::string l_Name = l_Light->GetName();

				AddResource(l_Name, l_Light);
				l_Light = NULL;
			}

		}
	}
	else
	{
		return false;
	}

	return true;
}

void CLightManager::Render(CRenderManager *RM)
{
	uint16 l_NumLights = m_ResourcesVector.size();

	for(uint16 i=0; i<l_NumLights; ++i)
	{
		m_ResourcesVector[i]->Render(RM);
	}
}