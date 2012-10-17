function set_player_life(life)
	get_game_process():get_character_manager():get_player().properties.current_life = life
end

function set_wolf_life(life)
	get_game_process():get_hud():set_wolf_life(life)
end