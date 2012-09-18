//----------------------------------------------------------------------------------
// COmniLight class
// Author: Marc Cuscullola
//
// Description:
// Define una luz de tipo Omni
//----------------------------------------------------------------------------------
#pragma once

#ifndef _OMNILIGHT_H
#define _OMNILIGHT_H

class CXMLTreeNode;
class CRenderManager;

#include "Textures\Texture.h"

#include "Light.h"

class COmniLight : public CLight
{
public:
	COmniLight();
	COmniLight( CXMLTreeNode &XMLNode );
	virtual ~COmniLight();

	virtual void		Render				( CRenderManager* RM );
	virtual void		RenderShadows		( CRenderManager* RM );
	void				SetShadowMap		(  );

	protected:
		void		CreateShadowTextures(bool staticMap, CTexture::TFormatType formatStatic, uint32 staticWidth, uint32 staticHeight, bool dynamicMap, CTexture::TFormatType formatDynamic, uint32 dynamicWidth, uint32 dynamicHeight);
};

#endif
