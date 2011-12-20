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

#include "cal3d\cal3d.h"
#include <string>
#include <vector>

class CAnimatedCoreModel
{
public:
	CAnimatedCoreModel();
	virtual ~CAnimatedCoreModel();

	void			Load		( const std::string &Path );

	CalCoreModel*			GetCoreModel		() const		{ return m_CalCoreModel; }
	size_t					GetNumTextures		() const		{ return m_TextureFilenameVector.size(); }
	const std::string&		GetTextureName		( size_t id )	{ return m_TextureFilenameVector[id]; }

private:
	bool			LoadMesh		();
	bool			LoadSkeleton	();
	bool			LoadAnimation	( const std::string &Name, const std::string &Filename );


	CalCoreModel*				m_CalCoreModel;

	std::string					m_Name;
	std::string					m_MeshFilename;
	std::string					m_SkeletonFilename;
	std::vector<std::string>	m_TextureFilenameVector;
	std::string					m_Path;
};

#endif
