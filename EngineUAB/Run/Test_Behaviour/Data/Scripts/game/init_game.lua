--loadfile "player.lua"

function init_game_data()
	print_logger(0, "entro al init_game_data")
	-- player = CPlayer()
	-- player:init("Player", Vect3f(0.0, 0.0, 0.0), 0)
	-- print_logger(0, "player creat")
	
	-- get_game_process():get_character_manager():set_player(player)
	-- print_logger(0, "player retornat")
end

-- function add_player()
	-- print_logger(0, "entro al init_game_data i creo el player")
	-- player = CPlayer()
	-- print_logger(0, "player creat")

	-- get_game_process():get_character_manager():set_player(player)
	-- print_logger(0, "player retornat")
-- end

-- -- No funciona!! obligatori passar el id i el nom pq el add_enemy ho requerix	
-- function add_enemy( _id )
	-- -- print_logger(0, "entro al init_game_data i creo el enemic")
	-- -- enemy = CEnemy( _id ) 
	-- -- print_logger(0, "enemy creat")
	
	-- --get_game_process():get_character_manager():add_enemy(enemy)
	-- --print_logger(0, "enemy retornat")
-- end

-- local my_enemies={}

-- function add_enemy( _id, _name )
	-- print_logger(0, "entro al init_game_data i creo el enemic")
	-- local enemy = CEnemy( _id, _name ) 
	-- print_logger(0, "enemy creat")
	-- table.insert(my_enemies, enemy)
	
	-- get_game_process():get_character_manager():add_enemy(enemy)
	-- print_logger(0, "enemy retornat")
-- end