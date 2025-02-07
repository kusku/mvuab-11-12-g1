class 'CPlayerRunState' (CState)
	function CPlayerRunState:__init(name) 
		CState.__init(self, name)
		self.action_2_input = core:get_action_to_input()
		self.rotation_velocity = 10.0
	end

	function CPlayerRunState:OnEnter(_CCharacter)
		if core:is_debug_mode() then
			core:get_debug_gui_manager().debug_render:set_state_name("Run")
		end
	end
	
	function CPlayerRunState:Execute(_CCharacter, _elapsed_time)
		local l_d 				= 0.0
		local l_yaw 			= 0.0
		local l_dir 			= Vect3f(0.0, 0.0, 0.0)
		local l_move_player 	= false
		local enemy_detected 	= nil
		local elapsed_time		= _CCharacter.elapsed_time
		local l_model_yaw 		= _CCharacter.physic_controller.yaw
		
		--Miramos si hay un target a poder fijarse
		if not _CCharacter.is_target_fixed then
			enemy_detected = _CCharacter:detect_enemy()
		end
		
		if not _CCharacter.locked then
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
			
			--Actualizaci�n del yaw a partir del movimiento del mouse
			l_d = self.action_2_input:do_action_mouse('YawPlayer')
			_CCharacter.yaw 	= _CCharacter.yaw - l_d
			_CCharacter.yaw 	= angle_filter(_CCharacter.yaw)
			l_yaw 				= _CCharacter.yaw
		
			--Guarda la direcci�n anterior
			_CCharacter.previous_direction = _CCharacter.current_direction
		
			--Mira los controles de movimiento
			if self.action_2_input:do_action('MovePlayerUp') then --El player se mueve hacia adelante
				if self.action_2_input:do_action('MovePlayerLeft') then
					l_yaw = l_yaw + math.pi/4
					
					if math.abs(l_model_yaw - l_yaw) > 0.1 * self.rotation_velocity then
						if l_yaw >= math.abs(l_model_yaw) then
							l_model_yaw = l_model_yaw + math.pi/4 * self.rotation_velocity * elapsed_time
						else
							l_model_yaw = l_model_yaw - math.pi/4 * self.rotation_velocity * elapsed_time
						end
					end

				elseif self.action_2_input:do_action('MovePlayerRight') then
					l_yaw = l_yaw - math.pi/4
					
					if math.abs(l_model_yaw - l_yaw) > 0.1 * self.rotation_velocity then
						if l_yaw >= l_model_yaw then
							l_model_yaw = l_model_yaw + math.pi/4 * self.rotation_velocity * elapsed_time
						else
							l_model_yaw = l_model_yaw - math.pi/4 * self.rotation_velocity * elapsed_time
						end
					end
				else
					if math.abs(l_model_yaw - l_yaw) > 0.1 * self.rotation_velocity then
						if l_yaw >= l_model_yaw then
							l_model_yaw = l_model_yaw + math.pi/4 * self.rotation_velocity * elapsed_time
						else
							l_model_yaw = l_model_yaw - math.pi/4 * self.rotation_velocity * elapsed_time
						end
					end
				end
				
				l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
				l_move_player = true
				
			elseif self.action_2_input:do_action('MovePlayerDown') then --El player se mueve hacia atr�s
				l_yaw = l_yaw - math.pi
				--l_model_yaw = l_model_yaw - math.pi
				
				if self.action_2_input:do_action('MovePlayerLeft') then
					l_yaw = l_yaw - math.pi/4
					
					if math.abs(l_model_yaw + l_yaw) > 0.1 * self.rotation_velocity then
						if math.abs(l_yaw) >= math.abs(l_model_yaw) then
							l_model_yaw = l_model_yaw - math.pi/4 * self.rotation_velocity * elapsed_time
						else
							l_model_yaw = l_model_yaw + math.pi/4 * self.rotation_velocity * elapsed_time
						end
					end
					
				elseif self.action_2_input:do_action('MovePlayerRight') then
					l_yaw = l_yaw + math.pi/4
					
					if math.abs(l_model_yaw + l_yaw) > 0.1 * self.rotation_velocity then
						if math.abs(l_yaw) >= math.abs(l_model_yaw) then
							l_model_yaw = l_model_yaw - math.pi/4 * self.rotation_velocity * elapsed_time
						else
							l_model_yaw = l_model_yaw + math.pi/4 * self.rotation_velocity * elapsed_time
						end
					end
				else
					if math.abs(l_model_yaw + l_yaw) > 0.1 * self.rotation_velocity then
						if l_yaw >= l_model_yaw then
							l_model_yaw = l_model_yaw + math.pi/4 * self.rotation_velocity * elapsed_time
						else
							l_model_yaw = l_model_yaw - math.pi/4 * self.rotation_velocity * elapsed_time
						end
					end
				end
				
				l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
				l_move_player = true
				
			elseif self.action_2_input:do_action('MovePlayerLeft') then --El player se mueve hacia la izquierda
				l_yaw = l_yaw + math.pi/2
				
				if math.abs(l_model_yaw - l_yaw) > 0.1 * self.rotation_velocity then
					if l_yaw <= l_model_yaw then
						l_model_yaw = l_model_yaw - math.pi/4 * self.rotation_velocity * elapsed_time
					else
						l_model_yaw = l_model_yaw + math.pi/4 * self.rotation_velocity * elapsed_time
					end
				end
				
				l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
				l_move_player = true
				
			elseif self.action_2_input:do_action('MovePlayerRight') then --El player se mueve hacia la derecha
				l_yaw = l_yaw - math.pi/2
				
				if math.abs(l_model_yaw - l_yaw) > 0.1 * self.rotation_velocity then
					if l_yaw <= l_model_yaw then
						l_model_yaw = l_model_yaw - math.pi/4 * self.rotation_velocity * elapsed_time
					else
						l_model_yaw = l_model_yaw + math.pi/4 * self.rotation_velocity * elapsed_time
					end
				end
				
				l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
				l_move_player = true
			end
		end
		
		if l_model_yaw > 2*math.pi then
			l_model_yaw = l_model_yaw - 2*math.pi
		elseif l_model_yaw < -2*math.pi then
			l_model_yaw = l_model_yaw + 2*math.pi
		end
		
		if math.abs(l_model_yaw - l_yaw) < 0.1 * self.rotation_velocity then
			l_model_yaw = l_yaw
		end
		
		--Le aplica la velocidad al movimiento
		l_dir = l_dir * 10.0 * _CCharacter.elapsed_time
		
		--Mueve el controller f�sico
		_CCharacter.physic_controller:move(l_dir, _CCharacter.elapsed_time)
		
		if l_move_player then
			--Pone de forma correcta los �ngulos
			_CCharacter.physic_controller.yaw = l_model_yaw
			_CCharacter.animated_model.yaw = -(l_model_yaw * 180.0 / math.pi) + 90.0
			_CCharacter.steering_entity.velocity = l_dir
			_CCharacter.steering_entity.position = _CCharacter.physic_controller.position
			_CCharacter.steering_entity.yaw = l_model_yaw
			
			--Mira si se hace un salto
			if self.action_2_input:do_action('PlayerJump') then
				_CCharacter.logic_fsm:change_state(_CCharacter.jump)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_jump)
			end
		else
			--Cambia de estado a idle
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		end
		_CCharacter.steering_entity.yaw = _CCharacter.steering_entity:get_angle2() 
		_CCharacter.steering_entity.velocity = l_dir
		-- print_logger ( 1, "CRabbit:updateIA->Squared_length : ".._CCharacter.steering_entity.velocity:squared_length() )
		if ( _CCharacter.steering_entity.velocity:squared_length() > 0.00000001 ) then
			
			_CCharacter.steering_entity.heading = _CCharacter.steering_entity.velocity
			_CCharacter.steering_entity.heading:normalize(1.0)
			local v = _CCharacter.steering_entity.heading
			_CCharacter.steering_entity.side = v:perpendicular()
		end
	end
	
	function CPlayerRunState:OnExit(_CCharacter)
	end
	
	function CPlayerRunState:OnMessage(_CCharacter, _Msg)
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
	
	function CPlayerRunState:__Finalize()
	end