#include "Seek.h"
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

CSeek::CSeek(void)
	: CSteering		( ::seek )
{
}


CSeek::~CSeek(void)
{
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect2f CSeek::CalculateSteering( CSteeringEntity *_pEntity )	
{
	if (m_Target != NULL)
    {
        Vect2f l_DesiredVelocity = Vect2f(m_Target - _pEntity->GetPosition()).Normalize() * _pEntity->GetMaxSpeed();

        return (l_DesiredVelocity - _pEntity->GetVelocity());
    }
    else 
    {
		LOGGER->AddNewLog( ELL_ERROR, "Debe indicarse el target para el steering de tipo Seek");
		return NULL;
    }
}
   
// -----------------------------------------
//			     REGISTER METHOD
// -----------------------------------------
void CSeek::RegisterLUAMethods( void )
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CSeek>("CSeek")
	];
}
// -----------------------------------------
//			     PROPERTIES
// -----------------------------------------