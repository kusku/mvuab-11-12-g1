
#include <assert.h>
#include "ParticleEmitterRing.h"
#include "Particles\ParticleSystem.h"
#include "Utils\BoostRandomHelper.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


CParticleEmitterRing::CParticleEmitterRing(const std::string& name, CParticleSystem* particleSystem, float particlesPerSecond, const Vect3f& initialPosition, bool horizontal, float radius, float height, float width)
	: CParticleEmitter(name, particleSystem, particlesPerSecond, initialPosition)
	, m_Horizontal(horizontal)
	, m_Radius(radius)
	, m_Height(height)
	, m_Width(width)
{

}


CParticleEmitterRing::~CParticleEmitterRing()
{
}

Vect3f CParticleEmitterRing::CalculateParticlePosition()
{
	float angle = BoostRandomHelper::GetNextFloat() * D3DX_PI * 2;

	float x = (float)mathUtils::Cos(angle);

	if(m_Horizontal)
	{
		float z = (float)mathUtils::Sin(angle);

		return Vect3f(x * m_Radius, 0, z * m_Radius + m_Width);
	}
	else
	{
		float y = (float)mathUtils::Sin(angle);

		return Vect3f(x * m_Radius, y * m_Radius + m_Height, 0);
	}
}
