#include "ParticleEmitterBox.h"
#include "Particles\ParticleSystem.h"
#include "Utils\BoostRandomHelper.h"
#include <d3dx9.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CParticleEmitterBox::CParticleEmitterBox(const std::string& _Name, CParticleSystem* _ParticleSystem, float _ParticlesPerSecond, const Vect3f& _InitialPosition, bool _UseDis, const Vect3f& _BoxMin, const Vect3f& _BoxMax, bool useRotation, float yaw, float pitch, float roll)
	: CParticleEmitter(_Name, _ParticleSystem, _ParticlesPerSecond, _InitialPosition, _UseDis)
	, m_BoxMin(_BoxMin)
	, m_BoxMax(_BoxMax)
	, m_UseRotation(useRotation)
	, m_Yaw(yaw)
	, m_Pitch(pitch)
	, m_Roll(roll)
{
}

CParticleEmitterBox::~CParticleEmitterBox()
{
}

Vect3f CParticleEmitterBox::CalculateParticlePosition()
{
	Vect3f l_Point;

	l_Point.x = BoostRandomHelper::GetFloat(m_BoxMin.x, m_BoxMax.x);
	l_Point.y = BoostRandomHelper::GetFloat(m_BoxMin.y, m_BoxMax.y);
	l_Point.z = BoostRandomHelper::GetFloat(m_BoxMin.z, m_BoxMax.z);

	if(m_UseRotation)
	{
		D3DXMATRIX matRotX, matRotY, matRotZ, matRes;
		
		D3DXMatrixRotationX(&matRotX, mathUtils::Deg2Rad(m_Pitch));
		D3DXMatrixRotationZ(&matRotZ, mathUtils::Deg2Rad(m_Roll));
		D3DXMatrixRotationY(&matRotY, mathUtils::Deg2Rad(m_Yaw));

		D3DXMatrixMultiply(&matRes, &matRotY, &matRotX);
		D3DXMatrixMultiply(&matRes, &matRes, &matRotZ);

		D3DXVECTOR3 pos(l_Point.x, l_Point.y, l_Point.z);
		D3DXVECTOR4 posTrans(0.0f, 0.0f, 0.0f, 0.0f);

		D3DXVec3Transform(&posTrans, &pos, &matRes);

		l_Point.x = posTrans.x;
		l_Point.y = posTrans.y;
		l_Point.z = posTrans.z;
	}

	return l_Point;
}
