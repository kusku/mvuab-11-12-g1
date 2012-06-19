class 'CPlayerIdleState' (CState)
	function CPlayerIdleState:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerIdleState:OnEnter(_CCharacter)
		core:get_debug_gui_manager().debug_render:set_state_name("Idle")
	end
	
	function CPlayerIdleState:Execute(_CCharacter)
		if not _CCharacter.locked then
			local l_enemy_detected = _CCharacter:detect_enemy()
			if _CCharacter.is_target_fixed then
				if l_enemy_detected ~= nil then
					if not l_enemy_detected:is_alive() then
						get_game_process():get_character_manager().target_enemy = nil
						_CCharacter.is_target_fixed = false
					end
				else
					get_game_process():get_character_manager().target_enemy = nil
					_CCharacter.is_target_fixed = false
				end
			end
		
			if self.action_2_input:do_action('PlayerTarget') then
				--Se asigna un target
				if not _CCharacter.is_target_fixed then
					if l_enemy_detected ~= nil then
						_CCharacter.is_target_fixed = true
						get_game_process():get_character_manager().target_enemy = l_enemy_detected
					end
				else
					_CCharacter.is_target_fixed = false
					get_game_process():get_character_manager().target_enemy = nil
				end
			end
		
			if self.action_2_input:do_action('AttackPlayer') then
				--El jugador ataca
				if _CCharacter.is_target_fixed then
					_CCharacter.logic_fsm:change_state(_CCharacter.target_attack)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
				else
					_CCharacter.logic_fsm:change_state(_CCharacter.attack)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
				end
				
			elseif 	self.action_2_input:do_action('MovePlayerUp') or 
					self.action_2_input:do_action('MovePlayerDown') or 
					self.action_2_input:do_action('MovePlayerLeft') or
					self.action_2_input:do_action('MovePlayerRight') then
				if _CCharacter.is_target_fixed then
					_CCharacter.logic_fsm:change_state(_CCharacter.target_run)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_run)
				else
					_CCharacter.logic_fsm:change_state(_CCharacter.run)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_run)
				end
			end
			
			--Actualización del yaw a partir del movimiento del mouse
			if not _CCharacter.is_target_fixed then
				l_d = self.action_2_input:do_action_mouse('YawPlayer')
				_CCharacter.yaw = _CCharacter.yaw + l_d
				_CCharacter.yaw = angle_filter(_CCharacter.yaw)
			else
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
					_CCharacter.yaw = angle_filter(_CCharacter.yaw)
				end
			end
		end
		
		--Actualiza el personaje
		_CCharacter.physic_controller:move(Vect3f(0.0, 0.0, 0.0), _CCharacter.elapsed_time)
	end
	
	function CPlayerIdleState:OnExit(_CCharacter)
	end
	
	function CPlayerIdleState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CPlayerIdleState:OnMessage")
		if ( _Msg.msg == msg_attack ) then
			local l_Enemy = get_game_process():get_character_manager():get_enemy_by_id(_Msg.sender)
			local l_received_pain = math.random( ( l_Enemy.properties.strong/ 2 ), l_Enemy.properties.strong) 
			local l_received_pain_to_hit = l_Enemy.properties.strong * 0.95		-- Busco el 95% de la fuerza total
			if ( l_received_pain >= l_received_pain_to_hit  ) then
				--_CCharacter.logic_fsm:change_state(_CCharacter.hit)
				--_CCharacter.graphic_fsm:change_state(_CCharacter.animated_hit)
				_CCharacter:hit_to_player()
			end
			print_logger(1, "PLAYER LIFE :".._CCharacter.properties.life)
			
			return true
		end
		return false
	end
	
	function CPlayerIdleState:__Finalize()
	end