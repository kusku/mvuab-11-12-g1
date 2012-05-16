function change_to_game_process()
	load_data()
	core:get_process():change_process()
end

function close_game()
	core:get_process():exit_game()
end