class 'CRabbitAttackState' (CState)
	function CRabbitAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CRabbitAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CRabbitAttackState:Enter")
		if not ( _CCharacter == nil ) then
			self.active_animation_name = self:GetRandomAnimationName()
			num = _CCharacter:get_animation_id(self.active_animation_name)
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
		self.attack_animation_time = 0.0
		_CCharacter.fatigue = 2
	end
	
	function CRabbitAttackState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitAttackState:Execute")
		if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
			if ( self.attack_animation_time > _CCharacter.animated_model:get_current_animation_duration(self.active_animation_name) ) then 
				-- -- print_logger(1, "attack realitzat del llob: ".._CCharacter:get_id().." a la caperucita")
				-- -- print_logger(1, "attack send_msg_immediately: "..SEND_MSG_IMMEDIATELY)
				-- -- print_logger(1, "_CCharacter.get_id() :".._CCharacter:get_id())
				-- -- print_logger(1, "_CCharacter.player.get_id() :".._CCharacter.player:get_id())
				-- -- print_logger(1, "msg_attack : "..msg_attack)
				if not ( _dispatchM == nil ) then
					-- print_logger( 2, "CRabbitAttackState:Execute->El Dispatch" )
					_dispatchM:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), _CCharacter.player:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
					self.attack_animation_time = 0.0
				else
					print_logger( 2, "CRabbitAttackState:Execute->El Dispatch es NULL" )
				end 
				
				if ( _CCharacter:is_fatigued() ) then 
					print_logger(0, "CRabbitanimatedattackstate:execute->enemigo fatigado... ".._CCharacter.fatigue.." y cambio de estado..." )
					_CCharacter.logic_fsm:change_state(_CCharacter.flee_state)
				else
					_CCharacter.fatigue = _CCharacter.fatigue - 1
					print_logger(0, "CRabbitattackstate:execute->más fatigado... ".._CCharacter.fatigue )
				end 
			else 
				self.attack_animation_time =  self.attack_animation_time + _CCharacter.elapsed_time
			end 
		
		-- Si el player NO es atacable lo volvemos a perseguir
		else 
			_CCharacter:move_to( _CCharacter.position, _CCharacter.elapsed_time )
			_CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
		end
	end
	
	function CRabbitAttackState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id(self.active_animation_name)
			_CCharacter:get_animation_model():clear_cycle( num, 0.7 )
		end
	end
	
	function CRabbitAttackState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CRabbitAttackState:OnMessage-> Missatge rebut: ".._Msg.msg)
		if ( _Msg.msg == msg_attack ) then
			-- Solo me pongo en hit si realmente he finalizado el estado de ataqueç
			print_logger ( 1, "ARA REBO!" )
			if ( self.attack_animation_time < _CCharacter.animated_model:get_current_animation_duration(self.active_animation_name) ) then 
				_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			else
				_CCharacter:rest_life(  _CCharacter.player.properties.strong )
			end
			return true
		end
		return false
	end
	
	function CRabbitAttackState:__Finalize()
	
	end

	function CRabbitAttackState:GetRandomAnimationName()
		local AnimationType = math.random(1, 2) 
		if ( AnimationType == 1 ) then
			return "attack_1"
		else
			return "attack_2"
		end
	
	end 