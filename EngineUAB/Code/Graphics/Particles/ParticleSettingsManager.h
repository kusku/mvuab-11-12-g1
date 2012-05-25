
#pragma once

#ifndef _PARTICLIE_SETTINGS_MANAGER_h
#define _PARTICLIE_SETTINGS_MANAGER_h

#include <string>
#include "ParticleDef.h"
#include "Utils\MapManager.h"

class CParticleSettingsManager : public CMapManager<TParticleSystemSettings>
{
	private:
		std::string				m_Filename;

	public:
		CParticleSettingsManager();
		virtual ~CParticleSettingsManager();

		bool					Load					(const std::string& filename);
		bool					Reload					();
};


#endif