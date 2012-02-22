#define __DONT_INCLUDE_MEM_LEAKS__
#include "base.h"

#include "PhysicCookingMesh.h"

//---PhysX includes----
#undef min
#undef max
#include "NxPhysics.h"
#include "NxCooking.h"
#include "PhysicUserAllocator.h"
#include "PhysXLoader.h"
#include "PhysicStream.h"
//--------------------------

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::Init (NxPhysicsSDK* physicSDK, CPhysicUserAllocator* myAllocator)
{

	m_pPhysicSDK = physicSDK;
	m_pMyAllocator = new CPhysicUserAllocator();
	assert(m_pPhysicSDK && m_pMyAllocator);
	m_bIsOk = (m_pMyAllocator != NULL && m_pPhysicSDK != NULL);	
	if	(m_bIsOk)
	{
		m_pCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
		m_bIsOk = (m_pCooking != NULL);
		if (m_bIsOk)
		{
			m_bIsOk = m_pCooking->NxInitCooking(m_pMyAllocator, NULL);
		}
	}

	if (!m_bIsOk)
	{
		Release();
	}

	return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CPhysicCookingMesh::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CPhysicCookingMesh::Release ()
{
	//---Delete TriangleMeshes-----------
	VecMeshes::iterator it(m_TriangleMeshes.begin());
	VecMeshes::iterator endIt(m_TriangleMeshes.end());
	while (it!=endIt)
	{
		NxTriangleMesh* mesh = it->second;
		if( mesh != NULL && mesh->getReferenceCount() == 0 )
		{
			m_pPhysicSDK->releaseTriangleMesh(*mesh);
		}
		++it;
	}
	m_TriangleMeshes.clear();
	//------------------------------------

	//Close Cooking
	if  (m_pCooking != NULL)
	{
		m_pCooking->NxCloseCooking();
	}
	CHECKED_DELETE(m_pMyAllocator);
}


NxTriangleMesh* CPhysicCookingMesh::GetPhysicMesh(const std::string& name)
{
	VecMeshes::iterator it = m_TriangleMeshes.find(name);
	if(it != m_TriangleMeshes.end() )
	{
		return it->second;
	}
	return NULL;
}

//----------------------------------------------------------------------------
// PhysicMesh created  from a Bin file. This file has been saved previously
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CreatePhysicMesh (	const std::string& bin_Filename, const std::string& nameMesh )
{
	bool isOk = false;
	VecMeshes::iterator it = m_TriangleMeshes.find(nameMesh);
	if( it == m_TriangleMeshes.end() )
	{
		NxTriangleMesh* triangleMesh = NULL;
		triangleMesh = m_pPhysicSDK->createTriangleMesh(CPhysicUserStream(bin_Filename.c_str(), true));
		isOk = (triangleMesh!=NULL);
		if( isOk )
		{
			m_TriangleMeshes.insert( std::pair<std::string, NxTriangleMesh*>(nameMesh, triangleMesh));
		}
	}

	return isOk;
}


//----------------------------------------------------------------------------
// Creating a PhysicMesh from a buffer
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CreatePhysicMesh		(	const std::vector<Vect3f>& vertices, const std::vector<uint32>& faces, 
																						 const std::string &nameMesh )
{
	bool isOk = false;
	std::map<std::string,NxTriangleMesh*>::iterator it = m_TriangleMeshes.find(nameMesh);
	if( it == m_TriangleMeshes.end() )
	{
		// Build physical model
		NxTriangleMeshDesc triangleMeshDesc;
		triangleMeshDesc.numVertices					= (NxU32) vertices.size();
		triangleMeshDesc.numTriangles					= (NxU32) faces.size()/3;
		triangleMeshDesc.pointStrideBytes			= sizeof(Vect3f);
		triangleMeshDesc.triangleStrideBytes	= 3*sizeof(uint32);
		triangleMeshDesc.points								= &vertices[0].x;
		triangleMeshDesc.triangles						= &faces[0];
		triangleMeshDesc.flags								= 0;

		assert(m_pCooking); //by if the flies...

		CPhysicMemoryWriteBuffer buf;
		if (m_pCooking->NxCookTriangleMesh(triangleMeshDesc,buf))
		{
			NxTriangleMesh* triangleMesh = NULL;
			triangleMesh = m_pPhysicSDK->createTriangleMesh(CPhysicMemoryReadBuffer(buf.data));
			isOk = (triangleMesh != NULL);
			if (isOk)
			{
				m_TriangleMeshes.insert( std::pair<std::string, NxTriangleMesh*>(nameMesh, triangleMesh));
			}
		}	

	}

	return isOk;	
}

//----------------------------------------------------------------------------
// Save a PhysicMesh in a bin file
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::SavePhysicMesh (	const std::vector<Vect3f>& vertices, const std::vector<uint32>& faces,
																				 const std::string &binFilename )
{
	// Build physical model
	NxTriangleMeshDesc triangleMeshDesc;
	triangleMeshDesc.numVertices					= (NxU32)vertices.size();
	triangleMeshDesc.numTriangles					= (NxU32)faces.size()/3;
	triangleMeshDesc.pointStrideBytes		= sizeof(Vect3f);
	triangleMeshDesc.triangleStrideBytes	= 3*sizeof(uint32);
	triangleMeshDesc.points								= &vertices[0].x;
	triangleMeshDesc.triangles						= &faces[0];
	triangleMeshDesc.flags								= 0;

	assert(m_pCooking);
	bool isOk = false;

	isOk = m_pCooking->NxCookTriangleMesh(triangleMeshDesc, CPhysicUserStream(binFilename.c_str(), false));

	return isOk;
}

//----------------------------------------------------------------------------
// Create a Cloth mesh
//----------------------------------------------------------------------------
bool CPhysicCookingMesh::CookClothMesh(const NxClothMeshDesc& desc, NxStream& stream)
{
	assert(m_pCooking);
	bool isOk = false;
	isOk = m_pCooking->NxCookClothMesh(desc,stream);
	return isOk;
}

