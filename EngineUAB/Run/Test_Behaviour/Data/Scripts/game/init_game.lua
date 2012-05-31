--loadfile "player.lua"

function init_game_data()
	print_logger(0, "entro al init_game_data")
	-- player = CPlayer()
	-- player:init("Player", Vect3f(0.0, 0.0, 0.0), 0)
	-- print_logger(0, "player creat")
	
	-- get_game_process():get_character_manager():set_player(player)
	-- print_logger(0, "player retornat")
end

function add_player()
	print_logger(0, "entro al init_game_data i creo el player")
	player = CPlayer()
	print_logger(0, "player creat")
	--player:init("Player", Vect3f(0.0, 0.0, 0.0), 0)
	print_logger(0, "player inicialitzat")
	
	get_game_process():get_character_manager():set_player(player)
	print_logger(0, "player retornat")
end

function add_enemy()
	print_logger(0, "entro al init_game_data i creo el enemic")
	enemy = CEnemy()
	enemy:init("Lobo1", Vect3f(5.0, 0.0, 0.0), 0)
	print_logger(0, "enemy creat")
	
	-- get_game_process():get_character_manager():set_player(player)
	-- print_logger(0, "player retornat")
end