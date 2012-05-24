class 'CPlayer' (CCharacter)
	function CPlayer:__init()
		CCharacter.__init(self,0)
	end
	
	function CPlayer:update(elapsed_time)
		print_logger(0, "update")
	end
	
function init_game_data()
	player = CPlayer()
	get_game_process():get_character_manager():set_player(player)
end