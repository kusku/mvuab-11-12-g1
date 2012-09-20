
#pragma once

#ifndef _PARTICLIE_EMITTER_SYSTEM_MANAGER_h
#define _PARTICLIE_EMITTER_SYSTEM_MANAGER_h

#include <string>
#include "ParticleEmitterSystem.h"
#include "Utils\MapManager.h"

class CParticleEmitterSystemManager : public CMapManager<CParticleEmitterSystem>
{
	private:
		std::string		m_Filename;

	public:
		CParticleEmitterSystemManager();
		virtual ~CParticleEmitterSystemManager();
		
		bool			Load					(const std::string& filename);
		bool			Reload					();
		void			CleanUp					();
};


#endif