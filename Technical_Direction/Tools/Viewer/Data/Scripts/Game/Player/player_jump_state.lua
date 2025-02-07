class 'CPlayerJumpState' (CState)
	function CPlayerJumpState:__init(name) 
		CState.__init(self, name)
		self.action_2_input = core:get_action_to_input()
		self.previous_position = Vect3f(0.0, 0.0, 0.0)
	end

	function CPlayerJumpState:OnEnter(_CCharacter)
		if core:is_debug_mode() then
			core:get_debug_gui_manager().debug_render:set_state_name("Jump")
		end
		
		self.animation_time = 0.0
		_CCharacter.physic_controller:jump(50)
		
		self.jump_yaw = _CCharacter.yaw
		if self.action_2_input:do_action('MovePlayerUp') then --El player se mueve hacia adelante
			if self.action_2_input:do_action('MovePlayerLeft') then
				self.jump_yaw = self.jump_yaw + math.pi/4
			elseif self.action_2_input:do_action('MovePlayerRight') then
				self.jump_yaw = self.jump_yaw - math.pi/4
			end
			
		elseif self.action_2_input:do_action('MovePlayerDown') then --El player se mueve hacia atr�s
			self.jump_yaw = self.jump_yaw - math.pi
			if self.action_2_input:do_action('MovePlayerLeft') then
				self.jump_yaw = self.jump_yaw - math.pi/4
			elseif self.action_2_input:do_action('MovePlayerRight') then
				self.jump_yaw = self.jump_yaw + math.pi/4
			end
			
		elseif self.action_2_input:do_action('MovePlayerLeft') then --El player se mueve hacia la izquierda
			self.jump_yaw = self.jump_yaw + math.pi/2
			
		elseif self.action_2_input:do_action('MovePlayerRight') then --El player se mueve hacia la derecha
			self.jump_yaw = self.jump_yaw - math.pi/2
		end
	end
	
	function CPlayerJumpState:Execute(_CCharacter, _elapsed_time)
		if self.animation_time > _CCharacter.animated_model:get_current_animation_duration("jump") - 0.05 then
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		else
			self.animation_time = self.animation_time + _CCharacter.elapsed_time
		end
		
		--Movimiento del player hacia adelante
		local l_dir = Vect3f(math.cos(self.jump_yaw), 0.0, math.sin(self.jump_yaw))
		
		--Le aplica la velocidad al movimiento
		l_dir = l_dir * 10.0 * _CCharacter.elapsed_time
		
		--Mueve el controller f�sico
		_CCharacter.physic_controller:move(l_dir, _CCharacter.elapsed_time)
		if self.previous_position.y == _CCharacter.physic_controller.position.y then	
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		end
				
		--Gira el perosnaje
		_CCharacter.physic_controller.yaw = self.jump_yaw
		_CCharacter.animated_model.yaw = -(self.jump_yaw * 180.0 / math.pi) + 90.0
	end
	
	function CPlayerJumpState:OnExit(_CCharacter)
		_CCharacter.physic_controller:move(Vect3f(0.0, 0.0, 0.0), _CCharacter.elapsed_time)
	end
	
	function CPlayerJumpState:OnMessage(_CCharacter, _Msg)
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
	
	function CPlayerJumpState:__Finalize()
	end