#include "Steering.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CSteering::CSteering( eBehaviorType _Type )
	: m_Type	( _Type )
	, m_Target	( Vect3f(0,0,0) )
{}


CSteering::~CSteering( void )
{}