function change_to_game_process()
	core:get_process():load_game_data()
	--core:get_process():change_process()
end

function change_to_game_over_gui_process()
	get_game_process():change_to_gui_process()
	core:get_gui_manager():pop_windows()
	core:get_gui_manager():push_windows('GameOver')
end

function change_to_end_gui_process()
	get_game_process():change_to_gui_process()
	core:get_gui_manager():pop_windows()
	core:get_gui_manager():push_windows('End')
end

function close_game()
	core:get_process():exit_game()
end