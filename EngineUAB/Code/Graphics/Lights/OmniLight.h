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

#include "Light.h"

class COmniLight : public CLight
{
public:
	COmniLight();
	COmniLight( CXMLTreeNode &XMLNode );
	virtual ~COmniLight();

	virtual void		Render				( CRenderManager* RM );
	void				SetShadowMap		( CRenderManager *RM );
};

#endif
