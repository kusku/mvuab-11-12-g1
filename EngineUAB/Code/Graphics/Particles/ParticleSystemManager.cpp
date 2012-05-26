
#include "ParticleSystemManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"
#include "ParticleSettingsManager.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


CParticleSystemManager::CParticleSystemManager()
	: m_Filename("")
{

}

CParticleSystemManager::~CParticleSystemManager()
{
	this->Destroy();
}

void CParticleSystemManager::Initialize()
{
	TMapResource::iterator it = m_Resources.begin();
	TMapResource::iterator itEnd = m_Resources.end();

	for (; it != itEnd; ++it)
	{
		it->second->Initialize();
	}
}


void CParticleSystemManager::Update( float elapsedTime )
{
	TMapResource::iterator it = m_Resources.begin();
	TMapResource::iterator itEnd = m_Resources.end();

	for (; it != itEnd; ++it)
	{
		it->second->Update(elapsedTime);
	}
}

void CParticleSystemManager::Render()
{
	TMapResource::iterator it = m_Resources.begin();
	TMapResource::iterator itEnd = m_Resources.end();

	for (; it != itEnd; ++it)
	{
		it->second->Render();
	}
}

bool CParticleSystemManager::Load( const std::string& filename )
{
	m_Filename = filename;

	return Reload();
}

bool CParticleSystemManager::Reload()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "CParticleSystemManager::Load->Error al intentar leer el archivo xml: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_xml = newFile["particle_systems"];
	if( l_xml.Exists() )
	{
		uint16 l_Count = l_xml.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_xml(i).GetName();

			if( l_Type == "system" )
			{
				std::string name = l_xml(i).GetPszProperty("name", "", true);
				std::string settingsName = l_xml(i).GetPszProperty("settings_name", "", true);

				TParticleSystemSettings* settings = CORE->GetParticleSettingsManager()->GetResource(settingsName);
				assert(settings);

				CParticleSystem* system = new CParticleSystem(name, settings);

				bool isOk = this->AddResource(name, system);
				assert(isOk);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}
