#pragma once

#ifndef _PARTICLIE_SETTINGS_MANAGER_h
#define _PARTICLIE_SETTINGS_MANAGER_h

#include <string>
#include "ParticleDef.h"
#include "Utils\MapManager.h"
#include "Utils\TemplatedVectorMapManager.h"

class CParticleSettingsManager : public CTemplatedVectorMapManager<TParticleSystemSettings>
{
	private:
		std::string				m_Filename;

	public:
		CParticleSettingsManager();
		virtual ~CParticleSettingsManager();

		bool					Load					(const std::string& filename);
		bool					Reload					();
		void					CleanUp					();

		void					AddDefaultSettings		(const std::string &name);
};


#endif