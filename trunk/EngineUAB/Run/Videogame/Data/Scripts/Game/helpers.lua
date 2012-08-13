
-- -------------------------------------------------------------------------------------
--  get_distance_to_player: Retorna la distancia entre el player y enemigo
-- -------------------------------------------------------------------------------------
function get_distance_to_player( _enemy, _player )
	l_positionA = Vect2f(_enemy.position.x, _enemy.position.z)
	l_positionB = Vect2f(_player.position.x, _player.position.z)
	
	l_distance = l_positionA:distance(l_positionB)
	return l_distance
end

-- -------------------------------------------------------------------------------------
--  is_player_inside_impact_distance: Mira si estoy en ataque corto al player 
-- -------------------------------------------------------------------------------------
function is_player_inside_impact_distance( _enemy, _player )
	l_positionA = Vect2f(_enemy.position.x, _enemy.position.z)
	l_positionB = Vect2f(_player.position.x, _player.position.z)
	
	-- print_logger(0 , "posición distance :".._enemy.position.x.." ".._enemy.position.y.." ".._enemy.position.z)
	-- print_logger(0 , "posición player :".._player.position.x.." ".._player.position.y.." ".._player.position.z)
	
	-- si la distancia entre el player i el enemigo es menor de un valor empieza el ataque
	l_distance = l_positionA:distance(l_positionB)
	if ( l_distance <= _enemy.properties.impact_distance ) then 
		-- print_logger (0, "Player es atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return true
	else
		-- print_logger (0, "Player NO atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return false
	end
end

-- -------------------------------------------------------------------------------------
--  is_player_inside_attack_distance: Mira si estoy cerca el player en disposició de ataque
-- -------------------------------------------------------------------------------------
function is_player_inside_attack_distance( _enemy, _player )
	l_positionA = Vect2f(_enemy.position.x, _enemy.position.z)
	l_positionB = Vect2f(_player.position.x, _player.position.z)
	
	-- print_logger(0 , "posición distance :".._enemy.position.x.." ".._enemy.position.y.." ".._enemy.position.z)
	-- print_logger(0 , "posición player :".._player.position.x.." ".._player.position.y.." ".._player.position.z)
	
	-- si la distancia entre el player i el enemigo es menor de un valor empieza el ataque
	l_distance = l_positionA:distance(l_positionB)
	if ( l_distance <= _enemy.properties.attack_distance ) then 
		-- print_logger (0, "Player es atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return true
	else
		-- print_logger (0, "Player NO atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return false
	end
end

-- -------------------------------------------------------------------------------------
--  is_player_inside_aproximation_distance: Mira si estoy cerca del player en 
--  disposición para poder atacar pero no encima
-- -------------------------------------------------------------------------------------
function is_player_inside_aproximation_distance( _enemy, _player )
	l_positionA = Vect2f(_enemy.position.x, _enemy.position.z)
	l_positionB = Vect2f(_player.position.x, _player.position.z)
	
	-- print_logger(0 , "posición distance :".._enemy.position.x.." ".._enemy.position.y.." ".._enemy.position.z)
	-- print_logger(0 , "posición player :".._player.position.x.." ".._player.position.y.." ".._player.position.z)
	
	-- si la distancia entre el player i el enemigo es menor de un valor empieza el ataque
	l_distance = l_positionA:distance(l_positionB)
	if ( l_distance <= _enemy.properties.aproximation_distance ) and ( l_distance > _enemy.properties.attack_distance ) then 
		-- print_logger (0, "Player es atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return true
	else
		-- print_logger (0, "Player NO atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return false
	end
end

-- -------------------------------------------------------------------------------------
--  is_player_inside_prepared_distance: Mira si estoy proximo para preparar un ataque
-- -------------------------------------------------------------------------------------
function is_player_inside_prepared_distance( _enemy, _player )
	l_positionA = Vect2f(_enemy.position.x, _enemy.position.z)
	l_positionB = Vect2f(_player.position.x, _player.position.z)
	
	-- print_logger(0 , "posición distance :".._enemy.position.x.." ".._enemy.position.y.." ".._enemy.position.z)
	-- print_logger(0 , "posición player :".._player.position.x.." ".._player.position.y.." ".._player.position.z)
	
	
	-- si la distancia entre el player i el enemigo es menor de un valor empieza el ataque
	l_distance = l_positionA:distance(l_positionB)
	if ( l_distance <= _enemy.properties.prepared_attack_distance ) then --and ( l_distance > _enemy.properties.aproximation_distance )  then 
		-- print_logger (0, "Player es atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
		return true
	else
		-- print_logger (0, "Player NO atacable y esta a una distancia de : " ..l_distance.."metros? i té una distancia d'attack de : ".._enemy.properties.attack_distance)
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
	l_distance = l_positionA:distance(l_positionB)
	if ( l_distance < _enemy.properties.detection_distance ) then 
		--print_logger (0, "Player detected!! go to player at: "..l_distance.." meters? metres de detecció enemic: ".._enemy.properties.detection_distance )
		return true
	else
		--print_logger (0, "Player massa lluny!! : "..l_distance.." meters? metres de detecció enemic: ".._enemy.properties.detection_distance )
		return false
	end
end

-- -------------------------------------------------------------------------------------
--  is_enemy_focused: Mira si el enemigo està cerca y focalizado o bien seleccionado
--  es decir fijado cara a cara con el player
-- -------------------------------------------------------------------------------------
function is_enemy_focused( _enemy )
	if ( _enemy == nil ) then
		-- print_logger( 2, "is_enemy_focused-> Error: enemy is nill" )
		return false
	end 

	-- Buscamos el enemigo más cercano en 360º ya que ataca el más cercano y no el que tenga de cara el player
	local l_character_manager = get_game_process():get_character_manager()
	local l_enemy_detected = l_character_manager:search_target_enemy( _enemy.properties.detection_distance, 2 * math.pi)
		
	-- Si no tenemos enemigo como target o como preview target (enemigo más cercano señalado) lo buscamos
	-- if ( get_game_process():get_character_manager().target_enemy == nil ) then 
		-- if ( get_game_process():get_character_manager().preview_target_enemy == nil ) then
				-- l_enemy_detected = _enemy.player:detect_enemy()
		-- else
			-- l_enemy_detected = get_game_process():get_character_manager().preview_target_enemy 
	-- else 
		-- l_enemy_detected = get_game_process():get_character_manager().target_enemy 
	-- end
	
	if ( l_enemy_detected == nil ) then 
		print_logger ( 1, "Enemy not detected" )
		return false
	else 
		if ( l_enemy_detected.name == _enemy.name ) then
			return true
		else 
			return false
		end
	end
end
	
-- -------------------------------------------------------------------------------------
--  is_player_ready : Esta función debería devolver si el player no està en Hit ( en el suelo ), o si 
--	está en cualquier estado disponible a ser atacado
-- -------------------------------------------------------------------------------------
function is_player_ready( _player )
	if ( _player.logic_fsm == nil ) then
		return false
	end
	
	local l_name = _player.logic_fsm.current_state.type
	-- print_logger(0, "is_ready-> miro si estic en hit"..l_name)
	-- if _player.logic_fsm:is_in_state(_player.hit) then
	if ( l_name == "CPlayerAnimationHitState" ) then
		-- print_logger(1, "is_ready-> isInState")
		return false
	else
		-- print_logger(1, "is_ready-> not isInState")
		return true
	end
end
	
-- -------------------------------------------------------------------------------------
--  is_player_attackable: Mira si estoy muy proximo al player en disposició de ataque,
--  si no tiene obstaculos, si está focalitzado y el player listo
-- -------------------------------------------------------------------------------------
function is_player_attackable( _enemy, _player )

	if ( _enemy == nil ) or ( _player == nil ) then 
		return false 
	end 
	
	local l_is_focused 					= is_enemy_focused(_enemy)
	local l_inside_attack_distance		= is_player_inside_attack_distance( _enemy, _player )
	local l_is_player_ready				= is_player_ready(_player)
	
	-- if ( l_is_focused ) then 
		-- print_logger ( 0, "is_player_attackable-> l_is_focused enemy ".._enemy.name.." true" )
	-- else 
		-- print_logger ( 0, "is_player_attackable-> l_is_focused ".._enemy.name.." false" )
	-- end 	
	
	-- if ( l_inside_attack_distance ) then 
		-- print_logger ( 0, "is_player_attackable-> l_inside_attack_distance ".._enemy.name.." true" )
	-- else 
		-- print_logger ( 0, "is_player_attackable-> l_inside_attack_distance ".._enemy.name.." false" )
	-- end 	
	
	-- if ( l_is_player_ready ) then 
		-- print_logger ( 0, "is_player_attackable-> l_is_player_ready ".._enemy.name.." true" )
	-- else 
		-- print_logger ( 0, "is_player_attackable-> l_is_player_ready ".._enemy.name.." false" )
	-- end 	
	
	-- Devolverá true solo si está detectado/focalizado, dentro de la distancia de ataque y el player listo para ser atacado
	if  ( ( l_is_focused ) and ( l_inside_attack_distance ) and ( l_is_player_ready ) ) then 
	-- if  ( ( l_inside_attack_distance ) and ( l_is_player_ready ) ) then 
		-- print_logger ( 1, "is_player_attackable-> REtorno ara cap a la main: true" )
		return true
	else
		-- print_logger ( 1, "is_player_attackable-> REtorno ara cap a la main : false" )
		return false
	end
	
end

-- -------------------------------------------------------------------------------------
--  is_enemy_prepared_to_attack: Mira si está en la distancia de preparació d'atac
-- -------------------------------------------------------------------------------------
function is_enemy_prepared_to_attack( _enemy, _player )
	local l_inside_prepared_distance	= is_player_inside_prepared_distance( _enemy, _player )
	
	-- Devolverá true solo si está dentro de la distancia de preparación para el ataque 
	if ( l_inside_prepared_distance )  then
		return true
	else
		return false
	end
end

-- -------------------------------------------------------------------------------------
--  is_enemy_aproximated_to_attack: Mira si está en la distancia d'aproximació d'atac
-- -------------------------------------------------------------------------------------
function is_enemy_aproximated_to_attack( _enemy, _player )
	-- local l_is_focused 						= is_enemy_focused(_enemy)
	local l_inside_aproximation_distance	= is_player_inside_aproximation_distance( _enemy, _player )
	--local l_has_free_way_to_player 		= has_obstacles_founded_on_the_way( _enemy )
	
	-- Devolverá true solo si está detectado/focalizado, dentro de la distancia de aproximación y el player listo para ser atacado
	if ( l_inside_aproximation_distance ) then -- ( l_has_free_way_to_player ) 
		-- print_logger ( 1, "is_enemy_aproximated_to_attack-> REtorno ara cap a la main: true" )
		return true
	else
		-- print_logger ( 1, "is_enemy_aproximated_to_attack-> REtorno ara cap a la main : false" )
		return false
	end
end

-- -------------------------------------------------------------------------------------
--  is_weak_than_player: Retorna si l'enemic és més débil que el player
-- -------------------------------------------------------------------------------------
function is_weaker_than_player( _enemy, _player )
	local l_is_weaker = ( _enemy.properties.life <= player.properties.life )
	return l_is_weaker
end

-- -------------------------------------------------------------------------------------
--  has_obstacles_founded_on_the_way: Retorna si el enemigo tiene obstaculos directo 
--  en el camino al player
-- -------------------------------------------------------------------------------------
function has_obstacles_founded_on_the_way( _enemy )
	return false
end 

-- -------------------------------------------------------------------------------------
--  angle_filter: Transforma un ángulo en un valor entre 0 y 2*pi
-- -------------------------------------------------------------------------------------
function angle_filter(_angle)
	l_angle = _angle	
	local l_2_pi = 2 * math.pi
	if l_angle > l_2_pi then
		l_angle = l_angle - l_2_pi
	elseif l_angle < -l_2_pi then
		l_angle = l_angle + l_2_pi
	end
	
	return l_angle
end

function get_random_time(_min, _max)
	return random
end 

function go_in_to_fustrum(_CCharacter, _angle)
	local l_angle = math.rad(_angle)
	if ( get_game_process():get_character_manager():is_enemy_visible_in_angle(_CCharacter, l_angle) ) then
		-- print_logger(1," Helper:go_in_to_fustrum->")
		-- local l = math.deg(_angle)
		-- print_logger(1," GRAUS "..l)
		-- print_logger(1,"visible al fustrum")
		-- _CCharacter.behaviors:seek_off()
		-- _CCharacter.steering_entity.velocity = Vect3f(0,0,0)

		-- _CCharacter.behaviors.pursuit.target = _CCharacter.player.position
		-- _CCharacter.behaviors.pursuit:update_evader_entity( _CCharacter.player.steering_entity )
		-- _CCharacter.behaviors:pursuit_on()
		_CCharacter.behaviors.seek.target = _CCharacter.player.position
		_CCharacter.behaviors:seek_on()
		
		-- Rotamos al objetivo y movemos
		_CCharacter:face_to( _CCharacter.player.position, _CCharacter.elapsed_time )
		_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _CCharacter.elapsed_time )

		return
	-- En caso contrario buscamos un punto dentro del fustrum de la càmara. Si en el camino tenemos opción de ataque entonces atacará
	else
		-- -- print_logger(1,"NO visible al fustrum")
		_CCharacter.behaviors:pursuit_off()
		local l_point_to_go = get_point_of_front(_CCharacter)
		_CCharacter.behaviors.seek.target = l_point_to_go
		_CCharacter.behaviors:seek_on()

		_CCharacter:face_to( _CCharacter.player.position, _CCharacter.elapsed_time )
		_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _CCharacter.elapsed_time )
	end 
end

-- -------------------------------------------------------------------------------------------------------------
-- 	get_point_of_front: Permite pillar un punto enfrente de la dirección de la camara. Útil para ver que los enemigos, antes de atacar, intentan que sean visualizados por el player.
-- -------------------------------------------------------------------------------------------------------------
function get_point_of_front(_CCharacter)
	local l_front 	= get_game_process().player_camera:get_direction()
	local l_radi 	= _CCharacter.properties.attack_distance + 2
	local l_pos  	= _CCharacter.player.steering_entity.position 
	-- local l_front 	= _CCharacter.steering_entity:get_front()
	local l_height 	= _CCharacter.player.steering_entity.height
	l_front:normalize(1.0)
	l_pos = Vect3f( l_pos.x + l_front.x * l_radi , l_pos.y + l_height, l_pos.z + l_front.z * l_radi );
	-- print_logger(1, " position.x : "..l_pos.x.." position.y : "..l_pos.y.." position.z : "..l_pos.z)
	
	return l_pos
end 
	
	