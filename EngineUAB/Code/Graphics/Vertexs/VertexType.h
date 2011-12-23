#pragma once

#ifndef _VERTEXTYPE_H
#define _VERTEXTYPE_H

#include <d3dx9.h>

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

#endif