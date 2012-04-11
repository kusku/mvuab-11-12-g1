#pragma once

#ifndef __BILLBOARD_ANIMATION_H__
#define __BILLBOARD_ANIMATION_H__

#include <vector>

#include "Billboard.h"

#include "Math\Color.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Utils\Named.h"

//---Forward Declarations---
class CRenderManager;
class CCamera;
class CTexture;
class CXMLTreeNode;
//--------------------------


class CBillboardAnimation : public CBillboard, public CNamed
{
public:
	//--- Init and End protocols------------------------------------------
							CBillboardAnimation		( float _Time, const std::vector<CTexture *> & _vBillboardAnimation );
							CBillboardAnimation		( CXMLTreeNode &_Node );
							CBillboardAnimation		( const CBillboardAnimation *_BillboardAnimation );
							~CBillboardAnimation	( void );	

	//----Funcions principals -------------------------------------------
	void					Render					( CRenderManager &_RM, const CColor &_Color = colWHITE );
	void					Update					( float _ElapsedTime );
	
	//----Funcions privades ------------------------------------------------------
private:
	void					Destroy					( void );
	
	//----Properties ( get & Set )---------------------------------------
public:
	void					SetTimePerImage			( float _TimePerImage )							{ m_fTimePerImage = _TimePerImage; }
	float					GetTimePerImage			( void ) const									{ return m_fTimePerImage; }

	void					SetUpdatedTime			( float _CurrentTime )							{ m_fCurrentTime = _CurrentTime; }
	float					GetUpdatedTime			( void ) const									{ return m_fCurrentTime; }

	void					SetImageIndex			( uint16 _uiImage )								{ m_uiImage = _uiImage; }
	uint16					GetImageIndex			( void ) const									{ return m_uiImage; }

	void					SetColor				( Vect4f _vColor )								{ m_vColor = _vColor; }
	Vect4f					GetColor				( void ) const									{ return m_vColor; }

	void					SetSize					( float _fSize )								{ m_fSize = _fSize; }
	float					GetSize					( void ) const									{ return m_fSize; }

	void					SetTotalTextures		( uint16 _uiNumTextures							) { m_uiNumTextures = _uiNumTextures; }
	uint16					GetTotalTextures		( void ) const									{ return m_uiNumTextures; }

	void					SetLoop					( bool _bLoop )									{ m_bLoop = _bLoop; }
	bool					GetLoop					( void ) const									{ return m_bLoop; }

	void					SetTexturesVector		( std::vector<CTexture *>	 _vTextures )		{ m_vTextures = _vTextures; }
	std::vector<CTexture *>	GetTexturesVector		( void ) const									{ return m_vTextures; }
	
private:
	
	float						m_fTimePerImage;	// Variable per gestionar el temps i que tria quina imatge mostrar
	float						m_fCurrentTime;		// Variable per gestionar el temps i que tria quina imatge mostrar
	
	uint16						m_uiImage;			// Imatge en curs 

	Vect4f						m_vColor;
	float						m_fSize;
	uint16						m_uiNumTextures;
	bool						m_bLoop;

	std::vector<CTexture *>		m_vTextures;		// Vector que emmagatzema un punter de la textura enmmagatzemada en el manager
};

#endif __BILLBOARD_ANIMATION_H__