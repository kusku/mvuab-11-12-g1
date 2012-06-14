-- ["msg_idle"]	
-- msg_ready"]		
-- msg_sleep"]		
-- msg_attack"]	
-- ["msg_run_away"]
-- msg_patrol"]	
-- msg_pursuit"]	
-- msg_roam"]		
-- msg_evade"]		
-- msg_chase"]		


-- -------------------------------------------------------------------------------------
--  is_player_attackable: Mira si estoy muy proximo al player en disposició de ataque
-- -------------------------------------------------------------------------------------
function is_player_attackable( _enemy, _player )
	l_positionA = Vect2f(_enemy.position.x, _enemy.position.z)
	l_positionB = Vect2f(_player.position.x, _player.position.z)
	
	-- print_logger(0 , "posición distance :".._enemy.position.x.." ".._enemy.position.y.." ".._enemy.position.z)
	-- print_logger(0 , "posición player :".._player.position.x.." ".._player.position.y.." ".._player.position.z)
	
	-- si la distancia entre el player i el enemigo es menor de un valor empieza el ataque
	l_distance = l_positionA:sq_distance(l_positionB)
	if ( l_distance <= _enemy.properties.attack_distance ) then 
		--print_logger (0, "Player es atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return true
	else
		--print_logger (0, "Player NO atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return false
	end
end

-- -------------------------------------------------------------------------------------
--  is_player_detected: Mira si detecta el player segun la distancia de detección
-- -------------------------------------------------------------------------------------
function is_player_detected( _enemy, _player )
	l_positionA = Vect2f(_enemy.position.x, _enemy.position.z)
	l_positionB = Vect2f(_player.position.x, _player.position.z)
	
	-- print_logger(0 , "posición enemigo :".._enemy.position.x.." ".._enemy.position.y.." ".._enemy.position.z)
	-- print_logger(0 , "posición player :".._player.position.x.." ".._player.position.y.." ".._player.position.z)
	
	-- miro si el player entra en distancia de detección
	l_distance = l_positionA:sq_distance(l_positionB)
	if ( l_distance < _enemy.properties.detection_distance ) then 
		--print_logger (0, "Player detected!! go to player at: "..l_distance.." meters? metres de detecció enemic: ".._enemy.properties.detection_distance )
		return true
	else
		--print_logger (0, "Player massa lluny!! : "..l_distance.." meters? metres de detecció enemic: ".._enemy.properties.detection_distance )
		return false
	end
end

-- -------------------------------------------------------------------------------------
--  get_distance_to_player: Retorna la distancia entre el player y enemigo
-- -------------------------------------------------------------------------------------
function get_distance_to_player( _enemy, _player )
	l_positionA = Vect2f(_enemy.position.x, _enemy.position.z)
	l_positionB = Vect2f(_player.position.x, _player.position.z)
	
	l_distance = l_positionA:sq_distance(l_positionB)
	
	return l_distance
end
