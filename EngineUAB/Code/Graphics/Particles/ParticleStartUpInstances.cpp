
#include <assert.h>
#include "ParticleStartUpInstances.h"
#include "ParticleEmitter.h"
#include "ParticleEmitterSystem.h"
#include "ParticleEmitterSystemManager.h"
#include "ParticleEmitterManager.h"
#include "Core.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleStartUpInstances::CParticleStartUpInstances()
	: m_Filename("")
{
}

CParticleStartUpInstances::~CParticleStartUpInstances()
{
}

bool CParticleStartUpInstances::Load( const std::string& filename )
{
	m_Filename = filename;

	return Reload();
}

bool CParticleStartUpInstances::Reload()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "CParticleStartUpInstances::Load->Error al intentar leer el archivo xml: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_xml = newFile["particle_startup_instances"];
	if( l_xml.Exists() )
	{
		uint16 l_Count = l_xml.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_xml(i).GetName();

			if( l_Type == "emitter_system_instances" )
			{				
				uint16 l_CountChild = l_xml(i).GetNumChildren();
				CXMLTreeNode xmlChild = l_xml(i);

				for (int j = 0l; j < l_CountChild; ++j)
				{
					std::string l_TypeChild = xmlChild(j).GetName();

					if(l_TypeChild == "instance")
					{
						std::string name = xmlChild(j).GetPszProperty("name", "", true);
						std::string nameSystem = xmlChild(j).GetPszProperty("emitter_system_name", "", true);

						CParticleEmitterSystem* emsys = CORE->GetParticleEmitterSystemManager()->GetResource(nameSystem);

						assert(emsys);

						if(emsys == NULL)
						{
							continue;
						}

						CParticleEmitterSystemInstance* inst = emsys->GetParticleEmitterSystemInstance(name);

						assert(inst);
					}
				}
			}

			if( l_Type == "emitter_instance" )
			{				
				uint16 l_CountChild = l_xml(i).GetNumChildren();
				CXMLTreeNode xmlChild = l_xml(i);

				for (int j = 0l; j < l_CountChild; ++j)
				{
					std::string l_TypeChild = xmlChild(j).GetName();

					if(l_TypeChild == "instance")
					{
						std::string name = xmlChild(j).GetPszProperty("name", "", true);
						std::string nameEm = xmlChild(j).GetPszProperty("emitter_name", "", true);

						CParticleEmitter* em = CORE->GetParticleEmitterManager()->GetResource(nameEm);

						assert(em);

						if(em == NULL)
						{
							continue;
						}

						CParticleEmitterInstance* inst = em->GetParticleEmitterInstance(name);

						assert(inst);
					}
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}