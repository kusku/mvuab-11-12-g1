function init_game_data()
	--[[local process = get_game_process()

	player = CPlayer()
	player:init("Player", Vect3f(0.0, 3.5, 0.0), 0)
	process:get_character_manager():set_player(player)
	
	--Set Camera
	process:create_player_camera(1.0, 10000.0, 2.5, 0.7, 1.0, 'Caperucita')]]--
end

function add_player()
	print_logger(0, "add_player-> Añadiendo el player.")
	player = CPlayer()
	
	get_game_process():get_character_manager():set_player(player)
	get_game_process():create_player_camera(1.0, 10000.0, 2.5, 0.7, 1.0, 'Caperucita')
end

function add_enemy()
	print_logger(0, "add_enemy-> Añadiendo un enemigo.")
	enemy = CEnemy()
	enemy:init("Lobo1", Vect3f(5.0, 0.0, 0.0), 0)
	
	-- get_game_process():get_character_manager():set_player(player)
	-- print_logger(0, "player retornat")
end