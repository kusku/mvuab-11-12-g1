#include <algorithm>
#include "SteeringBehaviours.h"

#include "Scripting\ScriptManager.h"
#include "Base.h"

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CSteeringBehaviours::CSteeringBehaviours(void)
{
}


CSteeringBehaviours::~CSteeringBehaviours(void)
{
}


// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CSteeringBehaviours::Initialize( void )
{
	m_behaviors.clear();
}

void CSteeringBehaviours::Destroy( void )
{
	m_behaviors.clear();
}

//template<typename T>
//void CSteeringBehaviours::AddBehavior( Steering _steering ) 
//{
//    if(!this.HasBehavior(steering.Type))
//        _behaviors.Add(steering.Type);
//
//    this._seek = steering;
//	//<T>::
//}


// -----------------------------------------
//				MÈTODES
// -----------------------------------------

void CSteeringBehaviours::RegisterLUAMethods( void )
{
	lua_State* l_pLuaState = SCRIPT->GetLuaState();
	
	module(l_pLuaState) [
		class_<CSteeringBehaviours>("CSteeringBehaviours")
			.def("has_behavior", &CSteeringBehaviours::HasBehavior)
	];
}

// -----------------------------------------
//				PROPERTIES
// -----------------------------------------

int CSteeringBehaviours::HasBehavior( BehaviorType _Type )  
{
	tBehaviourType::iterator l_Found = m_behaviors.begin();
	
	int l_NumBehavioursFound = 0;

	while (true) 
	{
		l_Found = std::find ( l_Found, m_behaviors.end(), _Type );
		if (l_Found == m_behaviors.end()) break;

		l_NumBehavioursFound++;
		l_Found++;
	}
    
	return l_NumBehavioursFound;
}

