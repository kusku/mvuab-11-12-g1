#pragma once

#ifndef __CLASS_PARTICLES_SYSTEM_MANAGER_H__
#define __CLASS_PARTICLES_SYSTEM_MANAGER_H__

#include <string>
#include <vector>
#include <list>
#include "ParticleDef.h"
#include "Utils\MapManager.h"
#include "Math\Vector3.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "ParticlesEmitterInstance.h"
#include "ParticlesEmitterCore.h"

//---Forward Declarations---
class CRenderManager;
//class CParticlesEmitterInstance;
//class CParticlesEmitterCore;
//--------------------------

class CParticlesSystemManager : public CTemplatedVectorMapManager<CParticlesEmitterInstance>
{
public:
	//--- Init and End protocols------------------------------------------
					CParticlesSystemManager	( void );
	virtual			~CParticlesSystemManager	( void );

	//----Main Functions ------------------------------------------------
	void			Destroy					( void );
	bool			Load					( const std::string &_FileName );
	bool			Reload					( void );
	
	void			Update					( float _ElapsedTime );
	void			Render					( CRenderManager &_RM );
	
	//----Properties ( get & Set )---------------------------------------
	void			SetXMLFile				( const std::string &_Filename )		{ m_szFilename = _Filename; }
	std::string		GetXMLFile				( void )	const						{ return m_szFilename; }

	CParticlesEmitterInstance * GetParticlesEmitterInstance	( const std::string &_Name );
	CParticlesEmitterCore	  *	GetParticlesEmitterCore		( const std::string &_Name );
	
	//----Members -------------------------------------------------------
private:
	std::string							m_szFilename;
	
	CMapManager<CParticlesEmitterCore>	m_EmittersCores;

	//----Methods -----------------------------------------------------
	bool			LoadXML					( void );
};

#endif __CLASS_PARTICLES_SYSTEM_MANAGER_H__