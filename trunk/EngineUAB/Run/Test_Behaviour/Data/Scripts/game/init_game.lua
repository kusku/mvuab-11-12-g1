loadfile "Enemy.lua"

class 'CPlayer' (CCharacter)
	function CPlayer:__init()
		CCharacter.__init(self, 6)
	end
	
	function CPlayer:update(elapsed_time)
		print_logger(0, "update")
	end
	
function init_game_data()
	print_logger(0, "entro al init_game_data i creo el player")
	player = CPlayer()
	print_logger(0, "player creat")
	l_enemy = CEnemy()
	print_logger(0, "enemy creat")
	-- get_game_process():get_character_manager():set_player(player)
	-- print_logger(0, "player retornat")
	-- print_logger(0, "entro al init_game_data i creo l'enemic")
	-- l_Enemy = CEnemy()
	-- print_logger(0, "Enemic creat")
	proces = get_game_process()
	print_logger (0, "agafo el process" )
	-- get_game_process():get_character_manager():add_enemy(l_Enemy)
	-- print_logger(0, "Enemic enviat")
end

-- function load_enemy()

-- -- -- Create a bar
-- -- print_logger ( 0, "Antes de crear a Barry")
-- -- bar = Bar("Barry");
-- -- bar:Think();
-- -- print_logger ( 0, "Despues de crear a Barry")

-- -- -- Create a baz
-- -- baz = Baz("Bazzy");
-- -- baz:Think();

	-- l_Enemy = CEnemy()
	-- get_game_process():get_character_manager():add_enemy(l_Enemy);
-- end