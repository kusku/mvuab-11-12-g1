
class 'CRabbitTiredState' (CState)
	function CRabbitTiredState:__init(name) 
		CState.__init(self, name)
		print_logger(0, "Inicio del estado TIRED del Rabbit")
	end

	function CRabbitTiredState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitTiredState:Enter")
		
		if not ( _CCharacter == nil ) then
			self.active_animation = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( self.active_animation, 0.3 )
		end
		
		self.tired_animation_time = 0.0
		self.max_animation_time = math.random(0.2, 0.5) 
		print_logger(1, "Ara esperem tant de temps "..self.max_animation_time )
	end
	
	-- ---------------------------------------------------------------------------------------------------------
	--	Caso especial de Flee. Realmente no se aleja de pánico sinó de cansancio pero me es útil igualmente
	-- ---------------------------------------------------------------------------------------------------------
	function CRabbitTiredState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitTiredState:Execute")
		
		-- Si llego a descansar ya no esta fatigado y nos volvemos
		if ( self.tired_animation_time >= self.max_animation_time ) then
			-- print_logger(0 , "CRabbitTiredState:Execute->Revertir estado al anterior:")
			_CCharacter.fatigue = false
			_CCharacter.logic_fsm:revert_to_previous_state()
		end
		
		self.tired_animation_time = self.tired_animation_time + _CCharacter.elapsed_time
		
		-- Mentre espero miro al player
		_CCharacter:face_to( _CCharacter.player.position, _CCharacter.elapsed_time )
	end
	
	function CRabbitTiredState:OnExit(_CCharacter)
		-- print_logger(0, "CRabbitTiredState:Exit")
		_CCharacter.animation_time = 0.0
		_CCharacter.max_animation_time = 0.0
		
		if not ( _CCharacter == nil ) then
			self.active_animation = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( self.active_animation, 0.3 )
		end
	end
	
	function CRabbitTiredState:OnMessage(_CCharacter, _Msg)
		-- print_logger(0, "CRabbitTiredState:OnMessage")
		if ( _Msg.msg == msg_attack ) then
			_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			return true
		end
		return false
	end
	
	function CRabbitTiredState:__Finalize()
	
	end