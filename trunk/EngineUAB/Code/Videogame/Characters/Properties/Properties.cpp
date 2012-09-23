#include "Properties.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// ------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------

CProperties::CProperties( const std::string &_Name, int _Life, int _Strong, float _Speed, float _HeightController, float _WidthController, 
							 float _Slope, float _SkinWidth, float _StepOffset, const Vect3f &_Velocity, const Vect3f &_Position )
	: CObject3D				( _Position, Vect3f(1.0f,1.0f,1.0f) , 0.0f, 0.0f, 0.0f  )
	, m_Mass				( 1 )
	, m_Life				( _Life )
	, m_Strong				( _Strong )
	, m_RespawnPosition		( Vect3f(0.f,0.f,0.f) )
	, m_BoundingRadius		( 1.f )
	, m_Velocity			( _Velocity )
	, m_Speed				( _Speed )
	, m_MaxRotationSpeed	( 360.f )
	, m_MinSpeed			( 1.f )
	, m_MaxSpeed			( 1.f )
	, m_MinAcceleration		( 1.f )
	, m_MaxAcceleration		( 1.f )
	, m_AccelerationAttack1	( 1.f )
	, m_AccelerationAttack2	( 1.f )
	, m_AccelerationAttack3	( 1.f )
	, m_MaxForce			( 1.f )
	, m_DetectionDistance	( 0.f )
	, m_ChaseDistance		( 0.f )
	, m_FollowPathDistance	( 0.f )
	, m_AttackDistance		( 1.f )
	, m_AttackAngle			( 0.f )
	, m_HeightController	( _HeightController )
	, m_WidthController		( _WidthController )
	, m_Slope				( _Slope )	
	, m_SkinWidth			( _SkinWidth )
	, m_StepOffset			( _StepOffset )
{
	SetName(_Name);
}

CProperties::CProperties( void )
	: CObject3D				( NULL, NULL, 0.0f, 0.0f, 0.0f  )
	, m_Mass				( 1 )
	, m_Life				( 0 )
	, m_Strong				( 0 )
	, m_RespawnPosition		( Vect3f(0.f,0.f,0.f) )
	, m_BoundingRadius		( 1.f )
	, m_Velocity			( Vect3f(0.f,0.f,0.f) )
	, m_Speed				( 1.f )
	, m_MaxRotationSpeed	( 360.f )
	, m_MinSpeed			( 1.f )
	, m_MaxSpeed			( 1.f )
	, m_MinAcceleration		( 1.f )
	, m_MaxAcceleration		( 1.f )
	, m_AccelerationAttack1	( 1.f )
	, m_AccelerationAttack2	( 1.f )
	, m_AccelerationAttack3	( 1.f )
	, m_MaxForce			( 1.f )
	, m_DetectionDistance	( 0.f )
	, m_ChaseDistance		( 0.f )
	, m_FollowPathDistance	( 0.f )
	, m_AttackDistance		( 1.f )
	, m_AttackAngle			( 0.f )
	, m_HeightController	( 1.f )
	, m_WidthController		( 1.f )
	, m_Slope				( 45.f )	
	, m_SkinWidth			( 0.1f )
	, m_StepOffset			( 0.5f )
	
{
}

CProperties::~CProperties(void)
{
}

// ------------------------------------------
//			  MÈTODES PRINCIPALS
// ------------------------------------------	


// ------------------------------------------
//				PROPIETATS 
// ------------------------------------------
