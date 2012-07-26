class 'CRabbitHitState' (CState)
	function CRabbitHitState:__init(name) 
		CState.__init(self, name)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CRabbitHitState:OnEnter(_CCharacter)
		--print_logger(0, "CRabbitHitState:Enter")
		if not ( _CCharacter == nil ) then
			local num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():blend_cycle( num, 0.01 )
		end
		self.hit_animation_time = 0.0
		print_logger(0, "HIT")
		_CCharacter:rest_life(_CCharacter.player.properties.strong)
	end
	
	function CRabbitHitState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitHitState:Execute")
		if ( self.hit_animation_time >= _CCharacter.animated_model:get_current_animation_duration("hit") ) then
			print_logger(0, "CRabbitHitState:OnMessage->RABBIT life : ".._CCharacter.properties.life)
			_CCharacter.logic_fsm:revert_to_previous_state()
		end
		self.hit_animation_time = self.hit_animation_time + _CCharacter.elapsed_time
	end
	
	function CRabbitHitState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitHitState:Exit")
		_CCharacter.hit_animation_time = 0.0
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():clear_cycle( num, 0.7 )
		end
	end
	
	function CRabbitHitState:OnMessage(_CCharacter, _Msg)
		--print_logger(0, "CRabbitHitState:OnMessage")
		return false
	end
	
	function CRabbitHitState:__Finalize()
	
	end