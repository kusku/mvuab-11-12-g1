#include "Properties.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// ------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------

CProperties::CProperties( const std::string &_Name, int _Life, int _Strong, float _Speed, float _HeightController, float _WidthController, 
							 float _Slope, float _SkinWidth, float _StepOffset, const Vect3f &_Direction, const Vect3f &_Position )
	: CNamed				( _Name )
	, CObject3D				( _Position, Vect3f(1.0f,1.0f,1.0f) , 0.0f, 0.0f, 0.0f  )
	, m_Life				( _Life )
	, m_Strong				( _Strong )
	, m_Speed				( _Speed )
	, m_RotationSpeed		( 360.f )
	, m_Direction			( _Direction )
	, m_RespawnPosition		( Vect3f(0.f,0.f,0.f) )
	, m_DetectionDistance	( 0.f )
	, m_DistanceChase		( 0.f )
	, m_AttackDistance		( 1.f )
	, m_HeightController	( _HeightController )
	, m_WidthController		( _WidthController )
	, m_Slope				( _Slope )	
	, m_SkinWidth			( _SkinWidth )
	, m_StepOffset			( _StepOffset )
{
}

CProperties::CProperties( void )
	: CNamed				( "" )
	, CObject3D				( NULL, NULL, 0.0f, 0.0f, 0.0f  )
	, m_Life				( 0 )
	, m_Strong				( 0 )
	, m_Speed				( 0 )
	, m_RotationSpeed		( 360.f )
	, m_Direction			( Vect3f(0.f,0.f,0.f) )
	, m_RespawnPosition		( Vect3f(0.f,0.f,0.f) )
	, m_DetectionDistance	( 0.f )
	, m_DistanceChase		( 0.f )
	, m_AttackDistance		( 1.f )
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
