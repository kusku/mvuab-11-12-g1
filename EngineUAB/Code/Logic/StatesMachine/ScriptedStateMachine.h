#ifndef __CLASS_SCRIPTED_STATE_MACHINE_H__
#define __CLASS_SCRIPTED_STATE_MACHINE_H__

#pragma warning (disable : 4786)

//extern "C"
//{
//  #include <lua.h>
//}
//
//#include <luabind/luabind.hpp>
#include "Scripting\ScriptingDefs.h"
#include <cassert>

//--- Foward Declarations ---//
//class CState;
//---------------------------//


/*!
 * @brief Definicion maquina estados finitos
 *
 * Definicion de una clase para el manejo de los estados definidos
 * para cada una de las entidades del juego. Cada entidad crea una
 * para su uso por la misma.
 *
 */

template <class T>
class CScriptedStateMachine 
{

public:
	/*!
	 *
	 * @param owner
	 * @return
	 */
	CScriptedStateMachine(T* owner) 
		: m_pOwner(owner)
		, m_pCurrentState()
		, m_pPreviousState()
		, m_pGlobalState() 
	{
	}
	/*!
	 * use these methods to initialize the FSM
	 * @param s
	 */
	void SetCurrentState(const luabind::object& s) 
	{
		m_pCurrentState = s;
	}
	/*!
	 *
	 * @param s
	 */
	void SetGlobalState(const luabind::object& s) 
	{
		m_pGlobalState = s;
	}
	/*!
	 *
	 * @param s
	 */
	void SetPreviousState(const luabind::object& s)
	{
		m_pPreviousState = s;
	}
	/*!
	 * call this to update the FSM
	 */
	void Update() {
		if (m_pGlobalState.is_valid()) 
		{
			m_pGlobalState["Execute"](m_pOwner);
		}
		//make sure the state is valid before calling its Execute 'method'
		if (m_pCurrentState.is_valid()) //this could also be written as 'if(m_CurrentState)'
		{
			m_pCurrentState["Execute"](m_pOwner);
		}
	}
	/*!
	 *
	 * @param msg
	 * @return
	 */
	bool HandleMessage(const Telegram& msg)
	{
		if (m_pCurrentState.is_valid() && luabind::call_function<bool>(
				m_pCurrentState["OnMessage"], m_pOwner, msg)) {
			return true;
		}
		if (m_pGlobalState.is_valid() && luabind::call_function<bool>(
				m_pGlobalState["OnMessage"], m_pOwner, msg)) {
			return true;
		}
		return false;
	}
	/*!
	 * change to a new state
	 * @param new_state
	 */
	void ChangeState(const luabind::object& new_state) 
	{

		// keep record of previous state
		m_pPreviousState = m_pCurrentState;

		//call the exit method of the existing state
		m_pCurrentState["Exit"](m_pOwner);

		//change state to the new state
		m_pCurrentState = new_state;

		//call the entry method of the new state
		m_pCurrentState["Enter"](m_pOwner);
	}
	/*!
	 *	volver al estado anterior. Utilizo un objeto intermedio
	 *	para no machacar el valor de m_pPreviousState dentro
	 *	de ChangeState
	 */
	void RevertToPreviousState() 
	{
			luabind::object new_state = m_pPreviousState;
			ChangeState(new_state);
	}
	/*!
	 * retrieve the current state
	 * @return
	 */
	const luabind::object& CurrentState() const
	{
		return m_pCurrentState;
	}
	/*!
	 *
	 * @return
	 */
	const luabind::object& GlobalState() const
	{
		return m_pGlobalState;
	}
	/*!
	 *
	 * @return
	 */
	const luabind::object& PreviousState() const 
	{
		return m_pPreviousState;
	}
	bool isInState(const luabind::object& st) const 
	{
		return m_pCurrentState == st;
	}

private:
	/*!
	 * pointer to the agent that owns this instance
	 */
	T* m_pOwner;
	/*!
	 * @brief Estado actual.
	 *
	 * El estado actual es una tabla de funciones lua. Esta tabla se representa
	 * en C++ como un luabind::object
	 */
	luabind::object m_pCurrentState;
	/*!
	 * @brief Estado anterior.
	 */
	luabind::object m_pPreviousState;
	/*!
	 * @brief Estado global
	 */
	luabind::object m_pGlobalState;

};

#endif __CLASS_SCRIPTED_STATE_MACHINE_H__