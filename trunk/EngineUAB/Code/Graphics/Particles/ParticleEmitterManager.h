
#pragma once

#ifndef _PARTICLIE_EMITTER_MANAGER_h
#define _PARTICLIE_EMITTER_MANAGER_h

#include <string>
#include "ParticleEmitter.h"
#include "Utils\MapManager.h"

class CParticleEmitterManager : public CMapManager<CParticleEmitter>
{
	private:
		std::string		m_Filename;

	public:
		CParticleEmitterManager();
		virtual ~CParticleEmitterManager();

		void			Update			(float elapsedTime);

		bool			Load					(const std::string& filename);
		bool			Reload					();
		void			CleanUp					();
};


#endif