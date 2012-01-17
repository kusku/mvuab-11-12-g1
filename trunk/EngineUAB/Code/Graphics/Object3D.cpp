#include "Object3D.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CObject3D::CObject3D(const Vect3f& pos, const Vect3f& scale, float yaw, float pitch, float roll)
	: m_Position(pos)
	, m_Scale(scale)
	, m_fYaw(yaw)
	, m_fPitch(pitch)
	, m_fRoll(roll)
	, m_Visible(true)
{

}

CObject3D::CObject3D()
	: m_Position(Vect3f(0.f,0.f,0.f))
	, m_Scale(Vect3f(0.f,0.f,0.f))
	, m_fYaw(0.f)
	, m_fPitch(0.f)
	, m_fRoll(0.f)
{
}