// Disable warning for loss of data
#pragma warning( disable : 4244 )  

//main include files
#include "Frustum.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

void CFrustum::Update(const Mat44f &clip) 
{
 //	// Extract the numbers for the RIGHT plane 
	//m_frustum[0].x = clip.m03 - clip.m00;
	//m_frustum[0].y = clip.m13 - clip.m10;
	//m_frustum[0].z = clip.m23 - clip.m20;
	//m_frustum[0].w = clip.m33 - clip.m30;

	//// Normalize the result 
	//float t = 1.0f / sqrtf( (m_frustum[0].x * m_frustum[0].x) + (m_frustum[0].y * m_frustum[0].y) + (m_frustum[0].z * m_frustum[0].z) );
	//m_frustum[0].x  *= t;
	//m_frustum[0].y  *= t;
	//m_frustum[0].z  *= t;
	//m_frustum[0].w  *= t;

	//// Extract the numbers for the LEFT plane 
	//m_frustum[1].x  = clip.m03 + clip.m00;
	//m_frustum[1].y  = clip.m13 + clip.m10;
	//m_frustum[1].z  = clip.m23 + clip.m20;
	//m_frustum[1].w  = clip.m33 + clip.m30;

	//// Normalize the result 
	//t = 1.0f / sqrtf( (m_frustum[1].x * m_frustum[1].x) + (m_frustum[1].y * m_frustum[1].y) + (m_frustum[1].z * m_frustum[1].z) );
	//m_frustum[1].x  *= t;
	//m_frustum[1].y  *= t;
	//m_frustum[1].z  *= t;
	//m_frustum[1].w  *= t;

	//// Extract the BOTTOM plane 
	//m_frustum[2].x  = clip.m03 + clip.m01;
	//m_frustum[2].y  = clip.m13 + clip.m11;
	//m_frustum[2].z  = clip.m23 + clip.m21;
	//m_frustum[2].w  = clip.m33 + clip.m31;

	//// Normalize the result 
	//t = 1.0f / sqrtf( (m_frustum[2].x * m_frustum[2].x) + (m_frustum[2].y * m_frustum[2].y) + (m_frustum[2].z * m_frustum[2].z) );
	//m_frustum[2].x  *= t;
	//m_frustum[2].y  *= t;
	//m_frustum[2].z  *= t;
	//m_frustum[2].w  *= t;

	//// Extract the TOP plane 
	//m_frustum[3].x  = clip.m03 - clip.m01;
	//m_frustum[3].y  = clip.m13 - clip.m11;
	//m_frustum[3].z  = clip.m23 - clip.m21;
	//m_frustum[3].w  = clip.m33 - clip.m31;

	//// Normalize the result 
	//t = 1.0f / sqrtf( (m_frustum[3].x * m_frustum[3].x) + (m_frustum[3].y * m_frustum[3].y) + (m_frustum[3].z * m_frustum[3].z) );
	//m_frustum[3].x  *= t;
	//m_frustum[3].y  *= t;
	//m_frustum[3].z  *= t;
	//m_frustum[3].w  *= t;

	//// Extract the FAR plane 
	//m_frustum[4].x  = clip.m03 - clip.m02;
	//m_frustum[4].y  = clip.m13 - clip.m12;
	//m_frustum[4].z  = clip.m23 - clip.m22;
	//m_frustum[4].w  = clip.m33 - clip.m32;

	//// Normalize the result 
	//t = 1.0f / sqrtf( (m_frustum[4].x * m_frustum[4].x) + (m_frustum[4].y * m_frustum[4].y) + (m_frustum[4].z * m_frustum[4].z) );
	//m_frustum[4].x  *= t;
	//m_frustum[4].y  *= t;
	//m_frustum[4].z  *= t;
	//m_frustum[4].w  *= t;

	//// Extract the NEAR plane 
	//m_frustum[5].x  = clip.m03 + clip.m02;
	//m_frustum[5].y  = clip.m13 + clip.m12;
	//m_frustum[5].z  = clip.m23 + clip.m22;
	//m_frustum[5].w  = clip.m33 + clip.m32;

	//// Normalize the result 
	//t = 1.0f / sqrtf( (m_frustum[5].x * m_frustum[5].x) + (m_frustum[5].y * m_frustum[5].y) + (m_frustum[5].z * m_frustum[5].z) );
	//m_frustum[5].x  *= t;
	//m_frustum[5].y  *= t;
	//m_frustum[5].z  *= t;
	//m_frustum[5].w  *= t;

	D3DXMATRIX viewProjection = clip.GetD3DXMatrix();
	// Left plane    
	m_frustum[0].x = viewProjection._14 + viewProjection._11;  
	m_frustum[0].y = viewProjection._24 + viewProjection._21;   
	m_frustum[0].z = viewProjection._34 + viewProjection._31; 
	m_frustum[0].w = viewProjection._44 + viewProjection._41; 

	// Right plane
	m_frustum[1].x = viewProjection._14 - viewProjection._11;  
	m_frustum[1].y = viewProjection._24 - viewProjection._21; 
	m_frustum[1].z = viewProjection._34 - viewProjection._31;  
	m_frustum[1].w = viewProjection._44 - viewProjection._41;  
	
	// Top plane 
	m_frustum[2].x = viewProjection._14 - viewProjection._12;  
	m_frustum[2].y = viewProjection._24 - viewProjection._22;  
	m_frustum[2].z = viewProjection._34 - viewProjection._32;  
	m_frustum[2].w = viewProjection._44 - viewProjection._42;    
	
	// Bottom plane   
	m_frustum[3].x = viewProjection._14 + viewProjection._12; 
	m_frustum[3].y = viewProjection._24 + viewProjection._22;  
	m_frustum[3].z = viewProjection._34 + viewProjection._32;  
	m_frustum[3].w = viewProjection._44 + viewProjection._42;

	// Near plane 
	m_frustum[4].x = viewProjection._13; 
	m_frustum[4].y = viewProjection._23;  
	m_frustum[4].z = viewProjection._33;  
	m_frustum[4].w = viewProjection._43;   
	
	// Far plane
	m_frustum[5].x = viewProjection._14 - viewProjection._13; 
	m_frustum[5].y = viewProjection._24 - viewProjection._23;   
	m_frustum[5].z = viewProjection._34 - viewProjection._33;  
	m_frustum[5].w = viewProjection._44 - viewProjection._43;

	for(int i = 0; i < 6; ++i)
	{
		float t = 1.0f / sqrtf( (m_frustum[i].x * m_frustum[i].x) + (m_frustum[i].y * m_frustum[i].y) + (m_frustum[i].z * m_frustum[i].z));
		m_frustum[i] /= t;
	}

#define NEAR_P		4
#define FAR_P		5
#define TOP_P		2
#define BOTTOM_P	3
#define RIGHT_P		1
#define LEFT_P		0

	m_Corners[0] = ComputeIntersection(m_frustum[NEAR_P], m_frustum[BOTTOM_P], m_frustum[RIGHT_P]);    //Near1          
	m_Corners[1] = ComputeIntersection(m_frustum[NEAR_P], m_frustum[TOP_P], m_frustum[RIGHT_P]);       //Near2        
	m_Corners[2] = ComputeIntersection(m_frustum[NEAR_P], m_frustum[TOP_P], m_frustum[LEFT_P]);        //Near3       
	m_Corners[3] = ComputeIntersection(m_frustum[NEAR_P], m_frustum[BOTTOM_P], m_frustum[LEFT_P]);     //Near3          
	m_Corners[4] = ComputeIntersection(m_frustum[FAR_P], m_frustum[BOTTOM_P], m_frustum[RIGHT_P]);    //Far1            
	m_Corners[5] = ComputeIntersection(m_frustum[FAR_P], m_frustum[TOP_P], m_frustum[RIGHT_P]);       //Far2           
	m_Corners[6] = ComputeIntersection(m_frustum[FAR_P], m_frustum[TOP_P], m_frustum[LEFT_P]);        //Far3           
	m_Corners[7] = ComputeIntersection(m_frustum[FAR_P], m_frustum[BOTTOM_P], m_frustum[LEFT_P]);     //Far3       
}

Vect3f CFrustum::ComputeIntersection(Vect4f plane1, Vect4f plane2, Vect4f plane3)
{
	// q = D1(N2 X N3) + D2(N3 X N1) + D3(N1 X N2)
	//     ---------------------------------------
	//              N1.(N2 X N3)

	Vect3f normal1(plane1.x, plane1.y, plane1.z);
	Vect3f normal2(plane2.x, plane2.y, plane2.z);
	Vect3f normal3(plane3.x, plane3.y, plane3.z);

	Vect3f N2CN3 = normal2.Cross(normal3);
	float N1DN2CN2 = normal1.Dot(N2CN3);

	Vect3f D1N2CN3 = -plane1.w * (normal2.Cross(normal3));
	Vect3f D2N3CN1 = -plane2.w * (normal3.Cross(normal1));
	Vect3f D3N1CN2 = -plane3.w * (normal1.Cross(normal2));

	Vect3f UpSum = D1N2CN3 + D2N3CN1 + D3N1CN2;

	Vect3f Res = UpSum / N1DN2CN2;

	return Res;
}

bool CFrustum::SphereVisible(const Vect3f &center,float radius) const 
{
	for (int i=0; i < 6; ++i)	
	{
		if (( (m_frustum[i].x*center.x) + (m_frustum[i].y*center.y) + 
			 (m_frustum[i].z*center.z) + (m_frustum[i].w )) <= -radius )
		{
				return false;
		}
	}

	return true;
}

bool CFrustum::BoxVisibleByVertexs( const Vect3f* points) const 
{  
  int iInCount;
	for(int p=0; p<6; ++p)
	{
		iInCount = 8;
		for(int i=0; i<8; ++i)
		{
			// Probamos el punto contra todos los planos
			if( (m_frustum[p].x*points[i].x + m_frustum[p].y*points[i].y + m_frustum[p].z*points[i].z + m_frustum[p].w) <= 0 )
				--iInCount;
		}
		// ¿Están todos los puntos fuera?
		if (iInCount == 0)
			return false;
	}
	
	// Si todos los puntos están dentro, entonces la caja
	// está dentro del frustum o parcialmente
	return true;
}

bool CFrustum::BoxVisible( const Vect3f &max, const Vect3f &min) const 
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
	for(int p=0; p<6; ++p)
	{
		iInCount = 8;
		for(int i=0; i<8; ++i)
		{
			// Probamos el punto contra todos los planos
			if( (m_frustum[p].x*points[3*i] + m_frustum[p].y*points[3*i+1] + m_frustum[p].z*points[3*i+2] + m_frustum[p].w) <= 0 )
			{
				--iInCount;
			}
		}
		// ¿Están todos los puntos fuera?
		if (iInCount == 0)
			return false;
	}
	
	// Si todos los puntos están dentro, entonces la caja
	// está dentro del frustum o parcialmente
	return true;
}

CFrustum::CFrustum()
{
	memset(m_proj, 0, sizeof(float) * 16);
	memset(m_modl, 0, sizeof(float) * 16);
	memset(m_clip, 0, sizeof(float) * 16);
	memset(m_frustum, 0, sizeof(Vect4f) * 6);
	memset(m_Corners, 0, sizeof(Vect3f) * 8);
}
