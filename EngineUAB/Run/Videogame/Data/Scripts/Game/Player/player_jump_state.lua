class 'CPlayerJumpState' (CState)
	function CPlayerJumpState:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerJumpState:OnEnter(_CCharacter)
		core:get_debug_gui_manager().debug_render:set_state_name("Jump")
		
		self.animation_time = 0.0
		_CCharacter.physic_controller:jump(60)
		
		self.jump_yaw = _CCharacter.yaw
		if self.action_2_input:do_action('MovePlayerUp') then --El player se mueve hacia adelante
			if self.action_2_input:do_action('MovePlayerLeft') then
				self.jump_yaw = self.jump_yaw + math.pi/4
			elseif self.action_2_input:do_action('MovePlayerRight') then
				self.jump_yaw = self.jump_yaw - math.pi/4
			end
			
		elseif self.action_2_input:do_action('MovePlayerDown') then --El player se mueve hacia atrás
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
	
	function CPlayerJumpState:Execute(_CCharacter)
		if self.animation_time > _CCharacter.animated_model:get_current_animation_duration("jump") - 0.05 then
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		else
			self.animation_time = self.animation_time + _CCharacter.elapsed_time
		end
		
		--Movimiento del player hacia adelante
		local l_dir = Vect3f(math.cos(self.jump_yaw), 0.0, math.sin(self.jump_yaw))
		
		--Le aplica la velocidad al movimiento
		l_dir = l_dir * 30.0 * _CCharacter.elapsed_time
		
		--Mueve el controller físico
		_CCharacter.physic_controller:move(l_dir, _CCharacter.elapsed_time)
				
		--Gira el perosnaje
		_CCharacter.physic_controller.yaw = self.jump_yaw
		_CCharacter.animated_model.yaw = -(self.jump_yaw * 180.0 / math.pi) + 90.0
	end
	
	function CPlayerJumpState:OnExit(_CCharacter)
	end
	
	function CPlayerJumpState:OnMessage(_CCharacter, _Msg)
		if ( _Msg.msg == msg_attack ) then
			_CCharacter:rest_life( 1 )
			_CCharacter.logic_fsm:change_state(_CCharacter.hit)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_hit)
			return true
		end
		return false
	end
	
	function CPlayerJumpState:__Finalize()
	end