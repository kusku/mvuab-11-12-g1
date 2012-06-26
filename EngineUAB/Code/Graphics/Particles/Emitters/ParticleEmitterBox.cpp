#include "ParticleEmitterBox.h"
#include "Particles\ParticleSystem.h"
#include "Utils\BoostRandomHelper.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitterBox::CParticleEmitterBox(const std::string& _Name, CParticleSystem* _ParticleSystem, float _ParticlesPerSecond, const Vect3f& _InitialPosition, bool _UseDis, const Vect3f& _BoxMin, const Vect3f& _BoxMax)
	: CParticleEmitter(_Name, _ParticleSystem, _ParticlesPerSecond, _InitialPosition, _UseDis)
	, m_BoxMin(_BoxMin)
	, m_BoxMax(_BoxMax)
{
}

CParticleEmitterBox::~CParticleEmitterBox()
{
}

const Vect3f& CParticleEmitterBox::CalculateParticlePosition()
{
	Vect3f l_Point;

	l_Point.x = BoostRandomHelper::GetFloat(m_BoxMin.x, m_BoxMax.x);
	l_Point.y = BoostRandomHelper::GetFloat(m_BoxMin.y, m_BoxMax.y);
	l_Point.z = BoostRandomHelper::GetFloat(m_BoxMin.z, m_BoxMax.z);

	return l_Point;
}
