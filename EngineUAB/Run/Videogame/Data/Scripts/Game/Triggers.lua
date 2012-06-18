function trigger_end_game()
	change_to_end_gui_process()
end

function trigger_inicio()
	get_game_process():get_character_manager():get_enemy_by_name('enemy1').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy2').enable = true
end

function trigger_cascada()
end

function trigger_puente()
end

function trigger_casa()
end
