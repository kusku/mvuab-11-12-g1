function init_game_data()
	player = CPlayer()
	player:init("Player", Vect3f(0.0, 0.0, 0.0), 0)
	
	get_game_process():get_character_manager():set_player(player)
end