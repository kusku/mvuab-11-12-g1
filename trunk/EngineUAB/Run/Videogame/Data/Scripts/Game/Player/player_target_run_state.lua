class 'CPlayerTargetRunState' (CState)
	function CPlayerTargetRunState:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerTargetRunState:OnEnter(_CCharacter)
		self.enemy_detected = _CCharacter:detect_enemy()
	end
	
	function CPlayerTargetRunState:Execute(_CCharacter)
		print_logger(0, "execute run target")
		local l_d = 0.0
		local l_yaw = 0.0
		local l_dir = Vect3f(0.0, 0.0, 0.0)
		local l_move_player = false
	
		--Mira si se desbloquea el target
		if self.action_2_input:do_action('PlayerTarget') then
			_CCharacter.is_target_fixed = false
		end
		
		local l_player_dir = get_game_process().player_camera:get_direction()
		local l_enemy_dir = get_game_process():get_character_manager().target_enemy.position - _CCharacter.position
		l_player_dir.y = 0.0
		l_enemy_dir.y = 0.0
		l_enemy_dir:normalize(1.0)
		l_player_dir:normalize(1.0)
		l_d = l_enemy_dir:dot(l_player_dir) --Calcula el ángulo entre donde mira el personaje y la dirección hacia el enemigo
		l_d = math.acos(l_d)
		if (math.deg(l_d) > 0.1 or math.deg(l_d) < -0.1)  then
			if( l_d < 0.0 ) then
				print_logger(0,"més petit")
			end
			_CCharacter.yaw = _CCharacter.yaw - l_d
			_CCharacter.yaw = angle_filter(_CCharacter.yaw)
			l_move_player = true
		end
	
		l_yaw = _CCharacter.yaw
		--Se mueve el personaje con el target asignado
		if self.action_2_input:do_action('MovePlayerUp') then
			l_dir = l_enemy_dir
			l_move_player = true
			
		elseif self.action_2_input:do_action('MovePlayerDown') then
			l_dir = Vect3f(-l_enemy_dir.x, -l_enemy_dir.y, -l_enemy_dir.z)
			l_move_player = true
		
		elseif self.action_2_input:do_action('MovePlayerLeft') then
			--l_dir = l_dir:cross(Vect3f(0.0, 1.0, 0.0))
			local l_vector_yaw = l_yaw + math.pi/2
			l_dir = Vect3f(math.cos(l_vector_yaw), 0.0, math.sin(l_vector_yaw))
			l_move_player = true
			
		elseif self.action_2_input:do_action('MovePlayerRight') then
			local l_vector_yaw = l_yaw -  math.pi/2
			l_dir = Vect3f(math.cos(l_vector_yaw), 0.0, math.sin(l_vector_yaw))
			l_move_player = true
		
		else
			l_dir = Vect3f(0.0, 0.0, 0.0)
		end
		
		--Le aplica la velocidad al movimiento
		l_dir = l_dir * 10.0 * _CCharacter.elapsed_time
		
		--Mueve el controller físico
		_CCharacter.physic_controller:move(l_dir, _CCharacter.elapsed_time)
		
		--Actualiza la posición del objeto 3D
		_CCharacter.position = _CCharacter.physic_controller.position
		_CCharacter.position = Vect3f(_CCharacter.position.x, _CCharacter.position.y - _CCharacter.physic_controller.height + 0.4, _CCharacter.position.z)
		
		--Actualiza la posición del modelo animado
		_CCharacter.animated_model.position = _CCharacter.position
		
		--Pone la rotación del personaje
		_CCharacter.physic_controller.yaw = l_yaw
		_CCharacter.animated_model.yaw = -(l_yaw * 180.0 / math.pi) + 90.0
		
		--Cambia de estado
		if not l_move_player then
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		else
			if not _CCharacter.is_target_fixed then
				_CCharacter.logic_fsm:change_state(_CCharacter.run)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_run)
			end
		end
	end
	
	function CPlayerTargetRunState:OnExit(_CCharacter)
	end
	
	function CPlayerTargetRunState:OnMessage(_CCharacter, _Msg)
	end
	
	function CPlayerTargetRunState:__Finalize()
	end