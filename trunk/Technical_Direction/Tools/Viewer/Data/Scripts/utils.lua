core = CSingleton_CCore.get_singleton()
_guiM = core:get_gui_manager()
_soundM = core:get_sound_manager()
_dispatchM = core:get_message_dispatchers()
options = nil

function init_game_utils()
	options = CSingleton_COptions.get_singleton()
end
