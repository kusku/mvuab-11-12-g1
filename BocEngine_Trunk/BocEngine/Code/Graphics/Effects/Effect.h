#pragma once

#ifndef _EFFECT_H
#define _EFFECT_H

#include <string>
#include <d3dx9.h>

#include "EffectDefs.h"
#include "Math\Vector3.h"
#include "EffectParameterCollection.h"
#include "EffectTechniqueCollection.h"

class CXMLTreeNode;
class CEffectTechnique;
class CEffectParameter;

class CEffect
{
public:
	
	//Declare Friend Class so Effect Parameter can access special private methods
	friend class CEffectParameter;
	
	CEffect( CXMLTreeNode *XMLNode );
	virtual ~CEffect();

	bool					Load				( const std::string &Filename );
	bool					Reload				();

	CEffectTechnique*		GetCurrentTechnique	() const	{return m_CurrentTechnique; }

	bool					SetCurrentTechnique	( CEffectTechnique* effectTechnique );
	CEffectTechnique*		GetTechnique		( const std::string& TechniqueName );

	CEffectParameter*		GetParameter			( const std::string& ParameterName );
	CEffectParameter*		GetParameterBySemantic	( const std::string& SemanticName );

	//Get Methods
	const std::string&	GetEffectName		() const		{ return m_EffectName; }

	//Wrap Methods
	bool Begin(uint32& passes, EFFECT_FLAGS flags);
	bool End();
	bool BeginPass(uint32 pass);
	bool EndPass();

protected:	
	virtual bool	LoadEffect	();
	virtual void	Unload		();

	bool	LoadTechniquesToMemory	();
	bool	LoadParametersToMemory	();

protected:
	CEffectTechniqueCollection m_TechniquesCollection;
	CEffectParameterCollection m_ParametersCollection;

	std::string				m_EffectName;
	std::string				m_FileName;
	LPD3DXEFFECT			m_Effect;
	CEffectTechnique*		m_CurrentTechnique;

public:
	static const uint32 m_MaterialID=NO_ID;
};

#endif
