
#include "ParticleEmitterManager.h"
#include "ParticleSystem.h"
#include "Emitters\ParticleEmitterRing.h"
#include "Emitters\ParticleEmitterPoint.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "ParticleSystemManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitterManager::CParticleEmitterManager()
	: m_Filename("")
{

}

CParticleEmitterManager::~CParticleEmitterManager()
{
	this->Destroy();
}

void CParticleEmitterManager::Update( float elapsedTime )
{
	TMapResource::iterator it = m_Resources.begin();
	TMapResource::iterator itEnd = m_Resources.end();

	for (; it != itEnd; ++it)
	{
		it->second->Update(elapsedTime);
	}
}

bool CParticleEmitterManager::Load( const std::string& filename )
{
	m_Filename = filename;

	return Reload();
}

bool CParticleEmitterManager::Reload()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "CParticleEmitterManager::Load->Error al intentar leer el archivo xml: " + m_Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_xml = newFile["particle_emitters"];
	if( l_xml.Exists() )
	{
		uint16 l_Count = l_xml.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_xml(i).GetName();

			if( l_Type == "emitter" )
			{
				std::string name = l_xml(i).GetPszProperty("name", "", true);
				std::string systemName = l_xml(i).GetPszProperty("system_name", "", true);
				float particlesPerSecond = l_xml(i).GetFloatProperty("particlesPerSecond", 0, true);
				std::string type = l_xml(i).GetPszProperty("type", "", true);
				Vect3f initPos = l_xml(i).GetVect3fProperty("initialPosition", Vect3f(0, 0, 0), true);

				CParticleSystem* system = CORE->GetParticleSystemManager()->GetResource(systemName);
				assert(system);

				CParticleEmitter* emitter = NULL;

				if(type == "point")
				{
					CParticleEmitterPoint* point = new CParticleEmitterPoint(name, system, particlesPerSecond, initPos);

					emitter = point;
				}
				else if(type == "ring")
				{
					bool horizontal = l_xml(i).GetBoolProperty("horizontal", false, true);
					float radius = l_xml(i).GetFloatProperty("radius", 0, true);
					float height = l_xml(i).GetFloatProperty("height", 0, true);
					float width = l_xml(i).GetFloatProperty("width", 0, true);

					CParticleEmitterRing* ring = new CParticleEmitterRing(name, system, particlesPerSecond, initPos, horizontal, radius, height, width);

					emitter = ring;
				}

				assert(emitter);
				
				bool isOk = this->AddResource(name, emitter);
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


