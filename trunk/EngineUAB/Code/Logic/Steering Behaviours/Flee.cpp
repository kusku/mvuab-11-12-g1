#include "Flee.h"
#include "Steering Behaviours\SteeringBehavioursDefs.h"
#include "Steering Behaviours\SteeringEntity.h"
#include "Scripting\ScriptManager.h"

#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CFlee::CFlee( float _PanicDistance )
	: CSteering			( ::flee )
	, m_PanicDistance	( _PanicDistance )
{
}


CFlee::~CFlee(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect2f CFlee::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
	{
		if ( (_pEntity->GetPosition()).SqDistance(m_Target) > m_PanicDistance )
			return Vect2f ( 0.0f, 0.0f );

		Vect2f l_DesiredVelocity = Vect2f(_pEntity->GetPosition() - m_Target).Normalize() * _pEntity->GetMaxSpeed();

		return (l_DesiredVelocity - _pEntity->GetVelocity());
	}
	else 
	{
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo flee");
		return NULL;
	}
}

// -----------------------------------------
//			     REGISTER METHOD
// -----------------------------------------
void CFlee::RegisterLUAMethods( void )
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CFlee>("CFlee")
	];
}
// -----------------------------------------
//			     PROPERTIES
// -----------------------------------------