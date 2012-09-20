#include "stdafx.h"
#include "PropertiesSaver.h"
#include "Graphic States\GraphicStates.h"

CPropertiesSaver::CPropertiesSaver()
	: m_pProperties(NULL)
{

}

CPropertiesSaver::~CPropertiesSaver()
{

}

void CPropertiesSaver::CleanUp()
{

}

CPropertiesSaver* CPropertiesSaver::m_pPropertiesSaver = NULL;

CPropertiesSaver* CPropertiesSaver::GetInstance()
{
	if( m_pPropertiesSaver == NULL )
	{
		m_pPropertiesSaver  = new CPropertiesSaver();
	}

	return m_pPropertiesSaver;
}

void CPropertiesSaver::SaveProperties( TParticleSystemSettings *_ParticleSystem )
{
	assert(m_pProperties);
	assert(_ParticleSystem);
	
	COleVariant l_Value;

	// Color Properties
	//-----------------------------------------------
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(0)->GetSubItem(0)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_MinColor.SetRed(static_cast<float>(l_Value.intVal) / 255.f);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(0)->GetSubItem(0)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_MinColor.SetGreen(static_cast<float>(l_Value.intVal) / 255.f);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(0)->GetSubItem(0)->GetSubItem(2)->GetValue();
	_ParticleSystem->m_MinColor.SetBlue(static_cast<float>(l_Value.intVal) / 255.f);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(0)->GetSubItem(0)->GetSubItem(3)->GetValue();
	_ParticleSystem->m_MinColor.SetAlpha(static_cast<float>(l_Value.intVal) / 255.f);

	l_Value = CPropertiesSaver::m_pProperties->GetProperty(0)->GetSubItem(1)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_MaxColor.SetRed(static_cast<float>(l_Value.intVal) / 255.f);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(0)->GetSubItem(1)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_MaxColor.SetGreen(static_cast<float>(l_Value.intVal) / 255.f);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(0)->GetSubItem(1)->GetSubItem(2)->GetValue();
	_ParticleSystem->m_MaxColor.SetBlue(static_cast<float>(l_Value.intVal) / 255.f);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(0)->GetSubItem(1)->GetSubItem(3)->GetValue();
	_ParticleSystem->m_MaxColor.SetAlpha(static_cast<float>(l_Value.intVal) / 255.f);

	// Duration Properties
	//-----------------------------------------------
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(1)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_Duration = static_cast<float>(l_Value.fltVal);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(1)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_DurationRandomness = static_cast<float>(l_Value.fltVal);

	// General Properties
	//-----------------------------------------------
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(2)->GetSubItem(0)->GetValue();
	std::string l_Texture = std::string( _bstr_t( l_Value.bstrVal ));
	if( l_Texture != "" )
	{
		_ParticleSystem->m_TextureName = l_Texture;
	}

	l_Value = CPropertiesSaver::m_pProperties->GetProperty(2)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_MaxParticles = l_Value.intVal;

	l_Value = CPropertiesSaver::m_pProperties->GetProperty(2)->GetSubItem(2)->GetValue();
	std::string l_Blend = std::string( _bstr_t( l_Value.bstrVal ) );
	if( l_Blend == "NonPremultiplied" )
	{
		_ParticleSystem->m_BlendState = TGraphicBlendStates::NonPremultiplied;
	}
	else if( l_Blend == "Additive" )
	{
		_ParticleSystem->m_BlendState = TGraphicBlendStates::Additive;
	}
	else
	{
		_ParticleSystem->m_BlendState = TGraphicBlendStates::DefaultState;
	}
	_ParticleSystem->m_BlendName = l_Blend;

	// Rotate Speed Properties
	//-----------------------------------------------
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(3)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_MinRotateSpeed = static_cast<float>(l_Value.fltVal);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(3)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_MaxRotateSpeed = static_cast<float>(l_Value.fltVal);

	// Size Properties
	//-----------------------------------------------
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(4)->GetSubItem(0)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_MinStartSize = static_cast<float>(l_Value.fltVal);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(4)->GetSubItem(0)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_MaxStartSize = static_cast<float>(l_Value.fltVal);

	l_Value = CPropertiesSaver::m_pProperties->GetProperty(4)->GetSubItem(1)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_MinEndSize = static_cast<float>(l_Value.fltVal);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(4)->GetSubItem(1)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_MaxEndSize = static_cast<float>(l_Value.fltVal);

	// Velocity Properties
	//-----------------------------------------------
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_EmitterVelocitySensitivity = static_cast<float>(l_Value.fltVal);

	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(1)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_MinHorizontalVelocity = static_cast<float>(l_Value.fltVal);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(1)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_MaxHorizontalVelocity = static_cast<float>(l_Value.fltVal);

	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(2)->GetSubItem(0)->GetValue();
	_ParticleSystem->m_MinVerticalVelocity = static_cast<float>(l_Value.fltVal);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(2)->GetSubItem(1)->GetValue();
	_ParticleSystem->m_MaxVerticalVelocity = static_cast<float>(l_Value.fltVal);

	Vect3f l_Gravity;
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(3)->GetSubItem(0)->GetValue();
	l_Gravity.x = static_cast<float>(l_Value.fltVal);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(3)->GetSubItem(1)->GetValue();
	l_Gravity.y = static_cast<float>(l_Value.fltVal);
	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(3)->GetSubItem(1)->GetValue();
	l_Gravity.z = static_cast<float>(l_Value.fltVal);

	l_Value = CPropertiesSaver::m_pProperties->GetProperty(5)->GetSubItem(4)->GetValue();
	_ParticleSystem->m_EndVelocity = static_cast<float>(l_Value.fltVal);
}

void CPropertiesSaver::LoadProperties( TParticleSystemSettings *_ParticleSystem )
{
	assert(m_pProperties);
	assert(_ParticleSystem);

	//---------------------------------------------------
	m_pProperties->RemoveAll();

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pProperty = NULL;

	//---------------------------------------------------
	CColor l_minColor = _ParticleSystem->m_MinColor;
	CColor l_maxColor = _ParticleSystem->m_MaxColor;

	CMFCPropertyGridProperty *l_pColor = new CMFCPropertyGridProperty(_T("Color"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Minimum"), 0, TRUE);
	CMFCPropertyGridProperty *l_pRed	= new CMFCPropertyGridProperty(_T("Red"), (_variant_t)(int)(l_minColor.GetRed() * 255.f), _T("Red channel for the minimum color."));
	CMFCPropertyGridProperty *l_pGreen	= new CMFCPropertyGridProperty(_T("Green"), (_variant_t)(int)(l_minColor.GetGreen() * 255.f), _T("Green channel for the minimum color."));
	CMFCPropertyGridProperty *l_pBlue	= new CMFCPropertyGridProperty(_T("Blue"), (_variant_t)(int)(l_minColor.GetBlue() * 255.f), _T("Blue channel for the minimum color."));
	CMFCPropertyGridProperty *l_pAlpha	= new CMFCPropertyGridProperty(_T("Alpha"), (_variant_t)(int)(l_minColor.GetAlpha() * 255.f), _T("Alpha channel for the minimum color."));
	l_pRed->EnableSpinControl(TRUE, 0, 255);
	l_pGreen->EnableSpinControl(TRUE, 0, 255);
	l_pBlue->EnableSpinControl(TRUE, 0, 255);
	l_pAlpha->EnableSpinControl(TRUE, 0, 255);
	l_pProperty->AddSubItem(l_pRed);
	l_pProperty->AddSubItem(l_pGreen);
	l_pProperty->AddSubItem(l_pBlue);
	l_pProperty->AddSubItem(l_pAlpha);
	l_pColor->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Maximum"), 0, TRUE);
	l_pRed		= new CMFCPropertyGridProperty(_T("Red"), (_variant_t)(int)(l_maxColor.GetRed() * 255.f), _T("Red channel for the maximum color."));
	l_pGreen	= new CMFCPropertyGridProperty(_T("Green"), (_variant_t)(int)(l_maxColor.GetGreen() * 255.f), _T("Green channel for the maximum color."));
	l_pBlue		= new CMFCPropertyGridProperty(_T("Blue"), (_variant_t)(int)(l_maxColor.GetBlue() * 255.f), _T("Blue channel for the maximum color."));
	l_pAlpha	= new CMFCPropertyGridProperty(_T("Alpha"), (_variant_t)(int)(l_maxColor.GetAlpha() * 255.f), _T("Alpha channel for the maximum color."));
	l_pRed->EnableSpinControl(TRUE, 0, 255);
	l_pGreen->EnableSpinControl(TRUE, 0, 255);
	l_pBlue->EnableSpinControl(TRUE, 0, 255);
	l_pAlpha->EnableSpinControl(TRUE, 0, 255);
	l_pProperty->AddSubItem(l_pRed);
	l_pProperty->AddSubItem(l_pGreen);
	l_pProperty->AddSubItem(l_pBlue);
	l_pProperty->AddSubItem(l_pAlpha);
	l_pColor->AddSubItem(l_pProperty);
	m_pProperties->AddProperty(l_pColor);
	
	//---------------------------------------------------_
	CMFCPropertyGridProperty *l_pDuration = new CMFCPropertyGridProperty(_T("Duration"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Duration"), (_variant_t)_ParticleSystem->m_Duration, _T("Duration of the particle."));
	l_pDuration->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Randomness"), (_variant_t)_ParticleSystem->m_DurationRandomness, _T("Duration randomness of the particle."));
	l_pDuration->AddSubItem(l_pProperty);
	m_pProperties->AddProperty(l_pDuration);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pGeneral = new CMFCPropertyGridProperty(_T("General"));
	static const TCHAR l_Filter[] = _T("PNG(*.png)|*.png|JPG(*.jpg)|*.jpg|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|DDS(*.dds)|*.dds|Todos los archivos(*.*)|*.*||");
	l_pGeneral->AddSubItem(new CMFCPropertyGridFileProperty(_T("Texture"), TRUE, _T(_ParticleSystem->m_TextureName.c_str()), _T("png"), 0, l_Filter, _T("Specify the texture.")));

	l_pProperty = new CMFCPropertyGridProperty(_T("Quantity"), (_variant_t)_ParticleSystem->m_MaxParticles, _T("Maximum of particles for emitter."));
	l_pProperty->EnableSpinControl(TRUE, 0, 10000);
	l_pGeneral->AddSubItem(l_pProperty);

	//TODO: Posar el blend corresponent
	l_pProperty = new CMFCPropertyGridProperty(_T("Blend"), _T(_ParticleSystem->m_BlendName.c_str()), _T("Type of blend."));
	l_pProperty->AddOption(_T("DefaultState"));
	l_pProperty->AddOption(_T("NonPremultiplied"));
	l_pProperty->AddOption(_T("Additive"));
	l_pGeneral->AddSubItem(l_pProperty);
	m_pProperties->AddProperty(l_pGeneral);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pSpeed = new CMFCPropertyGridProperty(_T("Rotate Speed"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Minimum"), (_variant_t)_ParticleSystem->m_MinRotateSpeed, _T("Minimum rotation speed."));
	l_pSpeed->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Maximum"), (_variant_t)_ParticleSystem->m_MaxRotateSpeed, _T("Maximum rotation speed."));
	l_pSpeed->AddSubItem(l_pProperty);
	m_pProperties->AddProperty(l_pSpeed);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pSize = new CMFCPropertyGridProperty(_T("Size"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Start"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)_ParticleSystem->m_MinStartSize, _T("Minimum start size.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)_ParticleSystem->m_MaxStartSize, _T("Maximum start size.")));
	l_pSize->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("End"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)_ParticleSystem->m_MinEndSize, _T("Minimum end size.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)_ParticleSystem->m_MaxEndSize, _T("Maximum end size.")));
	l_pSize->AddSubItem(l_pProperty);
	m_pProperties->AddProperty(l_pSize);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pVelocity = new CMFCPropertyGridProperty(_T("Velocity"));
	l_pProperty->Expand(true);
	l_pProperty = new CMFCPropertyGridProperty(_T("Sensitivity"), (_variant_t)_ParticleSystem->m_EmitterVelocitySensitivity, _T("Sensitivity of the velocity."));
	l_pVelocity->AddSubItem(l_pProperty);	
	l_pProperty = new CMFCPropertyGridProperty(_T("Horizontal"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)_ParticleSystem->m_MinHorizontalVelocity, _T("Minimum horizontal velocity.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)_ParticleSystem->m_MaxHorizontalVelocity, _T("Maximum horizontal velocity.")));
	l_pVelocity->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Vertical"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)_ParticleSystem->m_MinVerticalVelocity, _T("Minimum vertical velocity.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)_ParticleSystem->m_MaxVerticalVelocity, _T("Maximum vertical velocity.")));
	
	l_pVelocity->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Gravity"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t)_ParticleSystem->m_Gravity.x, _T("Gravity in the X axis.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t)_ParticleSystem->m_Gravity.y, _T("Gravity in the Y axis.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t)_ParticleSystem->m_Gravity.z, _T("Gravity in the Z axis.")));
	l_pVelocity->AddSubItem(l_pProperty);	
	l_pProperty = new CMFCPropertyGridProperty(_T("End"), (_variant_t)_ParticleSystem->m_EndVelocity, _T("Final velocity."));
	l_pVelocity->AddSubItem(l_pProperty);	
	m_pProperties->AddProperty(l_pVelocity);
	m_pProperties->ExpandAll(true);
}