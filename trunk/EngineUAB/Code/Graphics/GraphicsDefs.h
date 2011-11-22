#ifndef _GRAPHICSDEFS_H
#define _GRAPHICSDEFS_H

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
   float x, y, z;
   DWORD color;
   static unsigned int getFlags()
   {
       return D3DFVF_CUSTOMVERTEX;
   }
};

#endif