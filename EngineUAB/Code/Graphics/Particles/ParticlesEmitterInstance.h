#pragma once

#ifndef __CLASS_PARTICLES_EMITTER_INSTANCE_MANAGER_H__
#define __CLASS_PARTICLES_EMITTER_INSTANCE_MANAGER_H__

#include <string>
#include "Utils\Named.h"
#include "Utils\RecyclingArray.h"
#include "Utils\Random.h"
#include "Math\Color.h"
#include "Math\Vector3.h"
#include "Particle.h"
#include "ParticlesEmitterProperties.h"

//---Forward Declarations---
class CXMLTreeNode;
class CParticlesEmitterCore;
//--------------------------

//#define	NUMPARTICLES	2000

class CParticlesEmitterInstance : public CNamed, public CParticlesEmitterProperties
{
public:
	//--- Init and End protocols-----------------------------------------
				//CParticlesEmitterInstance	( void );
				CParticlesEmitterInstance	( CXMLTreeNode &_Node );
				~CParticlesEmitterInstance	( void );
		
	//----Funcions principals -------------------------------------------
	void		Init				( void );
	void		Render				( CRenderManager &_RM, const CColor &_Color = colWHITE );
	void		Update				( float _ElapsedTime );
	
	//----Funcions privades ---------------------------------------------
private:
	void		Destroy						( void );
	void		CreateParticles				( float _ElapsedTime );		// Crea N partícules noves segons l'espai de l'array de partícules
	void		CreateParticle				( void );					// Crea 1 sola partícula amb els parametres random i d'inici habituals segons el seu core

	CParticlesEmitterProperties* SearchTimeBasedAge ( float &_CurrentLife ); //, int &_CurrentIndex )
	
	float		SearchTimeBasedAge			( float _CurrentLife, int &_CurrentIndex );
	void		UpdateLocalParameters		( float _CurrentLife );



	bool		UpdateValuesByTime			( float _Time );
	void		UpdateValuesByIndex			( size_t _Index );
	
	void		AssignInitialParameters	( CParticle *l_Particle );
	void		UpdateParticlesParameters	( CParticle *l_Particle );
	
	Vect3f		RandomVector				( const Vect3f &_vect1, const Vect3f &_vect2 );
	CColor		RandomColor					( const CColor &_Color1, const CColor &_Color2 );
	float		RandomFloat					( const float &_Value1, const float &_Value2 );


	// Helper function to stuff a FLOAT into a DWORD argument
	//inline DWORD32 FtoDW				( float f ) { return *((DWORD*)&f); }


	//----Properties ( get & Set )---------------------------------------
public:
	bool		IsInstanceAlive		( void )			{ return m_bInstanceAlive; }

	//----Members -------------------------------------------------------
private:
	
	bool						m_bInstanceAlive;				// Diu si l'emissor està viu 
	CTexture *					m_Texture;
	CParticlesEmitterCore *		m_pCore;

	CRecyclingArray<CParticle>	m_Particles;

	CRandom						m_Randomize;
};

#endif __CLASS_PARTICLES_EMITTER_INSTANCE_MANAGER_H__
