function set_player_life(life)
	get_game_process():get_character_manager():get_player().properties.current_life = life
end