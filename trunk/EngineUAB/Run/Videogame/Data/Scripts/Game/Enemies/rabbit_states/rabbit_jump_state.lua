class 'CRabbitJumpState' (CState)
	function CRabbitJumpState:__init(name) 
		CState.__init(self, name)
		self.type = "CRabbitJumpState"
		-- print_logger(0, "Inicio del estado JUMP del rabbit")
	end

	function CRabbitJumpState:OnEnter(_CCharacter)
		--print_logger(0, "CRabbitJumpState:Enter")
		
		_CCharacter.action_animation_time = 0.0
		_CCharacter.is_action_finished = false
		
		-- _CCharacter.behaviors:seek_off()
		-- _CCharacter.behaviors:pursuit_off()
		_CCharacter.behaviors:separation_on()
		_CCharacter.behaviors:collision_avoidance_on()
		_CCharacter.behaviors:obstacle_wall_avoidance_on()
	end
	
	function CRabbitJumpState:Execute(_CCharacter, _elapsed_time)
		print_logger(0, "CRabbitJumpState:Execute")
		if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				
		
			
		-- Si el player NO es atacable lo volvemos a preparar o a perseguir
		else 
			_CCharacter.logic_fsm:change_state(_CCharacter.prepared_to_attack_state)
		end
	end
	
	function CRabbitJumpState:OnExit(_CCharacter)
		--print_logger(0, "CRabbitJumpState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id(self.active_animation_name)
			_CCharacter:get_animation_model():clear_cycle( num, 0.7 )
		end
		_CCharacter.behaviors:seek_off()
		_CCharacter.behaviors:pursuit_off()
		_CCharacter.behaviors:separation_off()
		_CCharacter.behaviors:collision_avoidance_off()
		_CCharacter.behaviors:obstacle_wall_avoidance_off()
	end
	
	function CRabbitJumpState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CRabbitJumpState:OnMessage-> Missatge rebut: ".._Msg.msg)
		if ( _Msg.msg == msg_attack ) then
			-- Solo me pongo en hit si realmente he finalizado el estado de ataqueç
			print_logger ( 1, "ARA REBO!" )
			if ( self.action_animation_time < _CCharacter.animated_model:get_current_animation_duration(self.active_animation_name) ) then 
				_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			else
				_CCharacter:rest_life(  _CCharacter.player.properties.strong )
			end
			return true
		end
		return false
	end
	
	function CRabbitJumpState:__Finalize()
	
	end
	
	-- --------------------------------------------------------------------------------------------------------------------
	--	GetRandomJumpName : Devuelve la dirección donde saltamos de las 8 posibles (igual que las coordenadas de un reloj)
	-- --------------------------------------------------------------------------------------------------------------------
	function CRabbitJumpState:GetRandomJumpName()
		-- 8 direcciones de saltos posibles
		local JumpType = math.random(1,8) 
		if ( JumpType == 1 ) then
			return "foward"
		elseif ( JumpType == 2 ) then
			return "foward-right"
		elseif ( JumpType == 3 ) then
			return "right"
		elseif ( JumpType == 4 ) then
			return "back-right"
		elseif ( JumpType == 5 ) then
			return "back"
		elseif ( JumpType == 6 ) then
			return "back-left"
		elseif ( JumpType == 7 ) then
			return "left"
		elseif ( JumpType == 8 ) then
			return "foward-left"
		end
	
	end 