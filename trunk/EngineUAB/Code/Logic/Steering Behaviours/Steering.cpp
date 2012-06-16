#include "Steering.h"
#include "Scripting\ScriptManager.h"
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
	, m_Target	( )
{}


CSteering::~CSteering( void )
{}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------

void CSteering::RegisterLUAMethods( void )
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CSteering>("CSteering")
			.property( "type", &CSteering::GetType)
			.property( "target", &CSteering::GetTarget, &CSteering::SetTarget)
	];
}