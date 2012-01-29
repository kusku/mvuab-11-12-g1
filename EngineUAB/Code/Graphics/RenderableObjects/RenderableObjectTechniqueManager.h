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
	void			Load			( const std::string &FileName );

	std::string		GetRenderableObjectTechniqueNameByVertexType		( uint32 VertexType );
	CTemplatedVectorMapManager<CPoolRenderableObjectTechnique> GetPoolRenderableObjectTechniques () const
	{
		return m_PoolRenderableObjectTechniques; 
	}

private:
	CTemplatedVectorMapManager<CPoolRenderableObjectTechnique>		m_PoolRenderableObjectTechniques;

	void InsertRendrableObjectTechnique (const std::string &ROTName, const std::string &TechniqueName);
};

#endif
