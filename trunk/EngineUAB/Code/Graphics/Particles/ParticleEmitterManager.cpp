#include "ParticleEmitterManager.h"
#include "ParticleSystem.h"
#include "Emitters\ParticleEmitterRing.h"
#include "Emitters\ParticleEmitterPoint.h"
#include "Emitters\ParticleEmitterLine.h"
#include "Emitters\ParticleEmitterBox.h"
#include "Emitters\ParticleEmitterSphere.h"
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
	CleanUp();
}

void CParticleEmitterManager::CleanUp()
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
				std::string type = l_xml(i).GetPszProperty("type", "", true);
				Vect3f initPos = l_xml(i).GetVect3fProperty("initialPosition", Vect3f(0, 0, 0), true);
				bool onLoop = l_xml(i).GetBoolProperty("on_loop", true, false);
				bool useDis = l_xml(i).GetBoolProperty("use_dis", true, false);

				float particlesPerSecond = l_xml(i).GetFloatProperty("particlesPerSecond", 0, true) / CORE->GetConfig().particle_level;
				uint32 pej = (uint32)l_xml(i).GetIntProperty("ejection_count", 0, !onLoop) / CORE->GetConfig().particle_level;

				particlesPerSecond = particlesPerSecond == 0.0f ? 1.0f : particlesPerSecond;
				pej = pej == 0 ? 1 : pej;

				CParticleSystem* system = CORE->GetParticleSystemManager()->GetResource(systemName);
				assert(system);

				CParticleEmitter* emitter = NULL;

				if(type == "point")
				{
					CParticleEmitterPoint* point = new CParticleEmitterPoint(name, system, particlesPerSecond, initPos, useDis);

					emitter = point;
				}
				else if(type == "ring")
				{
					bool horizontal = l_xml(i).GetBoolProperty("horizontal", false, true);
					float radius = l_xml(i).GetFloatProperty("radius", 0, true);
					float height = l_xml(i).GetFloatProperty("height", 0, true);
					float width = l_xml(i).GetFloatProperty("width", 0, true);

					CParticleEmitterRing* ring = new CParticleEmitterRing(name, system, particlesPerSecond, initPos, useDis, horizontal, radius, height, width);

					emitter = ring;
				}
				else if(type == "line")
				{
					Vect3f lineStart = l_xml(i).GetVect3fProperty("line_start", v3fZERO);
					Vect3f lineEnd = l_xml(i).GetVect3fProperty("line_end", v3fZERO);

					CParticleEmitterLine* line = new CParticleEmitterLine(name, system, particlesPerSecond, initPos, useDis, lineStart, lineEnd);

					emitter = line;
				}
				else if(type == "box")
				{
					Vect3f boxMin = l_xml(i).GetVect3fProperty("box_min", v3fZERO);
					Vect3f boxMax = l_xml(i).GetVect3fProperty("box_max", v3fZERO);

					CParticleEmitterBox* box = new CParticleEmitterBox(name, system, particlesPerSecond, initPos, useDis, boxMin, boxMax);

					emitter = box;
				}
				else if(type == "sphere")
				{
					float radius = l_xml(i).GetFloatProperty("radius", 1.f);

					CParticleEmitterSphere *sphere = new CParticleEmitterSphere(name, system, particlesPerSecond, initPos, useDis, radius);

					emitter = sphere;
				}

				assert(emitter);
				
				emitter->SetOnLoop(onLoop);
				emitter->SetParticlesEjectionCount(pej);

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


