#pragma once

#ifndef __CLASS_PARTICLE_MANAGER_H__
#define __CLASS_PARTICLE_MANAGER_H__

#include "Math\Vector3.h"
#include "Math\Color.h"
#include "Billboard\Billboard.h"

//---Forward Declarations---
class CRenderManager;
class CCamera;
//--------------------------


class CParticle : public CBillboard
{
public:
	//--- Init and End protocols-----------------------------------------
				CParticle			( void );
				~CParticle			( void );
		
	//----Funcions principals -------------------------------------------
	void		Render				( CRenderManager &_RM, const CColor &_Color = colWHITE );
	bool		Update				( float _ElapsedTime );
	
	//----Funcions privades ---------------------------------------------
private:
	void		Destroy				( void );
	
	//----Properties ( get & Set )---------------------------------------
public:
	void		SetLifeTime		( float _fLifetime )			{ m_fLifetime = _fLifetime; }
	float		GetLifeTime		( void ) const					{ return m_fLifetime; }

	void		SetAge			( float _fAge )					{ m_fAge = _fAge; }
	float		GetAge			( void ) const					{ return m_fAge; }

	void		SetDirection	( const Vect3f &_vDir )			{ m_vDir = _vDir; }
	Vect3f		GetDirection	( void ) const					{ return m_vDir; }
	
	void		SetVelocity		( const Vect3f &_vVelocity )	{ m_vVelocity = _vVelocity; }
	Vect3f		GetVelocity		( void ) const					{ return m_vVelocity; }

	void		SetIsAlive		( bool _bAlive )				{ m_bAlive = _bAlive; }
	bool		IsALive			( void ) 						{ return m_bAlive; }

	void		SetColor		( const CColor &_vColor )		{ m_vColor = _vColor; }
	CColor		GetColor		( void ) const					{ return m_vColor; }

	void		SetCoreIndex	( int _iIndex )					{ m_iCurrentCoreIndex = _iIndex; }
	int			GetCoreIndex	( void ) const					{ return m_iCurrentCoreIndex; }

	void		SetMapTime		( float _fTime )				{ m_fCurrentCoreTime = _fTime; }
	float		GetMapTime		( void ) const					{ return m_fCurrentCoreTime; }

	//----Members -------------------------------------------------------
private:
	float			m_fLifetime;			// Temps total de vida que té la partícula
	float			m_fAge;					// Edad de vida de la partícula
	Vect3f			m_vDir;					// Direcció i velocitat alhora
	bool			m_bAlive;				// Diu si està viva 
	CColor			m_vColor;				// Indica el color de la partícula
	int				m_iCurrentCoreIndex;	// Indica en quin índex del mapa de cores estic. Permet saber en quin temps estic apuntant i anar sequencialment recorrent el mapa de propietats del CORE
	float			m_fCurrentCoreTime;		// Indica quina KEY (temps) del mapa de cores estic. Permet saber en quin temps estic apuntant i anar sequencialment recorrent el mapa de propietats del CORE
	Vect3f			m_vVelocity;
};

#endif __CLASS_PARTICLE_MANAGER_H__
