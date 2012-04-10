#pragma once

#ifndef __CLASS_BILLBOARD_MANAGER_H__
#define __CLASS_BILLBOARD_MANAGER_H__

#include <string>
#include <vector>
#include "Utils\MapManager.h"
#include "Math\Vector3.h"
#include "BillboardAnimation.h"

//---Forward Declarations---
class CRenderManager;
class CBillboard;
class CParticlesEmitter;
//--------------------------

class CBillboardManager
{
public:
	//--- Init and End protocols------------------------------------------
					CBillboardManager	( void );
	virtual			~CBillboardManager	( void );

	//----Main Functions ------------------------------------------------
	void			Destroy				( void );
	bool			Load				( const std::string &_FileName );
	bool			Reload				( void );
	
	void			Update				( float _ElapsedTime );
	void			Render				( CRenderManager &_RM );
	
	//----Functions -----------------------------------------------------
	bool			LoadXML				( void );

	//----Properties ( get & Set )---------------------------------------
	void			SetXMLFile			( const std::string &_Filename )		{ m_szFilename = _Filename; }
	std::string		GetXMLFile			( void )	const						{ return m_szFilename; }

	//inline			CBillboardAnimation operator = ( const CBillboardAnimation& _pBillboardAnimation )    const;

	//----Members ---------------------------------------
private:
	std::string							m_szFilename;

	CMapManager<CBillboardAnimation>	m_vBillboardAnimationVectorCORE;
	std::vector<CBillboardAnimation*>	m_vBillboardAnimationVectorINSTANCES;

	//CParticleEmitter*					m_Emisor;
};

#endif __CLASS_BILLBOARD_MANAGER_H__
