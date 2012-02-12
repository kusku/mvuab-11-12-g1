#pragma once

#ifndef _RENDERABLE_OBJECT_TECHNIQUE_MANAGER_H
#define _RENDERABLE_OBJECT_TECHNIQUE_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "RenderableObjectTechnique.h"
#include "PoolRenderableObjectTechnique.h"
#include "Utils\Types.h"
#include <string>

class CRenderableObjectTechniqueManager : public CTemplatedVectorMapManager<CRenderableObjectTechnique>
{
public:
	CRenderableObjectTechniqueManager();
	virtual ~CRenderableObjectTechniqueManager();

	void			Destroy			();
	bool			Load			( const std::string &FileName );
	bool			Reload			();

	std::string		GetRenderableObjectTechniqueNameByVertexType		( uint32 VertexType );
	CTemplatedVectorMapManager<CPoolRenderableObjectTechnique> & GetPoolRenderableObjectTechniques ()
	{
		return m_PoolRenderableObjectTechniques; 
	}

private:
	CTemplatedVectorMapManager<CPoolRenderableObjectTechnique>		m_PoolRenderableObjectTechniques;
	std::string														m_FileName;

	void InsertRenderableObjectTechnique (const std::string &ROTName, const std::string &TechniqueName);
	bool LoadFile ();
};

#endif
