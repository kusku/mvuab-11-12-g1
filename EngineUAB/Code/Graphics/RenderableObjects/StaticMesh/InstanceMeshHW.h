
#pragma once

#ifndef _INSTANCE_MESH_HW_H
#define _INSTANCE_MESH_HW_H

class CStaticMesh;
class CRenderManager;
class CEffectTechnique;
class CXMLTreeNode;
class CObject3D;
struct TINSTANCE_VERTEX;

#include <string>
#include <map>
#include <d3dx9.h>

#include "Utils\Types.h"
#include "Math\Vector3.h"

class CObject3D;

class CInstanceMeshHW
{

private:
	void UpdateBuffer();

	LPDIRECT3DVERTEXBUFFER9		m_InstanceBuffer;
	TINSTANCE_VERTEX*			m_InstanceVertex;

	bool						m_Initialize;

	uint32						m_VisibleObjects;

	CStaticMesh*				m_StaticMesh;

	typedef std::map<std::string, CObject3D*> ObjectMap;
	typedef ObjectMap::iterator ObjectMapIt;
	typedef ObjectMap::const_iterator ObjectMapItConst;

	ObjectMap					m_ObjectMap;

public:
	CInstanceMeshHW(const std::string& coreName);
	virtual ~CInstanceMeshHW();

	bool AddHWInstance(CXMLTreeNode &Node);

	bool BuildInstanceBuffer();

	void Render ( CRenderManager *RM );

	CObject3D* GetInstance(const std::string& name);

	ObjectMap&	GetObjectMap()
	{
		return m_ObjectMap;
	}

	void CrearPhysic ( const std::string& _Name, const std::string& typePhysic, const Vect3f& position );
	//void CreateASEMesh	 ( const std::string &_Filename, const std::string &_Name  );
};

#endif
