//----------------------------------------------------------------------------------
// CInstanceMesh class
// Author: Marc Cuscullola
//
// Description:
// Crea una instancia de una core de un Static Mesh
//----------------------------------------------------------------------------------

#pragma once

#ifndef _INSTANCEMESH_H
#define _INSTANCEMESH_H

class CStaticMesh;
class CRenderManager;
class CEffectTechnique;

#include "RenderableObjects\RenderableObject.h"
#include <string>

class CInstanceMesh : public CRenderableObject
{
public:
	CInstanceMesh( const std::string &Name, const std::string &CoreName );
	virtual ~CInstanceMesh();

	void Render ( CRenderManager *RM );
	virtual void Render (CRenderManager *RM, CEffectTechnique* technique);

private:
	CStaticMesh		*m_StaticMesh;
};

#endif
