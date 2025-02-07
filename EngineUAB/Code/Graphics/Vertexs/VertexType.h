#pragma once

#ifndef _VERTEXTYPE_H
#define _VERTEXTYPE_H

#include "Base.h"
#include <d3dx9.h>

#define VERTEX_TYPE_GEOMETRY	0x0001
#define VERTEX_TYPE_NORMAL		0x0002
#define VERTEX_TYPE_TANGENT		0x0004
#define VERTEX_TYPE_BINORMAL	0x0008
#define VERTEX_TYPE_TEXTURE1	0x0010
#define VERTEX_TYPE_TEXTURE2	0x0020
#define VERTEX_TYPE_DIFFUSE		0x0040
#define VERTEX_TYPE_WEIGHTS		0x0080
#define VERTEX_TYPE_INDICES		0x0100
#define VERTEX_TYPE_PARALLAX	0x0200
#define VERTEX_TYPE_RNM			0x0400
#define VERTEX_TYPE_PARTICLE	0x0800
#define VERTEX_TYPE_INSTANCE	0x1000
#define VERTEX_TYPE_TEXSPLAT	0x2000

struct TGEOMETRY_VERTEX
{
	float x, y, z;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();
};

struct TCOLORED_VERTEX
{
	float x, y, z;
	unsigned long color;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_DIFFUSE;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_DIFFUSE;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclarationInstance;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclarationInstance();
};

struct TNORMALCOLORED_VERTEX
{
	float x, y, z;
	float nx, ny, nz;
	unsigned long color;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclarationInstance;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclarationInstance();
};

struct TTEXTURE1_VERTEX
{
	float x, y, z;
	float tu, tv;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_TEX1;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();
};

struct TTEXTURE2_VERTEX
{
	float x, y, z;
	float tu, tv;
	float tu2, tv2;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE2;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEX2;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();
};

struct TCOLOREDTEXTURE1_VERTEX
{
	float x, y, z;
	unsigned long color;
	float tu, tv;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();
};

struct TCOLOREDTEXTURE2_VERTEX
{
	float x, y, z;
	unsigned long color;
	float tu, tv;
	float tu2, tv2;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE2;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1|D3DFVF_TEX2;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();
};

struct TNORMALCOLOREDTEXTURE1_VERTEX
{
	float x, y, z;
	float nx, ny, nz;
	unsigned long color;
	float tu, tv;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();
};

struct TNORMALCOLOREDTEXTURE2_VERTEX
{
	float x, y, z;
	float nx, ny, nz;
	unsigned long color;
	float tu, tv;
	float tu2, tv2;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE2;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1|D3DFVF_TEX2;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();
};

struct TNORMALTEXTURE1_VERTEX
{
	float x, y, z;
	float nx, ny, nz;
	float tu, tv;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclarationInstance;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclarationInstance();
};

struct TNORMALTEXTURE2_VERTEX
{
	float x, y, z;
	float nx, ny, nz;
	float tu, tv;
	float tu2, tv2;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_TEXTURE2;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_TEX2;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclarationInstance;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclarationInstance();
};

struct TNORMALBINORMAL_VERTEX
{
	float x, y, z;
	float nx, ny, nz;
	float tu, tv;
	//float 
		
	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();
};

struct CAL3D_HW_VERTEX
{
	float x, y, z;
	float weights[4];
	float indices[4];
	float nx, ny, nz;
	float tu,tv;
	
	static inline unsigned short GetVertexType()
	{
		return 0;
	}

	static inline unsigned int GetFVF()
	{
		return 0;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

struct CAL3D_HW_VERTEX_BT
{
	float x, y, z;
	float weights[4];
	float indices[4];
	float nx, ny, nz, nw;
	float tangentx, tangenty, tangentz, tangentw;
	float binormalx, binormaly, binormalz, binormalw;
	float tu,tv;
	
	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_WEIGHTS|VERTEX_TYPE_INDICES|VERTEX_TYPE_NORMAL|
				VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return 0;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclarationInstance;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclarationInstance();
};

struct TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX
{
	float x, y, z;
	float nx, ny, nz, nw;
	float tangentx, tangenty, tangentz, tangentw;
	float binormalx, binormaly, binormalz, binormalw;
	float tu,tv;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|VERTEX_TYPE_TEXTURE1;
	}
	static inline unsigned int GetFVF()
	{
		return 0;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclarationInstance;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclarationInstance();
};

struct TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX
{
	float x, y, z;
	float nx, ny, nz, nw;
	float tangentx, tangenty, tangentz, tangentw;
	float binormalx, binormaly, binormalz, binormalw;
	float tu,tv;
	float tu2,tv2;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_RNM;
	}
	static inline unsigned int GetFVF()
	{
		return 0;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclarationInstance;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclarationInstance();
};

struct TPARTICLE_VERTEX
{
	int16 m_Corner_X;
	int16 m_Corner_Y;
	float m_x;
	float m_y;
	float m_z;
	float m_v_x;
	float m_v_y;
	float m_v_z;
	float m_Time;
	unsigned long m_RandomColorRGBA;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_PARTICLE;
	}
	static inline unsigned int GetFVF()
	{
		return 0;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

struct TINSTANCE_VERTEX
{
	float m_00;
	float m_01;
	float m_02;
	float m_03;

	float m_10;
	float m_11;
	float m_12;
	float m_13;

	float m_20;
	float m_21;
	float m_22;
	float m_23;

	float m_30;
	float m_31;
	float m_32;
	float m_33;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_INSTANCE;
	}
	static inline unsigned int GetFVF()
	{
		return 0;
	}

	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

extern void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount, 
										size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,
										size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);

#endif