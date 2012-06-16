#include "Properties.h"


// ------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------

CProperties::CProperties( const std::string &_Name, int _Life, int _Strong, int _Speed, const Vect3f &_Direction )
	: CObject3D			( )
	, m_Life			( _Life )
	, m_Strong			( _Strong )
	, m_Speed			( _Speed )
	, m_Direction		( _Direction )
	, m_RespawnPosition	( Vect3f(0.f,0.f,0.f) )
{
	SetName(_Name);
}

CProperties::CProperties( void )
	: CObject3D		( )
	, m_Life		( 0 )
	, m_Strong		( 0 )
	, m_Speed		( 0 )
	, m_Direction	( Vect3f(0.f,0.f,0.f) )
	, m_RespawnPosition	( Vect3f(0.f,0.f,0.f) )
{
}

CProperties::~CProperties(void)
{
}

// ------------------------------------------
//			  M�TODES PRINCIPALS
// ------------------------------------------	


// ------------------------------------------
//				PROPIETATS 
// ------------------------------------------
