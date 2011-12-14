#pragma once

#ifndef _RENDERABLEOBJECT_H
#define _RENDERABLEOBJECT_H

class CRenderManager;

#include "..\Object3D.h"
#include "Utils\Named.h"

class CRenderableObject : public CObject3D, public CNamed
{
public:
	CRenderableObject();
	virtual ~CRenderableObject();

	virtual void Update (float elapsedTime);
	virtual void Render (CRenderManager *RM) = 0;
};

#endif
