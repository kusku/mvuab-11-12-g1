#include "_ScriptAPI/SoundRegisterScript.h"
#include "Scripting/ScriptManager.h"
#include "SoundManager.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

namespace Sound
{
namespace ScriptAPI
{
	void RegisterScript(lua_State *_pLua)
	{
		module(_pLua) 
			[
				class_<CSoundManager>("CSoundManager")
					.def("play_event", (void(CSoundManager::*) (const std::string &)) &CSoundManager::PlayEvent)
					.def("play_event", (void(CSoundManager::*) (const std::string &, const std::string &)) &CSoundManager::PlayEvent)
					.def("set_switch", &CSoundManager::SetSwitch)
					.def("set_rtpc", &CSoundManager::SetRTPCValue)
					.def("stop_all_events", &CSoundManager::StopAllEvents)
					.def("pause_all_events", &CSoundManager::PauseAllEvents)
					.def("resume_all_events", &CSoundManager::ResumeAllEvents)
			];
	}

} //namespace ScriptAPI
} //namespace Sound