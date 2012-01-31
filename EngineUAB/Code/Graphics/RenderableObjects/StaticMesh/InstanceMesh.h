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
class CXMLTreeNode;

#include "RenderableObjects\RenderableObject.h"
#include <string>

class CInstanceMesh : public CRenderableObject
{
public:
	CInstanceMesh( const std::string &Name, const std::string &CoreName );
	CInstanceMesh( CXMLTreeNode &Node );
	virtual ~CInstanceMesh();

	//void Render ( CRenderManager *RM );
	void Render ( CRenderManager *RM );

private:
	CStaticMesh		*m_StaticMesh;
};

#endif
