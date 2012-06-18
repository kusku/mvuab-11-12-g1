function trigger_end_game()
	change_to_end_gui_process()
end

function trigger_inicio()
	get_game_process():get_character_manager():get_enemy_by_name('enemy2').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy3').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy4').enable = true
end

function trigger_cascada()
	get_game_process():get_character_manager():get_enemy_by_name('enemy5').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy6').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy7').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy8').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy9').enable = true
end

function trigger_puente()
	get_game_process():get_character_manager():get_enemy_by_name('enemy10').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy11').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy12').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy13').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy14').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy15').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy16').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy17').enable = true
end

function trigger_casa()
	get_game_process():get_character_manager():get_enemy_by_name('enemy18').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy19').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy20').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy21').enable = true
	get_game_process():get_character_manager():get_enemy_by_name('enemy1').enable = true
end
