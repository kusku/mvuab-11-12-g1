#ifndef __CLASS_PARTICLE_DEFINITIONS_H__
#define __CLASS_PARTICLE_DEFINITIONS_H__


#include "Math\Color.h"
#include "Math\Vector3.h"
#include "Base.h"
#include <string>

#include "Graphic States/GraphicStates.h"

struct TParticleSystemSettings
{
	std::string			m_Name;

	std::string			m_TextureName;

	uint32				m_MaxParticles;

	float				m_Duration;;

	float				m_DurationRandomness;

	float				m_EmitterVelocitySensitivity;

	float				m_MinHorizontalVelocity;
	float				m_MaxHorizontalVelocity;

	float				m_MinVerticalVelocity;
	float				m_MaxVerticalVelocity;

	Vect3f				m_Gravity;

	float				m_EndVelocity;

	CColor				m_MinColor;
	CColor				m_MaxColor;

	float				m_MinRotateSpeed;
	float				m_MaxRotateSpeed;

	float				m_MinStartSize;
	float				m_MaxStartSize;

	float				m_MinEndSize;
	float				m_MaxEndSize;

	TGraphicBlendStates	m_BlendState;
	std::string			m_BlendName;

	//Constructor
	TParticleSystemSettings()
		: m_Name("Default")
		, m_TextureName("Default")
		, m_MaxParticles(100)
		, m_Duration(1.0f)
		, m_DurationRandomness(0.0f)
		, m_EmitterVelocitySensitivity(1.0f)
		, m_MinHorizontalVelocity(0.0f)
		, m_MaxHorizontalVelocity(0.0f)
		, m_MinVerticalVelocity(0.0f)
		, m_MaxVerticalVelocity(0.0f)
		, m_Gravity(v3fZERO)
		, m_EndVelocity(1.0f)
		, m_MinColor(colWHITE)
		, m_MaxColor(colWHITE)
		, m_MinRotateSpeed(0.0f)
		, m_MaxRotateSpeed(0.0f)
		, m_MinStartSize(100.0f)
		, m_MaxStartSize(100.0f)
		, m_MinEndSize(100.0f)
		, m_MaxEndSize(100.0f)
		, m_BlendState(TGraphicBlendStates::NonPremultiplied)
		, m_BlendName("NonPremultiplied")
	{
	}

	TParticleSystemSettings(const std::string& name, const std::string textureName, uint32 maxParticles, float duration, float durationRandomness, float emitterVelocitySensitivity,
		float minHorizontalVelocity, float maxHorizontalVelocity, float minVerticalVelocity, float maxVerticalVelocity,
		const Vect3f& gravity, float endVelocity, const CColor& minColor, const CColor& maxColor, float minRotateSpeed,
		float maxRotateSpeed, float minStartSize, float maxStartSize, float minEndSize, float maxEndSize, const TGraphicBlendStates& blendState)
		: m_Name(name)
		, m_TextureName(textureName)
		, m_MaxParticles(maxParticles)
		, m_Duration(duration)
		, m_DurationRandomness(durationRandomness)
		, m_EmitterVelocitySensitivity(emitterVelocitySensitivity)
		, m_MinHorizontalVelocity(minHorizontalVelocity)
		, m_MaxHorizontalVelocity(maxHorizontalVelocity)
		, m_MinVerticalVelocity(minVerticalVelocity)
		, m_MaxVerticalVelocity(maxVerticalVelocity)
		, m_Gravity(gravity)
		, m_EndVelocity(endVelocity)
		, m_MinColor(minColor)
		, m_MaxColor(maxColor)
		, m_MinRotateSpeed(minRotateSpeed)
		, m_MaxRotateSpeed(maxRotateSpeed)
		, m_MinStartSize(minStartSize)
		, m_MaxStartSize(maxStartSize)
		, m_MinEndSize(minEndSize)
		, m_MaxEndSize(maxEndSize)
		, m_BlendState(blendState)
	{
	}
};

#endif