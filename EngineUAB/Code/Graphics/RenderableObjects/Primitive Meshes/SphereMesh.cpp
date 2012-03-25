
#include <sstream>

#include "SphereMesh.h"
#include "Math\Vector3.h"
#include "Vertexs\RenderableVertexs.h"
#include "Vertexs\IndexedVertexs.h"
#include "Core.h"
#include "RenderManager.h"
#include "Effects\EffectTechnique.h"

CSphereMesh::CSphereMesh(uint32 recursionLvl, const CColor& color)
	: m_NumVertexs(0)
	, m_NumFaces(0)
	, m_RenderableObjecTechniqueName("")
	, m_MeshName("")
	, m_RV(NULL)
	, m_RecursionLvl(recursionLvl)
	, m_Color(color)
{
	BuildSphere();
}

CSphereMesh::~CSphereMesh()
{
	CHECKED_DELETE(m_RV);
}

void CSphereMesh::CreateSphere(uint32 recursionLvl, const CColor& color)
{
	m_RecursionLvl = recursionLvl;
	m_Color = color;

	BuildSphere();
}

void CSphereMesh::BuildSphere()
{
	//Remove any RV
	CHECKED_DELETE(m_RV);

	//Clear Temp Vectors and Maps
	m_IdxBuffTemp.clear();
	m_VtxBuffTemp.clear();
	m_VtxMapTemp.clear();

	//Get Base Size
	float t = (1.0f + mathUtils::Sqrt<float>(5.0f)) / 2.0f;

	TCOLORED_VERTEX vtx;
	vtx.color = m_Color.GetUint32Argb();
	
	//////////////////////////////////////////
	//Create 12 vertices of a icosahedron
	//////////////////////////////////////////

	//Vertice 1
	vtx.x = -1;
	vtx.y = t;
	vtx.z = 0;
	AddVtx(vtx);

	//Vertice 2
	vtx.x = 1;
	vtx.y = t;
	vtx.z = 0;
	AddVtx(vtx);

	//Vertice 3
	vtx.x = -1;
	vtx.y = -t;
	vtx.z = 0;
	AddVtx(vtx);
	
	//Vertice 4
	vtx.x = 1;
	vtx.y = -t;
	vtx.z = 0;
	AddVtx(vtx);
	
	//Vertice 5
	vtx.x = 0;
	vtx.y = -1;
	vtx.z = t;
	AddVtx(vtx);

	//Vertice 6
	vtx.x = 0;
	vtx.y = 1;
	vtx.z = t;
	AddVtx(vtx);

    //Vertice 7
	vtx.x = 0;
	vtx.y = -1;
	vtx.z = -t;
	AddVtx(vtx);

    //Vertice 8
	vtx.x = 0;
	vtx.y = 1;
	vtx.z = -t;
	AddVtx(vtx);

	//Vertice 9
	vtx.x = t;
	vtx.y = 0;
	vtx.z = -1;
	AddVtx(vtx);
	
	//Vertice 10
	vtx.x = t;
	vtx.y = 0;
	vtx.z = 1;
	AddVtx(vtx);

	//Vertice 11
	vtx.x = -t;
	vtx.y = 0;
	vtx.z = -1;
	AddVtx(vtx);

	//Vertice 12
	vtx.x = -t;
	vtx.y = 0;
	vtx.z = 1;
	AddVtx(vtx);
	
	//////////////////////////
	//5 faces around point 0
	//////////////////////////

	//Face 1
	m_IdxBuffTemp.push_back(0);
	m_IdxBuffTemp.push_back(11);
	m_IdxBuffTemp.push_back(5);
	
	//Face 2
	m_IdxBuffTemp.push_back(0);
	m_IdxBuffTemp.push_back(5);
	m_IdxBuffTemp.push_back(1);
	
	//Face 3
	m_IdxBuffTemp.push_back(0);
	m_IdxBuffTemp.push_back(1);
	m_IdxBuffTemp.push_back(7);
	
	//Face 4
	m_IdxBuffTemp.push_back(0);
	m_IdxBuffTemp.push_back(7);
	m_IdxBuffTemp.push_back(10);
	
	//Face 5
	m_IdxBuffTemp.push_back(0);
	m_IdxBuffTemp.push_back(10);
	m_IdxBuffTemp.push_back(11);
	
	//////////////////////////
	//5 adjacent faces 
	//////////////////////////
	
	//Face 1
	m_IdxBuffTemp.push_back(1);
	m_IdxBuffTemp.push_back(5);
	m_IdxBuffTemp.push_back(9);
	
	//Face 2
	m_IdxBuffTemp.push_back(5);
	m_IdxBuffTemp.push_back(11);
	m_IdxBuffTemp.push_back(4);
	
	//Face 3
	m_IdxBuffTemp.push_back(11);
	m_IdxBuffTemp.push_back(10);
	m_IdxBuffTemp.push_back(2);
	
	//Face 4
	m_IdxBuffTemp.push_back(10);
	m_IdxBuffTemp.push_back(7);
	m_IdxBuffTemp.push_back(6);
	
	//Face 5
	m_IdxBuffTemp.push_back(7);
	m_IdxBuffTemp.push_back(1);
	m_IdxBuffTemp.push_back(8);

	//////////////////////////
	//5 faces around point 3
	//////////////////////////
	
	//Face 1
	m_IdxBuffTemp.push_back(3);
	m_IdxBuffTemp.push_back(9);
	m_IdxBuffTemp.push_back(4);
	
	//Face 2
	m_IdxBuffTemp.push_back(3);
	m_IdxBuffTemp.push_back(4);
	m_IdxBuffTemp.push_back(2);
	
	//Face 3
	m_IdxBuffTemp.push_back(3);
	m_IdxBuffTemp.push_back(2);
	m_IdxBuffTemp.push_back(6);
	
	//Face 4
	m_IdxBuffTemp.push_back(3);
	m_IdxBuffTemp.push_back(6);
	m_IdxBuffTemp.push_back(8);
	
	//Face 5
	m_IdxBuffTemp.push_back(3);
	m_IdxBuffTemp.push_back(8);
	m_IdxBuffTemp.push_back(9);
	

	//////////////////////////
	//5 adjacent faces 
	//////////////////////////

	//Face 1
	m_IdxBuffTemp.push_back(4);
	m_IdxBuffTemp.push_back(9);
	m_IdxBuffTemp.push_back(5);
	
	//Face 2
	m_IdxBuffTemp.push_back(2);
	m_IdxBuffTemp.push_back(4);
	m_IdxBuffTemp.push_back(11);
	
	//Face 3
	m_IdxBuffTemp.push_back(6);
	m_IdxBuffTemp.push_back(2);
	m_IdxBuffTemp.push_back(10);
	
	//Face 4
	m_IdxBuffTemp.push_back(8);
	m_IdxBuffTemp.push_back(6);
	m_IdxBuffTemp.push_back(7);
	
	//Face 5
	m_IdxBuffTemp.push_back(9);
	m_IdxBuffTemp.push_back(8);
	m_IdxBuffTemp.push_back(1);

	for(uint32 i = 0; i < m_RecursionLvl; ++i)
	{
		std::vector<uint16> idxBuf2;

		uint32 oldIndexBufSize = (uint32)m_IdxBuffTemp.size();

		for(uint32 j = 0; j < oldIndexBufSize; j += 3)
		{
			//Replace triangle by 4 triangles
			uint16 a = GetMiddlePoint(m_IdxBuffTemp[j], m_IdxBuffTemp[j + 1]);
			uint16 b = GetMiddlePoint(m_IdxBuffTemp[j + 1], m_IdxBuffTemp[j + 2]);
			uint16 c = GetMiddlePoint(m_IdxBuffTemp[j + 2], m_IdxBuffTemp[j]);

			//New Face 1
			idxBuf2.push_back(m_IdxBuffTemp[j]);
			idxBuf2.push_back(a);
			idxBuf2.push_back(c);
			
			//New Face 2
			idxBuf2.push_back(m_IdxBuffTemp[j + 1]);
			idxBuf2.push_back(b);
			idxBuf2.push_back(a);
			
			//New Face 3
			idxBuf2.push_back(m_IdxBuffTemp[j + 2]);
			idxBuf2.push_back(c);
			idxBuf2.push_back(b);
   		
			//New Face 4
			idxBuf2.push_back(a);
			idxBuf2.push_back(b);
			idxBuf2.push_back(c);
        }

		m_IdxBuffTemp = idxBuf2;
    }

	//Create RV
	CreateRV();
}

uint16 CSphereMesh::AddVtx(TCOLORED_VERTEX& vtx)
{
	Vect3f vect(vtx.x, vtx.y, vtx.z);
	vect.Normalize();

	vtx.x = vect.x;
	vtx.y = vect.y;
	vtx.z = vect.z;

	std::string vtxStr = GetVtxString(vtx);
	uint16 idx = 0;

	if(m_VtxMapTemp.find(vtxStr) != m_VtxMapTemp.end())
	{
		idx = m_VtxMapTemp[vtxStr];
	}
	else
	{
		idx = (uint16)m_VtxBuffTemp.size();
		m_VtxMapTemp[vtxStr] = (uint16)m_VtxBuffTemp.size();
		m_VtxBuffTemp.push_back(vtx);
	}

	return idx;
}

std::string CSphereMesh::GetVtxString(TCOLORED_VERTEX& vtx)
{
	std::stringstream ss;

	ss << "Pos: x: " << vtx.x << " y: " << vtx.y << " z: " << vtx.z;

	return ss.str();
}

uint16 CSphereMesh::GetMiddlePoint(uint32 vtxIndex1, uint32 vtxIndex2)
{	
	Vect3f point1(m_VtxBuffTemp[vtxIndex1].x, m_VtxBuffTemp[vtxIndex1].y, m_VtxBuffTemp[vtxIndex1].z);
	Vect3f point2(m_VtxBuffTemp[vtxIndex2].x, m_VtxBuffTemp[vtxIndex2].y, m_VtxBuffTemp[vtxIndex2].z);
	
	Vect3f middle(
		(point1.x + point2.x) / 2.0f,
		(point1.y + point2.y) / 2.0f,
		(point1.z + point2.z) / 2.0f
		);

	TCOLORED_VERTEX newVtx;
	newVtx.x = middle.x;
	newVtx.y = middle.y;
	newVtx.z = middle.z;
	newVtx.color = m_Color.GetUint32Argb();

	//Add vertex makes sure point is on unit sphere
	uint16 idx = AddVtx(newVtx); 

	return idx;
}

void CSphereMesh::CreateRV()
{
	TCOLORED_VERTEX* vtxArr = new TCOLORED_VERTEX[m_VtxBuffTemp.size()];
	uint16* idxArr = new uint16[m_IdxBuffTemp.size()];

	for(uint32 i = 0; i < m_VtxBuffTemp.size(); ++i)
	{
		vtxArr[i] = m_VtxBuffTemp[i];
	}
	
	for(uint32 j = 0; j < m_IdxBuffTemp.size(); ++j)
	{
		idxArr[j] = m_IdxBuffTemp[j];
	}

	m_RV = new CIndexedVertexs<TCOLORED_VERTEX>(CORE->GetRenderManager(), vtxArr, idxArr, m_VtxBuffTemp.size(), m_IdxBuffTemp.size());
}

void CSphereMesh::Render(CRenderManager *RM, CEffectTechnique* technique) const
{
	if(m_RV == NULL)
	{
		return;
	}

	m_RV->Render(RM, technique);
}