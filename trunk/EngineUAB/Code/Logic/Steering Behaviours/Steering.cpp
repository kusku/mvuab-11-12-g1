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

CSteering::CSteering( BehaviorType _Type )
	: m_Type ( _Type )
{}


CSteering::~CSteering( void )
{}

// -----------------------------------------
//			  M�TODES PRINCIPALS
// -----------------------------------------

void CSteering::RegisterLUAMethods( void )
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		class_<CSteering>("CSteering")
			
	];
}