#pragma once

#ifndef __CLASS_PARTICLES_EMITTER_CORE_MANAGER_H__
#define __CLASS_PARTICLES_EMITTER_CORE_MANAGER_H__

#include <string>
#include "Math\Color.h"
#include "Math\Vector3.h"
#include "ParticleDef.h"
#include "Utils\Named.h"
#include "ParticlesEmitterProperties.h"
#include "Utils\TemplatedVectorMapManager.h"

#define MAX_TIME_TO_SEARCH 10000.f


//---Forward Declarations---
class CXMLTreeNode;
//--------------------------

class CParticlesEmitterCore : public CNamed, public CTemplatedVectorMapManager<CParticlesEmitterProperties>
{
public:
	//--- Init and End protocols-----------------------------------------
				CParticlesEmitterCore	( const CXMLTreeNode &_Node );
				~CParticlesEmitterCore	( void );
	
	//----Funcions principals -------------------------------------------
	void		Init					( void );
	float		SearchNextMenorTime		( const CXMLTreeNode &_MainNode, float _MinimTime );

	//----Funcions publiques --------------------------------------------
	std::string ConvertFloatToString	( float _Value );

	//----Funcions privades ---------------------------------------------
private:	
	void		Destroy					( void );

	void		InitXMLTimeZero 		( const CXMLTreeNode &_Node );
	void		InitXMLOtherTimes 		( const CXMLTreeNode &_Node );
	
	float		CutAndRoundNumberToNDecimals (float dValue, int nDec); 

	//----Properties ( get & Set )---------------------------------------
public:
	CParticlesEmitterProperties * GetPreviousCore ( const std::string &_Time );	// Permet cercar l'anterior core abans del temps passat

	TEmitterAtTime GetPropertiesAtTime		( float _Time );
	TEmitterAtTime GetPropertiesAtIndex		( size_t _Index );

	void		SetPosition1AtTime			( float _Time, const Vect3f &_Position )	{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetPosition1( _Position );
																						}
	Vect3f		GetPosition1AtTime			( float _Time )								{
																							std::string l_strTime = ConvertFloatToString ( _Time );
																							return GetResource( l_strTime  )->GetPosition1();
																						}

	void		SetPosition2AtTime			( float _Time, const Vect3f &_Position )	{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetPosition2( _Position );
																						}
	Vect3f		GetPosition2AtTime			( float _Time ) 							{ 
																							std::string l_strTime = ConvertFloatToString ( _Time );
																							return GetResource( l_strTime  )->GetPosition2();
																						}

	void		SetVelocityAtTime			( float _Time, const Vect3f &_vVelocity )	{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetVelocity( _vVelocity );
																						}
	Vect3f		GetVelocityAtTime			( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetVelocity();
																						}
	void		SetMinRateAtTime			( float _Time, const float _fMinEmitRate )	{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetMinRate( _fMinEmitRate );
																						}
	float		GetMinRateAtTime			( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetMinRate();
																						}
	void		SetMaxRateAtTime			( float _Time, const float _fMaxEmitRate )	{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetMinRate( _fMaxEmitRate );
																						}
	float		GetMaxRateAtTime			( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetMaxRate();
																						}

	void		SetColor1AtTime				( float _Time, const CColor &_Color )		{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetColor1( _Color );
																						}
	CColor		GetColor1AtTime				( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetColor1(); 
																						}
	void		SetColor2AtTime				( float _Time, const CColor &_Color )		{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetColor1( _Color );
																						}
	CColor		GetColor2AtTime				( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetColor2(); 
																						}

	void		SetMinSizeAtTime			( float _Time, float _fMinSize )			{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetMinSize( _fMinSize ); 
																						}

	float		GetMinSizeAtTime			( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetMinSize();  
																						}

	void		SetMaxSizeAtTime			( float _Time, float _fMaxSize )			{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetMaxSize( _fMaxSize ); 
																						}

	float		GetMaxSizeAtTime			( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetMaxSize();  
																						}

	void		SetSpawnDirection1AtTime	( float _Time, const Vect3f &_vSpawnDir1 )	{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetSpawnDirection1( _vSpawnDir1 ); 
																						}

	Vect3f		GetSpawnDirection1AtTime	( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetSpawnDirection1();  
																						}

	void		SetSpawnDirection2AtTime	( float _Time, const Vect3f &_vSpawnDir2 )	{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetSpawnDirection2( _vSpawnDir2 ); 
																						}

	Vect3f		GetSpawnDirection2AtTime	( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetSpawnDirection2();  
																						}
								  
	void		SetNumExcessAtTime			( float _Time, float _fNumNewPartsExcess )	{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetNumExcess( _fNumNewPartsExcess ); 
																						}

	float		GetNumExcessAtTime			( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetNumExcess();  
																						}

	void		SetTextureNameAtTime		( float _Time, const std::string &_sTexture )	{ 
																								std::string l_Time = ConvertFloatToString (_Time );
																								GetResource( l_Time )->SetTextureName( _sTexture ); 
																							}

	std::string	GetTextureNameAtTime		( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetTextureName();  
																						}

	void		SetIsAliveAtTime			( float _Time, bool _bAlive )				{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetIsAlive( _bAlive ); 
																						}

	bool		IsALiveAtTime				( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->IsALive();  
																						}

	void		SetLifeTime1AtTime			( float _Time, float _fLifeTime1 )			{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetLifeTime1( _fLifeTime1 );
																						}

	float		GetLifeTime1AtTime			( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetLifeTime1();  
																						}

	void		SetLifeTime2AtTime			( float _Time, float _fLifeTime2 )			{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetLifeTime2( _fLifeTime2 );
																						}
	float		GetLifeTime2AtTime			( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetLifeTime2();  
																						}

	void		SetGravityAtTime			( float _Time, bool _UsebGravity )			{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetGravity( _UsebGravity ); 
																						}

	bool		GetGravityAtTime			( float _Time )								{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetGravity();  
																						}
	
	void		SetAngleAtTime				( float _Time, float _fAngle )				{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							GetResource( l_Time )->SetAngle( _fAngle ); 
																						}
	float		GetAngleAtTime				( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetAngle();  
																						}

	void		SetKeyTime					( float _Time )								{ 
																							m_fKeytime = _Time;
																						}
	float		GetKeyTime					( float _Time ) 							{ 
																							std::string l_Time = ConvertFloatToString (_Time );
																							return GetResource( l_Time )->GetKeyTime();  
																						}

	//----Members -------------------------------------------------------
private:
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

	bool			m_bGravity;

	float			m_fNumNewPartsExcess;
	
	bool			m_bAlive;				// Diu si l'emissor està viu 

	std::string		m_sTexture;

	float			m_fKeytime;				// La key del mapa de cores que determina tots els paràmetres de rang de dalt
};

#endif __CLASS_PARTICLES_EMITTER_CORE_MANAGER_H__
