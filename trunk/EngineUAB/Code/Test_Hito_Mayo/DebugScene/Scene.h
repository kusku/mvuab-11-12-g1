#pragma once

#ifndef __CLASS_SCENE_H__
#define __CLASS_SCENE_H__

#include <string>
#include <vector>

#include "Math\Color.h"
#include "Math\Vector3.h"
#include "PhysicsDefs.h"

//---Forward Declarations---
class CRenderManager;

class CPhysicUserData;
class CPhysicActor;
class CPhysicTriggerReport;
//--------------------------

class CScene
{
public:
	//--- Init and End protocols-----------------------------------------------------
					CScene		( void );
	virtual			~CScene		( void );

	void			Done					( void );
	virtual bool	IsOK					( void ) const { return m_bIsOk; };
	bool			Init					( void );
	void			Release					( void );

	//----Main Methods----------------------------------------------------------------
	void			Update					( float ElapsedTime );
	void			Render					( CRenderManager *_RM );
	void			RenderScene				( void );
	
	//----Methods --------------------------------------------------------------------
	void			CreateScene				( void );												// Crea un scenari aleatori
	void			DibuixarEixosPla		( void );

	//----Test Functions--------------------------------------------------------------
	//void			ExisteixColisio			( void );
	

	// ----- Escenari
	void			CreateGrid				( void );
	void			CreateStairs			( void );
	void			CreateWalls				( void );
	void			CreateASEMesh			( const std::string &_Filename, const std::string &_Name );

	void			CreatePlane				( const Vect3f &_Pos, float _Distance = 0.f, float _Density = 0.f, const Vect3f &_LocalPos = Vect3f(0.f,0.f,0.f), bool _StaticShape = false, 
												ECollisionGroup _ColisionGroup = ::ECG_ESCENARI, const CColor &_Color = colWHITE );
	
	CPhysicActor*	CreateBox				( const Vect3f &_Pos, const Vect3f &_BoxDim, const Vect3f &_LocalPos = Vect3f(0.f,0.f,0.f), float _Density = 0.f, bool _StaticShape = false, 
												ECollisionGroup _ColisionGroup = ::ECG_ESCENARI, const CColor &_Color = colWHITE );
	
	void			CreateSphere			( const Vect3f &_Pos, float _Radius,  const Vect3f &_LocalPos = Vect3f(0.f,0.f,0.f), float _Density = 0.f, const Vect3f &_VelocitatInicial = Vect3f (0.f,0.f,0.f),														bool _StaticShape = false, ECollisionGroup _ColisionGroup = ::ECG_ESCENARI, const CColor &_Color = colWHITE );

	//----Methods --------------------------------------------------------------------
	
private:
	bool							m_bIsOk;

	std::vector<CPhysicActor*>		m_ActorVector;		

	CPhysicActor*					m_pPlaneActor;
	CPhysicUserData*				m_pPhysicUserDataPla;
};

#endif __CLASS_SCENE_H__