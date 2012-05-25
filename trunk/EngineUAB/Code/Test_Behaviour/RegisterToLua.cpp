#include "RegisterToLua.h"
#include "Scripting\ScriptManager.h"

#include "GameProcess.h"

#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\StateMachine.h"
#include "StatesMachine\Telegram.h"
#include "StatesMachine\MessageDispatcher.h"
#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\ScriptedStateMachine.h"

#include "Characters/Character.h"
#include "Character_Wrapper.h"
#include "Object3D.h"
#include "Utils\Named.h"
#include "Utils\TemplatedVectorMapManager.h"

#include "Foo.h"

#include "core.h"
#include "base.h"

void RegisterToLuaTelegram(lua_State* _pLua)
{
	module( _pLua)
		[
			class_<Telegram> ("Telegram")
			.def_readwrite("Msg",&Telegram::Msg)
			.def_readwrite("ExtraInfo", &Telegram::ExtraInfo)
		];
}

void RegisterToLuaMessageDispacher(lua_State* _pLua)
{
	//module(_pLua)
	//	[
	//		class_<CMessageDispatcher> ("Dispatch")
	//		.def("DispatchStateMessage",&MessageDispatcher::DispatchMsg)
	//	];
	//globals( Lua)["Dispatch"] = Dispatch; // Es una class singleton
}

void RegisterToLuaBaseGameEntity(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CBaseGameEntity> ("CBaseGameEntity")
			.def("get_id",&CBaseGameEntity::GetID)
		];
}

void RegisterToLuaCNamed(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CNamed> ("CName")
			
		];
}

void RegisterToLuaCObject3D(lua_State* _pLua)
{
	module(_pLua)
		[
			class_<CObject3D> ("CObject3D")
		];

}

void RegisterToLuaCharacter(lua_State* _pLua)
{
	//module(state) [
	//	class_<CCharacter, character_wrapper, bases<CBaseGameEntity, CObject3D, CNamed>>("CCharacter")
	//		.def(constructor<int>())
	//		.def("update", &CCharacter::Update, &character_wrapper::default_update)
	//		//.def(constructor<int, const std::string&>())
	//];

	module(_pLua)
		[
			class_<CCharacter, CCharacter_Wrapper, bases<CBaseGameEntity, CObject3D, CNamed>> ("CCharacter")
			.def(constructor<int> ())
			.def("update", &CCharacter::Update, &CCharacter_Wrapper::default_update)
			/*.def("get_logic_fsm",&CCharacter::GetLogicFSM)
			.def("get_graphic_fsm",&CCharacter::GetGraphicFSM)*/
		];
}

void RegisterToLuaCharacterManager(lua_State* _pLua)
{
	/*module(_pLua)
		[
			class_<CCharactersManager, bases<CTemplatedVectorMapManager<CCharacter>> > ("CCharactersManager")
				.def("add_enemy", &CCharactersManager::AddEnemy)
		]*/
}

void RegisterToLuaScriptedStateMachine(lua_State* _pLua){
	/*module( _pLua)
		[class_<CScriptedStateMachine<Actor> > ("ScriptedStateMachine")
		.def("ChangeState", &CScriptedStateMachine<Actor>::ChangeState)
		.def("CurrentState", &CScriptedStateMachine<Actor>::CurrentState)
		.def("PreviousState", &CScriptedStateMachine<Actor>::PreviousState)
		.def("GlobasState", &CScriptedStateMachine<Actor>::GlobalState)
		.def("SetCurrentState",&CScriptedStateMachine<Actor>::SetCurrentState)
		.def("SetGlobalState",&CScriptedStateMachine<Actor>::SetGlobalState)
		.def("RevertToPreviousState",&CScriptedStateMachine<Actor>::RevertToPreviousState)
		.def("isInState",&CScriptedStateMachine<Actor>::isInState)
		];*/
}

void RegisterToLuaGlobals(lua_State* _pLua)
{
	globals(_pLua)["ent_caperucita"] = (int)ent_caperucita;
	globals(_pLua)["ent_lobo"] = (int)ent_lobo;

	luabind::module(_pLua) 
		[
			luabind::class_<CFoo>("Foo")
			.def(luabind::constructor<luabind::object, const char*>())
			.def("Think", &CFoo::Think)
			.def("Kill", &CFoo::Kill)
			.def_readwrite("name", &CFoo::m_Name)
		];
}

