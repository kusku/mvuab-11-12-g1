class 'CPlayerTargetRunState' (CState)
	function CPlayerTargetRunState:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerTargetRunState:OnEnter(_CCharacter)
		core:get_debug_gui_manager().debug_render:set_state_name("Target run")
	
		self.enemy_detected = get_game_process():get_character_manager().preview_target_enemy
		if self.enemy_detected == nil then
			self.enemy_detected = _CCharacter:detect_enemy()
		end
		get_game_process():get_character_manager().target_enemy = self.enemy_detected
	end
	
	function CPlayerTargetRunState:Execute(_CCharacter)
		local l_d = 0.0
		local l_yaw = 0.0
		local l_dir = Vect3f(0.0, 0.0, 0.0)
		local l_move_player = false
		
		if not _CCharacter.locked then
			--Mira si se desbloquea el target
			if self.action_2_input:do_action('PlayerTarget') then
				_CCharacter.is_target_fixed = false
			end
			
			--El jugador ataca
			if self.action_2_input:do_action('AttackPlayer') then
				if _CCharacter.is_target_fixed then
					_CCharacter.logic_fsm:change_state(_CCharacter.target_attack)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
				else
					_CCharacter.logic_fsm:change_state(_CCharacter.attack)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
				end
			end
			
			--Calcula el ángulo entre donde miras y el enemigo
			local l_player_dir = get_game_process().player_camera:get_direction()
			local l_enemy_dir = get_game_process():get_character_manager().target_enemy.position - _CCharacter.position
			l_player_dir.y = 0.0
			l_enemy_dir.y = 0.0
			l_enemy_dir:normalize(1.0)
			l_player_dir:normalize(1.0)
			l_d = l_enemy_dir:dot(l_player_dir) --Calcula el ángulo entre donde mira el personaje y la dirección hacia el enemigo		
			l_d = math.acos(l_d)
			
			if (math.deg(l_d) > 0.1)  then
				_CCharacter.yaw = _CCharacter.yaw - l_d
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
				local l_vector_yaw = l_yaw + math.pi/2
				l_dir = Vect3f(math.cos(l_vector_yaw), 0.0, math.sin(l_vector_yaw))
				l_move_player = true
				
			elseif self.action_2_input:do_action('MovePlayerRight') then
				_CCharacter.yaw = l_yaw + 2*l_d
				local l_vector_yaw = _CCharacter.yaw - math.pi/2
				l_dir = Vect3f(math.cos(l_vector_yaw), 0.0, math.sin(l_vector_yaw))
				l_move_player = true
			
			else
				l_dir = Vect3f(0.0, 0.0, 0.0)
			end
		end
		
		_CCharacter.yaw = angle_filter(_CCharacter.yaw)
		--Le aplica la velocidad al movimiento
		l_dir = l_dir * 10.0 * _CCharacter.elapsed_time
		
		--Mueve el controller físico
		_CCharacter.physic_controller:move(l_dir, _CCharacter.elapsed_time)
		
		--Pone la rotación del personaje
		_CCharacter.physic_controller.yaw = l_yaw
		_CCharacter.animated_model.yaw = -(l_yaw * 180.0 / math.pi) + 90.0
		
		--Cambia de estado
		if not l_move_player then
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		else
			--Mira si se hace un salto
			if self.action_2_input:do_action('PlayerJump') then 
				_CCharacter.logic_fsm:change_state(_CCharacter.jump)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_jump)
			elseif not _CCharacter.is_target_fixed then
				_CCharacter.logic_fsm:change_state(_CCharacter.run)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_run)
				get_game_process():get_character_manager().target_enemy = nil
			end
		end
	end
	
	function CPlayerTargetRunState:OnExit(_CCharacter)
	end
	
	function CPlayerTargetRunState:OnMessage(_CCharacter, _Msg)
		if ( _Msg.msg == msg_attack ) then
			_CCharacter.logic_fsm:change_state(_CCharacter.hit)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_hit)
			return true
		end
		return false
	end
	
	function CPlayerTargetRunState:__Finalize()
	end