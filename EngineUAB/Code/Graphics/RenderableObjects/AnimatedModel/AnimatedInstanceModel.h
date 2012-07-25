//----------------------------------------------------------------------------------
// CAnimatedInstanceModel class
// Author: Marc Cuscullola
//
// Description:
// Crea y gestiona una instancia de una core de un modelo animado.
//----------------------------------------------------------------------------------

#pragma once

#ifndef _ANIMATED_INSTANCE_MODEL_H
#define _ANIMATED_INSTANCE_MODEL_H

class CTexture;
class CRenderManager;
class CAnimatedCoreModel;
class CEffectTechnique;
class CXMLTreeNode;

#include "cal3d\cal3d.h"
#include "Utils\BaseUtils.h"

#include "RenderableObjects/RenderableObject.h"

#include <vector>
#include <d3dx9.h>
class CAnimatedInstanceModel : public CRenderableObject
{
public:
	//--- Init and End protocols------------------------------------------
				CAnimatedInstanceModel	( void );
	virtual		~CAnimatedInstanceModel ( void );

	//----Main Methods -----------------------------------------------------
	void		ReadDataXML				( CXMLTreeNode &_Node );
	void		Initialize				( CAnimatedCoreModel *_pAnimatedCoreModel );

	void		Update					( float elapsedTime );

	void		Render					( CRenderManager *_RM );
	void		RenderModelBySoftware	( CRenderManager *_RM );
	void		RenderModelByHardware	( CRenderManager *_RM );

	void		Destroy					( void );

	//---- Methods ---------------------------------------------------------
	void		ExecuteAction			( uint32 _Id, float _Time );
	void		RemoveAction			( uint32 _Id );
	void		BlendCycle				( uint32 _Id, float _Time );
	void		ClearCycle				( uint32 _Id, float _Time );
	
	float		GetCurrentAnimationDuration	( const std::string &_Name );

	//----Properties --------------------------------------------------------
	bool		GetCreatePhysics		( void ) const		{ return m_CreatePhysics; }
	bool		GetCreateBehaviour		( void ) const		{ return m_CreateLogicBehaviour; }

	CAnimatedCoreModel*	GetAnimatedCoreModel ( void ) const	{ return m_AnimatedCoreModel; }

	bool		GetBonePosition			( const std::string & _Bone, Vect3f &_Position );
	bool		GetBoneRotation			( const std::string & _Bone, Vect4f & _Rotation );

	//----Members -------------------------------------------------------	
private:
	CalModel*					m_CalModel;
	CAnimatedCoreModel*			m_AnimatedCoreModel;

	bool						m_CreatePhysics;
	bool						m_CreateLogicBehaviour;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
};

#endif
