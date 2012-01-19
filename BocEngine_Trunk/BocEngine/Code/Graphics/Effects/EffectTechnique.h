#pragma once

#ifndef _EFFECTTECHNIQUE_H
#define _EFFECTTECHNIQUE_H

class CEffect;
class CXMLTreeNode;

#include "Utils\Types.h"
#include <string>
#include <d3dx9.h>

class CEffect;

class CEffectTechnique
{
public:
	friend class CEffect;

	CEffectTechnique(CEffect* parentEffect, D3DXHANDLE effectTechnique, const std::string techniqueName);
	virtual ~CEffectTechnique();

	const std::string&	GetTechniqueName	() const		{ return m_TechniqueName; }

private:
	D3DXHANDLE	GetD3DXTechnique	() const	{ return m_D3DTechnique; }

private:
	const CEffect*	m_ParentEffect;
	D3DXHANDLE	m_D3DTechnique;
	std::string m_TechniqueName;
};

#endif