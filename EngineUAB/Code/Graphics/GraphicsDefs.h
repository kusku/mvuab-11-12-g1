#ifndef _GRAPHICSDEFS_H
#define _GRAPHICSDEFS_H

#define MAXBONES 29

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_SCREEN_COLOR_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define D3DFVF_SCREEN_COLOR_TEXTURED_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1)
#define D3DFVF_SCREEN_TEXTURE_VERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

typedef enum ETypeAlignment { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };
typedef enum ETypeFlip { NONE_FLIP, FLIP_X, FLIP_Y };



struct CUSTOMVERTEX
{
   float x, y, z;
   DWORD color;
   static unsigned int getFlags()
   {
       return D3DFVF_CUSTOMVERTEX;
   }
};

struct SCREEN_TEXTURE_VERTEX
{
	float x, y, z, w;
	float tu, tv;
	static unsigned int getFlags()
	{
		return D3DFVF_SCREEN_TEXTURE_VERTEX;
	}
};

struct SCREEN_COLOR_VERTEX
{
    float x,y,z,w;
    DWORD color;
    static unsigned int getFlags()
    {
        return D3DFVF_SCREEN_COLOR_VERTEX;
    }
};

struct SCREEN_COLOR_TEXTURED_VERTEX
{
	float x, y, z, w;
	DWORD color;
	float tu, tv;
	static unsigned int getFlags()
	{
		return D3DFVF_SCREEN_COLOR_TEXTURED_VERTEX;
	}
};


#endif