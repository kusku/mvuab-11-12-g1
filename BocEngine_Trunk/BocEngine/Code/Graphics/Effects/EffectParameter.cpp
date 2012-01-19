#include <assert.h>

#include "EffectParameter.h"
#include "Effect.h"
#include "Base.h"
#include "Textures\Texture.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CEffectParameter::CEffectParameter(CEffect* parentEffect, D3DXHANDLE effectParameter, const std::string& parameterName, const std::string& parameterSemantic)
{
	assert(parentEffect);
	assert(effectParameter);

	m_ParentEffect = parentEffect;
	m_EffectParameter = effectParameter;
	m_ParameterName = parameterName;
	m_ParameterSemanticName = parameterSemantic;
}

CEffectParameter::~CEffectParameter()
{
}

/***Gets***/
float CEffectParameter::GetFloat() const
{
	float value = false;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetFloat(m_EffectParameter, &value), l_HR);

	return value;
}

Vect2f CEffectParameter::GetVect2f() const
{
	float value[2] = {0.0f, 0.0f};
	Vect2f retValue;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetFloatArray(m_EffectParameter, value, 2), l_HR);

	retValue(value[0], value[1]);

	return retValue;
}

Vect3f CEffectParameter::GetVect3f() const
{
	float value[3]= {0.0f, 0.0f, 0.0f};
	Vect3f retValue;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetFloatArray(m_EffectParameter, value, 3), l_HR);

	retValue(value[0], value[1], value[2]);

	return retValue;
}

Vect4f CEffectParameter::GetVect4f() const
{
	float value[4]= {0.0f, 0.0f, 0.0f, 0.0f};
	Vect4f retValue;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetFloatArray(m_EffectParameter, value, 4), l_HR);

	retValue(value[0], value[1], value[2], value[3]);

	return retValue;
}

int CEffectParameter::GetInt() const
{
	int value = 0;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetInt(m_EffectParameter, &value), l_HR);

	return value;
}

Vect2i CEffectParameter::GetVect2i() const
{
	int value[2] = {0, 0};
	Vect2i retValue;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetIntArray(m_EffectParameter, value, 2), l_HR);

	retValue(value[0], value[1]);

	return retValue;
}

Vect3i CEffectParameter::GetVect3i() const
{
	int value[3] = {0, 0, 0};
	Vect3i retValue;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetIntArray(m_EffectParameter, value, 3), l_HR);

	retValue(value[0], value[1], value[2]);

	return retValue;
}

Vect4i CEffectParameter::GetVect4i() const
{
	int value[4] = {0, 0, 0, 0};
	Vect4i retValue;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetIntArray(m_EffectParameter, value, 4), l_HR);

	retValue(value[0], value[1], value[2], value[3]);

	return retValue;
}

Mat44f CEffectParameter::GetMatrix() const
{
	D3DXMATRIX value;
	Mat44f retValue;
	HRESULT l_HR;
	
	HRDX(m_ParentEffect->m_Effect->GetMatrix(m_EffectParameter, &value), l_HR);

	retValue = Mat44f(value);

	return retValue;
}

std::string CEffectParameter::GetString() const
{
	LPCSTR value;
	std::string retValue;
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->GetString(m_EffectParameter, &value), l_HR);

	retValue = std::string(value);

	return value;
}

CTexture* CEffectParameter::GetTexture() const
{
	LPDIRECT3DBASETEXTURE9 value;
	CTexture* retValue;
	HRESULT l_HR;
	
	HRDX(m_ParentEffect->m_Effect->GetTexture(m_EffectParameter, &value), l_HR);

	retValue = new CTexture();
	retValue->SetDXTexture( LPDIRECT3DTEXTURE9(value) );
	
	return retValue;
}

CColor CEffectParameter::GetColor() const
{
	Vect4f value = GetVect4f();
	CColor retValue(value);

	return retValue;
}
	
/***Sets***/
bool CEffectParameter::SetFloat(float value)
{
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->SetFloat(m_EffectParameter, value), l_HR);

	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetVect2f(Vect2f value)
{
	HRESULT l_HR;
	float valueArray[2] = {value[0], value[1]};

	HRDX(m_ParentEffect->m_Effect->SetFloatArray(m_EffectParameter, valueArray, 2), l_HR);

	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetVect3f(Vect3f value)
{
	HRESULT l_HR;
	float valueArray[3] = {value[0], value[1], value[2]};

	HRDX(m_ParentEffect->m_Effect->SetFloatArray(m_EffectParameter, valueArray, 3), l_HR);

	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetVect4f(Vect4f value)
{
	HRESULT l_HR;
	float valueArray[4] = {value[0], value[1], value[2], value[3]};

	HRDX(m_ParentEffect->m_Effect->SetFloatArray(m_EffectParameter, valueArray, 4), l_HR);

	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetInt(int value)
{
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->SetInt(m_EffectParameter, value), l_HR);

	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetVect2i(Vect2i value)
{
	HRESULT l_HR;
	int valueArray[2] = {value[0], value[1]};

	HRDX(m_ParentEffect->m_Effect->SetIntArray(m_EffectParameter, valueArray, 2), l_HR);

	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetVect3i(Vect3i value)
{
	HRESULT l_HR;
	int valueArray[3] = {value[0], value[1], value[2]};

	HRDX(m_ParentEffect->m_Effect->SetIntArray(m_EffectParameter, valueArray, 3), l_HR);

	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetVect4i(Vect4i value)
{
	HRESULT l_HR;
	int valueArray[4] = {value[0], value[1], value[2], value[3]};

	HRDX(m_ParentEffect->m_Effect->SetIntArray(m_EffectParameter, valueArray, 4), l_HR);

	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetMatrix(Mat44f value)
{
	HRESULT l_HR;
	D3DXMATRIX valueDX = value.GetD3DXMatrix();

	HRDX(m_ParentEffect->m_Effect->SetMatrix(m_EffectParameter, &valueDX), l_HR);
	
	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetString(const std::string& value)
{
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->SetString(m_EffectParameter, value.c_str()), l_HR);
	
	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetTexture(CTexture* value)
{
	HRESULT l_HR;

	HRDX(m_ParentEffect->m_Effect->SetTexture(m_EffectParameter, value->GetDXTexture()), l_HR);
	
	return (SUCCEEDED(l_HR));
}

bool CEffectParameter::SetColor(CColor value)
{
	Vect4f valueVect = value.GetArgb();

	return SetVect4f(valueVect);
}
