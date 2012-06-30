#include "ParticleEmitterSphere.h"
#include "Particles\ParticleSystem.h"
#include "Utils\BoostRandomHelper.h"
#include "Math\Matrix33.h"
#include "Math\MathTypes.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitterSphere::CParticleEmitterSphere(const std::string& _Name, CParticleSystem* _ParticleSystem, float _ParticlesPerSecond, const Vect3f& _InitialPosition, bool _UseDis, float _Radius)
	: CParticleEmitter(_Name, _ParticleSystem, _ParticlesPerSecond, _InitialPosition, _UseDis)
	, m_fRadius(_Radius)
{
}

CParticleEmitterSphere::~CParticleEmitterSphere()
{
}

Vect3f CParticleEmitterSphere::CalculateParticlePosition()
{
	Vect3f l_Point = Vect3f(1.f, 1.f, 1.f);
	float yaw = BoostRandomHelper::GetFloat(e2PI<float>());
	float pitch = BoostRandomHelper::GetFloat(e2PI<float>());
	float scale = BoostRandomHelper::GetFloat(m_fRadius);

	l_Point = l_Point.RotateY(yaw);
	l_Point = l_Point.RotateX(pitch);

	l_Point = l_Point * scale;

	return l_Point;
}
