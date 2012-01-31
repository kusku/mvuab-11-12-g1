#pragma once

#ifndef _RENDERABLEOBJECT_H
#define _RENDERABLEOBJECT_H

class CRenderManager;
class CEffectTechnique;
class CRenderableObjectTechnique;

#include "Object3D.h"
#include "Utils\Named.h"

class CRenderableObject : public CObject3D, public CNamed
{
public:
	CRenderableObject();
	virtual ~CRenderableObject();

	virtual void Update (float elapsedTime);
	virtual void Render (CRenderManager *RM) = 0;
	//virtual void Render (CRenderManager *RM, CEffectTechnique* technique) = 0;

	void		SetRenderableObjectTechnique	( CRenderableObjectTechnique *ROT )		{ m_pROTechnique = ROT; }
	CRenderableObjectTechnique*		GetRenderableObjectTechnique		() const		{ return m_pROTechnique; }

private:
	CRenderableObjectTechnique		*m_pROTechnique;
};

#endif
