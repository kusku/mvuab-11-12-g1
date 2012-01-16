#pragma once

#ifndef _VERTEXTYPE_H
#define _VERTEXTYPE_H

#include <d3dx9.h>
#include "Base.h"
#include "Core.h"
#include "RenderManager.h"

#define VERTEX_TYPE_GEOMETRY 0x0001
#define VERTEX_TYPE_NORMAL 0x0002
#define VERTEX_TYPE_TANGENT 0x0004
#define VERTEX_TYPE_BINORMAL 0x0008
#define VERTEX_TYPE_TEXTURE1 0x0010
#define VERTEX_TYPE_TEXTURE2 0x0020
#define VERTEX_TYPE_DIFFUSE 0x0040

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
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },

				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },
			
				{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_COLOR, 0 },

				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },
				
				{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 0 },
			
				{ 0, 20, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_COLOR, 0 },

				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },
				
				{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 0 },

				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },
				
				{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 0 },
				
				{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 1 },

				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },
				
				{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_COLOR, 0 },
				
				{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 0 },

				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },
				
				{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_COLOR, 0 },
				
				{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 0 },
				
				{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 1 },

				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },
				
				{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_NORMAL, 0 },
				
				{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_COLOR, 0 },
				
				{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 1 },

				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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
	static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration()
	{
		if(s_VertexDeclaration==NULL)
		{
			D3DVERTEXELEMENT9 l_VertexDeclaration[] =
			{
				{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_POSITION, 0 },
			
				{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_NORMAL, 0 },
				
				{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 0 },
			
				{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
					D3DDECLUSAGE_TEXCOORD, 1 },
			
				D3DDECL_END()
			};

			CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																			&s_VertexDeclaration);
		}

		return s_VertexDeclaration;
	}
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

};
#endif