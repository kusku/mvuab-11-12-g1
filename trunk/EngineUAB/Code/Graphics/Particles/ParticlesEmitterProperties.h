#pragma once

#ifndef __CLASS_PARTICLES_EMITTER_PROPERTIES_H__
#define __CLASS_PARTICLES_EMITTER_PROPERTIES_H__

#include <string>
#include "Math\Color.h"
#include "Math\Vector3.h"
#include "ParticleDef.h"

//---Forward Declarations---
class CXMLTreeNode;
//--------------------------

class CParticlesEmitterProperties 
{
public:
	//--- Init and End protocols-----------------------------------------
				CParticlesEmitterProperties		( void );
				CParticlesEmitterProperties		( const CXMLTreeNode &_Node );
				CParticlesEmitterProperties		( const CParticlesEmitterProperties *_pParticlesEmitterProperties );
				~CParticlesEmitterProperties	( void );
		
	//----Funcions principals -------------------------------------------
	
	//----Funcions privades ---------------------------------------------
	

	//----Properties ( get & Set )---------------------------------------
public:
	
	TEmitterAtTime	GetProperties		( void );

	void			SetPosition1		( const Vect3f &_Position )			{ m_vPosition1 = _Position; }
	Vect3f			GetPosition1		( void ) const						{ return m_vPosition1; }
	
	void			SetPosition2		( const Vect3f &_Position )			{ m_vPosition2 = _Position; }
	Vect3f			GetPosition2		( void ) const						{ return m_vPosition2; }

	void			SetVelocity			(  const Vect3f &_vVelocity )		{ m_vVelocity = _vVelocity; }
	Vect3f			GetVelocity			( void ) const						{ return m_vVelocity; }

	void			SetMinRate			( float _fMinEmitRate )				{ m_fMinEmitRate = _fMinEmitRate; }
	float			GetMinRate			( void ) const						{ return m_fMinEmitRate; }
	
	void			SetMaxRate			( float _fMaxEmitRate )				{ m_fMaxEmitRate = _fMaxEmitRate; }
	float			GetMaxRate			( void ) const						{ return m_fMaxEmitRate; }

	void			SetColor1			( const CColor &_Color )			{ m_Color1 = _Color; }
	CColor			GetColor1			( void ) const						{ return m_Color1; }

	void			SetColor2			( const CColor &_Color )			{ m_Color2 = _Color; }
	CColor			GetColor2			( void ) const						{ return m_Color2; }

	void			SetMinSize			( float _fMinSize )					{ m_fMinSize = _fMinSize; }
	float			GetMinSize			( void ) const						{ return m_fMinSize; }
	
	void			SetMaxSize			( float _fMaxSize )					{ m_fMaxSize = _fMaxSize; }
	float			GetMaxSize			( void ) const						{ return m_fMaxSize; }

	void			SetSpawnDirection1	( const Vect3f &_vSpawnDir1 )		{ m_vSpawnDir1 = _vSpawnDir1; }
	Vect3f			GetSpawnDirection1	( void ) const						{ return m_vSpawnDir1; }

	void			SetSpawnDirection2	( const Vect3f &_vSpawnDir2 )		{ m_vSpawnDir2 = _vSpawnDir2; }
	Vect3f			GetSpawnDirection2	( void ) const						{ return m_vSpawnDir2; }
									  
	void			SetNumExcess		( float _fNumNewPartsExcess )		{ m_fNumNewPartsExcess = _fNumNewPartsExcess; }
	float			GetNumExcess		( void ) const						{ return m_fNumNewPartsExcess; }

	void			SetTextureName		( const std::string &_sTexture )	{ m_sTexture = _sTexture; }
	std::string		GetTextureName		( void ) const						{ return m_sTexture; }

	void			SetIsAlive			( bool _bAlive )					{ m_bAlive = _bAlive; }
	bool			IsALive				( void ) const						{ return m_bAlive; }
	
	void			SetGravity			( bool _bUseGravity )				{ m_bGravity = _bUseGravity; }
	bool			GetGravity			( void ) const						{ return m_bGravity; }

	void			SetAngle			( float _fAngle )					{ m_fAngle = _fAngle; }
	float			GetAngle			( void ) const						{ return m_fAngle; }

	void			SetLifeTime1		( float _fLifeTime )				{ m_fLifeTime1 = _fLifeTime; }
	float			GetLifeTime1		( void ) const						{ return m_fLifeTime1; }
	
	void			SetLifeTime2		( float _fLifeTime )				{ m_fLifeTime2 = _fLifeTime; }
	float			GetLifeTime2		( void ) const						{ return m_fLifeTime2; }

	void			SetKeyTime			( float _fKeyTime )					{ m_fKeyTime = _fKeyTime; }
	float			GetKeyTime			( void ) const						{ return m_fKeyTime; }

	//----Members -------------------------------------------------------
protected:
	float			m_fLifeTime1;
	float			m_fLifeTime2;

	Vect3f			m_vPosition1;
	Vect3f			m_vPosition2;
	
	Vect3f			m_vVelocity;
	
	float			m_fAngle;

	float			m_fMinEmitRate;
	float			m_fMaxEmitRate;

	CColor			m_Color1;
	CColor			m_Color2;

	float			m_fMinSize;
	float			m_fMaxSize;

	Vect3f			m_vSpawnDir1;
	Vect3f			m_vSpawnDir2;

	float			m_fNumNewPartsExcess;
	
	bool			m_bAlive;				// Diu si l'emissor està viu 
	bool			m_bGravity;

	std::string		m_sTexture;

	float			m_fKeyTime;	// Necesari per saber el índex al qual pertany aquests paràmetres dins del vector del core
};

#endif __CLASS_PARTICLES_EMITTER_PROPERTIES_H__