//----------------------------------------------------------------------------------
// CAnimatedCoreModel class
// Author: Marc Cuscullola
//
// Description:
// Crea la core de un modelo animado a partir de un fichero XML
//----------------------------------------------------------------------------------

#pragma once

#ifndef _ANIMATED_CORE_MODEL_H
#define _ANIMATED_CORE_MODEL_H

#include <string>
#include <vector>
#include "Utils\Named.h"
#include "Utils\Types.h"

class CRenderableVertexs;
class CalHardwareModel;
class CalModel;
class CalCoreModel;
class CTexture;

class CAnimatedCoreModel : public CNamed
{
public:
	//--- Init and End protocols------------------------------------------
							CAnimatedCoreModel		( void );
	virtual					~CAnimatedCoreModel		( void );

	//----Main Methods -----------------------------------------------------
	void					Load					( const std::string &Path, const std::string &XMLFilename );
	
	//---- Methods ---------------------------------------------------------
	bool					LoadVertexBuffer		( CalModel *_pModel );
	CalHardwareModel*		GetCalHardwareModel		( void ) const;
	CRenderableVertexs*		GetRenderableVertexs	( void ) const;
	
private:
	bool					LoadMesh				( const std::string &_Filename );
	bool					LoadSkeleton			( const std::string &_Filename );
	bool					LoadAnimation			( const std::string &_Name, const std::string &_Filename );
	void					LoadTextures			( void );
	void					ClearTextureVector		( void );

	//----Properties --------------------------------------------------------
public:
	CalCoreModel*			GetCoreModel			( void ) const		{ return m_CalCoreModel; }
	size_t					GetNumTextures			( void ) const		{ return m_TextureFilenameVector.size(); }
	const std::string&		GetTextureName			( size_t id )		{ return m_TextureFilenameVector[id]; }
	CTexture*				GetTextureById			( uint16 id )		{ return m_TextureVector[id]; }

	//----Members -----------------------------------------------------------
private:
	CalCoreModel*				m_CalCoreModel;
	CalHardwareModel*			m_CalHardwareModel;
	CRenderableVertexs*			m_RenderableVertexs;

	std::string					m_Name;
	std::vector<std::string>	m_TextureFilenameVector;
	std::vector<CTexture*>		m_TextureVector;
	std::string					m_Path;
	
	uint32						m_NumVtxs;
	uint32						m_NumFaces;
};

#endif
