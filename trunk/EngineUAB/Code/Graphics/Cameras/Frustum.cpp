
//main include files
#include "Frustum.h"
#include "Math\Vector3.h"
#include "Core.h"
#include "RenderManager.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


CFrustum::CFrustum()
{
	ZeroMemory(m_frustum, sizeof(D3DXPLANE) * 6);
	ZeroMemory(m_Corners, sizeof(Vect3f) * 8);
}


void CFrustum::Update(const D3DXMATRIX& viewProjection) 
{
	D3DXVECTOR4 col0(viewProjection(0,0), viewProjection(1,0), viewProjection(2,0), viewProjection(3,0));
	D3DXVECTOR4 col1(viewProjection(0,1), viewProjection(1,1), viewProjection(2,1), viewProjection(3,1));
	D3DXVECTOR4 col2(viewProjection(0,2), viewProjection(1,2), viewProjection(2,2), viewProjection(3,2));
	D3DXVECTOR4 col3(viewProjection(0,3), viewProjection(1,3), viewProjection(2,3), viewProjection(3,3));

	// Planes face inward.
	m_frustum[0] = (D3DXPLANE)(col2);        // near
	m_frustum[1] = (D3DXPLANE)(col3 - col2); // far
	m_frustum[2] = (D3DXPLANE)(col3 + col0); // left
	m_frustum[3] = (D3DXPLANE)(col3 - col0); // right
	m_frustum[4] = (D3DXPLANE)(col3 - col1); // top
	m_frustum[5] = (D3DXPLANE)(col3 + col1); // bottom

	for(int i = 0; i < 6; ++i)
	{
		D3DXPlaneNormalize(&m_frustum[i], &m_frustum[i]);
	}

#define NEAR_P		0
#define FAR_P		1
#define LEFT_P		2
#define RIGHT_P		3
#define TOP_P		4
#define BOTTOM_P	5

	m_Corners[0] = ComputeIntersection(m_frustum[NEAR_P],	m_frustum[BOTTOM_P],	m_frustum[RIGHT_P]);	//Near1          
	m_Corners[1] = ComputeIntersection(m_frustum[NEAR_P],	m_frustum[TOP_P],		m_frustum[RIGHT_P]);	//Near2        
	m_Corners[2] = ComputeIntersection(m_frustum[NEAR_P],	m_frustum[TOP_P],		m_frustum[LEFT_P]);		//Near3       
	m_Corners[3] = ComputeIntersection(m_frustum[NEAR_P],	m_frustum[BOTTOM_P],	m_frustum[LEFT_P]);		//Near4          
	m_Corners[4] = ComputeIntersection(m_frustum[FAR_P],	m_frustum[BOTTOM_P],	m_frustum[RIGHT_P]);	//Far1
	m_Corners[5] = ComputeIntersection(m_frustum[FAR_P],	m_frustum[TOP_P],		m_frustum[RIGHT_P]);	//Far2
	m_Corners[6] = ComputeIntersection(m_frustum[FAR_P],	m_frustum[TOP_P],		m_frustum[LEFT_P]);		//Far3
	m_Corners[7] = ComputeIntersection(m_frustum[FAR_P],	m_frustum[BOTTOM_P],	m_frustum[LEFT_P]);		//Far4

	m_frustumBB = TBoundingBox(m_Corners);
}

bool CFrustum::SphereVisible(const D3DXVECTOR3 &center,float radius) const 
{
	for (int i=0; i < 6; ++i)	
	{
		if (( (m_frustum[i][0]*center.x) + (m_frustum[i][1]*center.y) + 
			(m_frustum[i][2]*center.z) + (m_frustum[i][3] )) <= -radius )
		{
			return false;
		}
	}

	return true;
}	

bool CFrustum::BoxVisibleByVertexs( const Vect3f* points) const 
{  
	int iInCount;
	for(int p=0; p<6; p++)
	{
		iInCount = 8;
		for(int i=0; i<8; i++)
		{
			// Probamos el punto contra todos los planos
			if( (m_frustum[p][0]*points[i].x + m_frustum[p][1]*points[i].y + m_frustum[p][2]*points[i].z + m_frustum[p][3]) <= 0 )
				--iInCount;
		}
		// ¿Están todos los puntos fuera?
		if (iInCount == 0)
			return false;
	}

	//Si todos los puntos están dentro, entonces la caja
	//está dentro del frustum o parcialmente
	return true;
}

bool CFrustum::BoxVisible(const TBoundingBox& bb, const D3DXMATRIX& WorldMatrix) const
{
	D3DXVECTOR3 minTrans(bb.m_MinPos.x, bb.m_MinPos.y, bb.m_MinPos.z);
	D3DXVECTOR3 maxTrans(bb.m_MaxPos.x, bb.m_MaxPos.y, bb.m_MaxPos.z);

	D3DXVECTOR4 minTrans4, maxTrans4;

	D3DXVec3Transform(&minTrans4, &minTrans, &WorldMatrix);
	D3DXVec3Transform(&maxTrans4, &maxTrans, &WorldMatrix);

	minTrans = D3DXVECTOR3(minTrans4.x, minTrans4.y, minTrans4.z);
	maxTrans = D3DXVECTOR3(maxTrans4.x, maxTrans4.y, maxTrans4.z);

	return BoxVisible(minTrans, maxTrans);
}


bool CFrustum::BoxVisible( const D3DXVECTOR3 &max, const D3DXVECTOR3 &min) const 
{
	float points[24];

	// calculamos los 8 puntos de la bounding box
	points[0] = max.x;
	points[1] = max.y;
	points[2] = max.z;

	points[3] = min.x;
	points[4] = max.y;
	points[5] = max.z;

	points[6] = min.x;
	points[7] = min.y;
	points[8] = max.z;

	points[9] = max.x;
	points[10] = min.y;
	points[11] = max.z;

	points[12] = min.x;
	points[13] = min.y;
	points[14] = min.z;

	points[15] = max.x;
	points[16] = min.y;
	points[17] = min.z;

	points[18] = max.x;
	points[19] = max.y;
	points[20] = min.z;

	points[21] = min.x;
	points[22] = max.y;
	points[23] = min.z;

	int iInCount;
	for(int p=0; p<6; p++)
	{
		iInCount = 8;
		for(int i=0; i<8; i++)
		{
			// Probamos el punto contra todos los planos
			if( (m_frustum[p].a * points[3*i] + m_frustum[p].b * points[3*i+1] + m_frustum[p].c * points[3*i+2] + m_frustum[p].d) <= 0 )
			{
				--iInCount;
			}
		}
		// ¿Están todos los puntos fuera?
		if (iInCount == 0)
		{
			return false;
		}
	}	

	// Si todos los puntos están dentro, entonces la caja
	// está dentro del frustum o parcialmente
	return true;
}

bool CFrustum::BoxVisible(CalBoundingBox& bb)
{
	CalVector cbb[8];
	ZeroMemory(cbb, sizeof(CalVector) * 8);

	bb.computePoints(cbb);

	D3DXVECTOR3 minDX(cbb[0].x, cbb[0].y, cbb[0].z);
	D3DXVECTOR3 maxDX(cbb[0].x, cbb[0].y, cbb[0].z);

	for (int i = 1; i < 8; ++i)
	{
		if(minDX.x > cbb[i].x)
		{
			minDX.x = cbb[i].x;
		}

		if(minDX.y > cbb[i].y)
		{
			minDX.y = cbb[i].y;
		}

		if(minDX.z > cbb[i].z)
		{
			minDX.z = cbb[i].z;
		}

		if(maxDX.x < cbb[i].x)
		{
			maxDX.x = cbb[i].x;
		}

		if(maxDX.y < cbb[i].y)
		{
			maxDX.y = cbb[i].y;
		}

		if(maxDX.z < cbb[i].z)
		{
			maxDX.z = cbb[i].z;
		}
	}

	return BoxVisible(maxDX, minDX);
}

Vect3f CFrustum::ComputeIntersection(D3DXPLANE plane1, D3DXPLANE plane2, D3DXPLANE plane3)
{
	// q = D1(N2 X N3) + D2(N3 X N1) + D3(N1 X N2)
	//     ---------------------------------------
	//              N1.(N2 X N3)

	D3DXVECTOR3 normal1(plane1.a, plane1.b, plane1.c);
	D3DXVECTOR3 normal2(plane2.a, plane2.b, plane2.c);
	D3DXVECTOR3 normal3(plane3.a, plane3.b, plane3.c);

	D3DXVECTOR3 N2CN3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 N3CN1(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 N1CN2(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 D1N2CN3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 D2N3CN1(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 D3N1CN2(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 UpSum(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Res(0.0f, 0.0f, 0.0f);
	float N1DN2CN3 = 0.0f;

	D3DXVec3Cross(&N2CN3, &normal2, &normal3);
	D3DXVec3Cross(&N3CN1, &normal3, &normal1);
	D3DXVec3Cross(&N1CN2, &normal1, &normal2);

	N1DN2CN3 = D3DXVec3Dot(&normal1, &N2CN3);

	if(N1DN2CN3 == 0.0f)
	{
		return Vect3f((float)INFINITE, (float)INFINITE, (float)INFINITE);
	}

	D1N2CN3 = -plane1.d * N2CN3;
	D2N3CN1 = -plane2.d * N3CN1;
	D3N1CN2 = -plane3.d * N1CN2;
	
	UpSum = D1N2CN3 + D2N3CN1 + D3N1CN2;

	Res = UpSum / N1DN2CN3;

	return Vect3f(Res.x, Res.y, Res.z);
}

void CFrustum::DrawFrustum()
{
	Vect3f line1(0.0f, 0.0f, 0.0f);
	Vect3f line2(0.0f, 0.0f, 0.0f);

	//NearPlane
	CORE->GetRenderManager()->DrawLine(m_Corners[0], m_Corners[1], colRED);
	CORE->GetRenderManager()->DrawLine(m_Corners[1], m_Corners[2], colRED);
	CORE->GetRenderManager()->DrawLine(m_Corners[2], m_Corners[3], colRED);
	CORE->GetRenderManager()->DrawLine(m_Corners[3], m_Corners[0], colRED);

	//FarPlane
	CORE->GetRenderManager()->DrawLine(m_Corners[4], m_Corners[5], colCYAN);
	CORE->GetRenderManager()->DrawLine(m_Corners[5], m_Corners[6], colCYAN);
	CORE->GetRenderManager()->DrawLine(m_Corners[6], m_Corners[7], colCYAN);
	CORE->GetRenderManager()->DrawLine(m_Corners[7], m_Corners[4], colCYAN);

	//RightPlane
	CORE->GetRenderManager()->DrawLine(m_Corners[0], m_Corners[4], colWHITE);
	CORE->GetRenderManager()->DrawLine(m_Corners[1], m_Corners[5], colWHITE);
	
	//LeftPlane
	CORE->GetRenderManager()->DrawLine(m_Corners[3], m_Corners[7], colWHITE);
	CORE->GetRenderManager()->DrawLine(m_Corners[2], m_Corners[6], colWHITE);

	for(int x = 0; x < 19; ++x)
	{
		//NearPlane
		line1.y = mathUtils::Lerp(m_Corners[1].y, m_Corners[2].y, ((float)x + 1.0f) / 20.0f);
		line2.y = mathUtils::Lerp(m_Corners[0].y, m_Corners[3].y, ((float)x + 1.0f) / 20.0f);

		line1.x = mathUtils::Lerp(m_Corners[1].x, m_Corners[2].x, ((float)x + 1.0f) / 20.0f);
		line2.x = mathUtils::Lerp(m_Corners[0].x, m_Corners[3].x, ((float)x + 1.0f) / 20.0f);
		
		line1.z = mathUtils::Lerp(m_Corners[1].z, m_Corners[2].z, ((float)x + 1.0f) / 20.0f);
		line2.z = mathUtils::Lerp(m_Corners[0].z, m_Corners[3].z, ((float)x + 1.0f) / 20.0f);
		
		CORE->GetRenderManager()->DrawLine(line1, line2, colRED);

		//FarPlane
		line1.y = mathUtils::Lerp(m_Corners[5].y, m_Corners[6].y, ((float)x + 1.0f) / 20.0f);
		line2.y = mathUtils::Lerp(m_Corners[4].y, m_Corners[7].y, ((float)x + 1.0f) / 20.0f);

		line1.x = mathUtils::Lerp(m_Corners[5].x, m_Corners[6].x, ((float)x + 1.0f) / 20.0f);
		line2.x = mathUtils::Lerp(m_Corners[4].x, m_Corners[7].x, ((float)x + 1.0f) / 20.0f);
		
		line1.z = mathUtils::Lerp(m_Corners[5].z, m_Corners[6].z, ((float)x + 1.0f) / 20.0f);
		line2.z = mathUtils::Lerp(m_Corners[4].z, m_Corners[7].z, ((float)x + 1.0f) / 20.0f);
		
		CORE->GetRenderManager()->DrawLine(line1, line2, colCYAN);

		//RightPlane
		line1.y = mathUtils::Lerp(m_Corners[0].y, m_Corners[4].y, ((float)x + 1.0f) / 20.0f);
		line2.y = mathUtils::Lerp(m_Corners[1].y, m_Corners[5].y, ((float)x + 1.0f) / 20.0f);

		line1.x = mathUtils::Lerp(m_Corners[0].x, m_Corners[4].x, ((float)x + 1.0f) / 20.0f);
		line2.x = mathUtils::Lerp(m_Corners[1].x, m_Corners[5].x, ((float)x + 1.0f) / 20.0f);
		
		line1.z = mathUtils::Lerp(m_Corners[0].z, m_Corners[4].z, ((float)x + 1.0f) / 20.0f);
		line2.z = mathUtils::Lerp(m_Corners[1].z, m_Corners[5].z, ((float)x + 1.0f) / 20.0f);
		
		CORE->GetRenderManager()->DrawLine(line1, line2, colWHITE);

		//LeftPlane
		line1.y = mathUtils::Lerp(m_Corners[3].y, m_Corners[7].y, ((float)x + 1.0f) / 20.0f);
		line2.y = mathUtils::Lerp(m_Corners[2].y, m_Corners[6].y, ((float)x + 1.0f) / 20.0f);

		line1.x = mathUtils::Lerp(m_Corners[3].x, m_Corners[7].x, ((float)x + 1.0f) / 20.0f);
		line2.x = mathUtils::Lerp(m_Corners[2].x, m_Corners[6].x, ((float)x + 1.0f) / 20.0f);
		
		line1.z = mathUtils::Lerp(m_Corners[3].z, m_Corners[7].z, ((float)x + 1.0f) / 20.0f);
		line2.z = mathUtils::Lerp(m_Corners[2].z, m_Corners[6].z, ((float)x + 1.0f) / 20.0f);
		
		CORE->GetRenderManager()->DrawLine(line1, line2, colWHITE);

		//Top
		line1.y = mathUtils::Lerp(m_Corners[1].y, m_Corners[5].y, ((float)x + 1.0f) / 20.0f);
		line2.y = mathUtils::Lerp(m_Corners[2].y, m_Corners[6].y, ((float)x + 1.0f) / 20.0f);

		line1.x = mathUtils::Lerp(m_Corners[1].x, m_Corners[5].x, ((float)x + 1.0f) / 20.0f);
		line2.x = mathUtils::Lerp(m_Corners[2].x, m_Corners[6].x, ((float)x + 1.0f) / 20.0f);
		
		line1.z = mathUtils::Lerp(m_Corners[1].z, m_Corners[5].z, ((float)x + 1.0f) / 20.0f);
		line2.z = mathUtils::Lerp(m_Corners[2].z, m_Corners[6].z, ((float)x + 1.0f) / 20.0f);

		CORE->GetRenderManager()->DrawLine(line1, line2, colYELLOW);
		
		//Bottom
		line1.y = mathUtils::Lerp(m_Corners[0].y, m_Corners[4].y, ((float)x + 1.0f) / 20.0f);
		line2.y = mathUtils::Lerp(m_Corners[3].y, m_Corners[7].y, ((float)x + 1.0f) / 20.0f);

		line1.x = mathUtils::Lerp(m_Corners[0].x, m_Corners[4].x, ((float)x + 1.0f) / 20.0f);
		line2.x = mathUtils::Lerp(m_Corners[3].x, m_Corners[7].x, ((float)x + 1.0f) / 20.0f);
		
		line1.z = mathUtils::Lerp(m_Corners[0].z, m_Corners[4].z, ((float)x + 1.0f) / 20.0f);
		line2.z = mathUtils::Lerp(m_Corners[3].z, m_Corners[7].z, ((float)x + 1.0f) / 20.0f);

		CORE->GetRenderManager()->DrawLine(line1, line2, colYELLOW);
	}
}
