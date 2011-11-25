#ifndef _GRAPHICSDEFS_H
#define _GRAPHICSDEFS_H

#define ACTION_LOGGER "Logger"
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_SCREEN_COLOR_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

typedef enum ETypeAlignment { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };

struct CUSTOMVERTEX
{
   float x, y, z;
   DWORD color;
   static unsigned int getFlags()
   {
       return D3DFVF_CUSTOMVERTEX;
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


#endif