#pragma once

#ifndef _PARTICLE_EDITOR_PROCESS_H_
#define _PARTICLE_EDITOR_PROCESS_H_

#include "EngineProcess.h"
#include "Player\Player.h"
#include <string>

class CRenderManager;
class CThPSCamera;

class CParticleEditorProcess : public CEngineProcess
{
public:
	CParticleEditorProcess	();
	~CParticleEditorProcess	();

	bool			Init				();
	void			Done				();
	void			Update				( float _ElapsedTime );
	void			Render				( CRenderManager &_RM );

	virtual void	Release				();

	static void		DisableAllEmitters	();
	void			SetSettingsToEmitters();

	void			SetParticleSettings	( const std::string &settings );

private:
	void			LoadSystemsOnEditor	();

protected:
	bool				m_bIsInitializeParticles;

	CThPSCamera			*m_pThPSCamera;
	CPlayer				m_Player;

	std::string			m_CurrentParticleSettings;
	std::string			m_PreviousTextureFilename;
};


#endif //_PARTICLE_EDITOR_PROCESS_H_