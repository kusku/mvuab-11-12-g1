#include "Properties.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// ------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------

CProperties::CProperties( const std::string &_Name, int _Life, int _Strong, float _Speed, float _Height, const Vect3f &_Direction )
	: CNamed				( _Name )
	, CObject3D				( )
	, m_Life				( _Life )
	, m_Strong				( _Strong )
	, m_Speed				( _Speed )
	, m_RotationSpeed		( 360.f )
	, m_Direction			( _Direction )
	, m_RespawnPosition		( Vect3f(0.f,0.f,0.f) )
	, m_DetectionDistance	( 0.f )
	, m_DistanceChase		( 0.f )
	, m_AttackDistance		( 1.f )
	, m_Height				( _Height )
{
}

CProperties::CProperties( void )
	: CNamed				( "" )
	, CObject3D				( )
	, m_Life				( 0 )
	, m_Strong				( 0 )
	, m_Speed				( 0 )
	, m_RotationSpeed		( 360.f )
	, m_Direction			( Vect3f(0.f,0.f,0.f) )
	, m_RespawnPosition		( Vect3f(0.f,0.f,0.f) )
	, m_DetectionDistance	( 0.f )
	, m_DistanceChase		( 0.f )
	, m_AttackDistance		( 1.f )
	, m_Height				( 1.0 )
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
