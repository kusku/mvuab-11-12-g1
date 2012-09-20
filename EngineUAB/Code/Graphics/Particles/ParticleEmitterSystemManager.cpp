#include "ParticleEmitterSystemManager.h"
#include "ParticleSystem.h"
#include "ParticleSystemManager.h"
#include "ParticleEmitterManager.h"
#include "ParticleEmitter.h"

#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Logger\Logger.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitterSystemManager::CParticleEmitterSystemManager()
	: m_Filename("")
{

}

CParticleEmitterSystemManager::~CParticleEmitterSystemManager()
{
	CleanUp();
}

void CParticleEmitterSystemManager::CleanUp()
{
	this->Destroy();
}

bool CParticleEmitterSystemManager::Load( const std::string& filename )
{
	m_Filename = filename;

	return Reload();
}

bool CParticleEmitterSystemManager::Reload()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "CParticleEmitterSystemManager::Load->Error al intentar leer el archivo xml: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_xml = newFile["particle_emitters_systems"];
	if( l_xml.Exists() )
	{
		uint16 l_Count = l_xml.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_xml(i).GetName();

			if( l_Type == "emitter_system" )
			{
				std::string name = l_xml(i).GetPszProperty("name", "", true);
				
				assert(this->GetResource(name) == NULL);

				CParticleEmitterSystem* pes = new CParticleEmitterSystem(name);

				uint16 l_CountEmitters = l_xml(i).GetNumChildren();
				CXMLTreeNode childs = l_xml(i);

				for(uint16 j=0; j<l_CountEmitters; ++j)
				{
					std::string l_ChildType = childs(j).GetName();

					if(l_ChildType == "emitter")
					{
						std::string nameEmitter = childs(j).GetPszProperty("name", "", true);

						CParticleEmitter* pe = CORE->GetParticleEmitterManager()->GetResource(nameEmitter);

						pes->AddParticleEmitter(nameEmitter, pe);
					}
				}

				this->AddResource(name, pes);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}


