#include "Object3D.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CObject3D::CObject3D( const Vect3f& _Pos, const Vect3f& _Scale, float _Yaw, float _Pitch, float _Roll )
	: m_Position		( _Pos )
	, m_Scale			( _Scale )
	, m_fYaw			( _Yaw )
	, m_fPitch			( _Pitch )
	, m_fRoll			( _Roll )
	, m_Visible			( true)	
{
	m_TechniqueName = "";
}

CObject3D::CObject3D()
	: m_Position	( Vect3f(0.f,0.f,0.f) )
	, m_Scale		( Vect3f(1.f,1.f,1.f) )
	, m_fYaw		( 0.f )
	, m_fPitch		( 0.f )
	, m_fRoll		( 0.f )
	, m_Visible		( true )	
{
	m_TechniqueName = "";
}

const Mat44f CObject3D::GetTransform() const
{
	Mat44f world		= m44fIDENTITY;
	Mat44f scale		= m44fIDENTITY;
	Mat44f rotationY	= m44fIDENTITY;
	Mat44f rotationX	= m44fIDENTITY;
	Mat44f rotationZ	= m44fIDENTITY;
	Mat44f translation	= m44fIDENTITY;

	translation.SetFromPos	(m_Position);
	scale.SetFromScale		(m_Scale.x, m_Scale.y, m_Scale.z);
	rotationX.SetFromAngleX	(mathUtils::Deg2Rad<float>(m_fPitch));
	rotationY.SetFromAngleY	(mathUtils::Deg2Rad<float>(m_fYaw));
	rotationZ.SetFromAngleZ	(mathUtils::Deg2Rad<float>(m_fRoll));
	
	//World = Scale * Rotation * Translation
	world = translation * scale * (rotationY * rotationX * rotationZ);

	return world;
}

void CObject3D::InitMat44()
{

}