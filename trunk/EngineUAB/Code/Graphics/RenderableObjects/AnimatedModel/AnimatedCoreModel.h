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
#include <map>
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
	CAnimatedCoreModel			();
	virtual ~CAnimatedCoreModel	();

	//----Main Methods -----------------------------------------------------
	void					Load					( const std::string &Path, const std::string &XMLFilename );
	
	//---- Methods ---------------------------------------------------------
	bool					LoadVertexBuffer		( CalModel *_pModel );
	CalHardwareModel*		GetCalHardwareModel		() const;
	CRenderableVertexs*		GetRenderableVertexs	() const;
	
	//----Properties --------------------------------------------------------
	CalCoreModel*			GetCoreModel			() const			{ return m_CalCoreModel; }
	size_t					GetNumTextures			() const			{ return m_TextureFilenameVector.size(); }
	const std::string&		GetTextureName			( size_t id )		{ return m_TextureFilenameVector[id]; }
	CTexture*				GetTextureById			( uint16 id )		{ return m_TextureVector[id]; }
	uint16					GetAnimationIdByName	( const std::string &_Name ) { return m_AnimationsMap[_Name]; }

private:
	bool					LoadMesh				( const std::string &_Filename );
	bool					LoadSkeleton			( const std::string &_Filename );
	bool					LoadAnimation			( const std::string &_Name, const std::string &_Filename );
	void					LoadTextures			();
	void					ClearTextureVector		();

	//----Members -----------------------------------------------------------
private:
	CalCoreModel*				m_CalCoreModel;
	CalHardwareModel*			m_CalHardwareModel;
	CRenderableVertexs*			m_RenderableVertexs;

	std::string					m_Name;
	std::vector<std::string>	m_TextureFilenameVector;
	std::vector<CTexture*>		m_TextureVector;
	std::string					m_Path;
	
	std::map<std::string, uint16> m_AnimationsMap;
	
	uint32						m_NumVtxs;
	uint32						m_NumFaces;
};

#endif
