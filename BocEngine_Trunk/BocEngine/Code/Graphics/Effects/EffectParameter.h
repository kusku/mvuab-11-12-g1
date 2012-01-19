
#pragma once

#ifndef EFFECT_PARAMTER_H
#define EFFECT_PARAMTER_H

#include <d3d9.h>
#include <string>
#include "Utils\Types.h"
#include "Math\Matrix44.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Math\Color.h"

class CEffect;
class CTexture;

class CEffectParameter
{
private:
	const CEffect* m_ParentEffect;
	D3DXHANDLE m_EffectParameter;
	std::string m_ParameterName;
	std::string m_ParameterSemanticName;

public:
	CEffectParameter(CEffect* parentEffect, D3DXHANDLE effectParameter, const std::string& parameterName, const std::string& parameterSemantic);

	const std::string&	GetName		() const		{return m_ParameterName;}
	const std::string&	GetSemantic	() const		{return m_ParameterSemanticName;}

	/***Gets***/
	float		GetFloat() const;
	Vect2f		GetVect2f() const;
	Vect3f		GetVect3f() const;
	Vect4f		GetVect4f() const;
	int			GetInt() const;
	Vect2i		GetVect2i() const;
	Vect3i		GetVect3i() const;
	Vect4i		GetVect4i() const;
	Mat44f		GetMatrix() const;
	std::string	GetString() const;
	CTexture*	GetTexture() const;
	CColor		GetColor() const;
	
	/***Sets***/
	bool SetFloat(float value);
	bool SetVect2f(Vect2f value);
	bool SetVect3f(Vect3f value);
	bool SetVect4f(Vect4f value);
	bool SetInt(int value);
	bool SetVect2i(Vect2i value);
	bool SetVect3i(Vect3i value);
	bool SetVect4i(Vect4i value);
	bool SetMatrix(Mat44f value);
	bool SetString(const std::string& value);
	bool SetTexture(CTexture* value);
	bool SetColor(CColor color);

	~CEffectParameter();
};

#endif EFFECT_PARAMTER_H