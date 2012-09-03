class 'CWolfAttackState' (CState)
	function CWolfAttackState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
		self.attack_animation_time = 0.0
		_CCharacter.fatigue = 2
	end
	
	function CWolfAttackState:Execute(_CCharacter, _elapsed_time)
		-- print_logger(0, "CWolfAttackState:Execute")
		if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
			if ( self.attack_animation_time > _CCharacter.animated_model:get_current_animation_duration("attack_still") ) then 
				-- print_logger(1, "attack realitzat del llob: ".._CCharacter:get_id().." a la caperucita")
				-- print_logger(1, "attack send_msg_immediately: "..SEND_MSG_IMMEDIATELY)
				-- print_logger(1, "_CCharacter.get_id() :".._CCharacter:get_id())
				-- print_logger(1, "_CCharacter.player.get_id() :".._CCharacter.player:get_id())
				-- print_logger(1, "msg_attack : "..msg_attack)
				if not ( _dispatchM == nil ) then
					-- print_logger( 2, "CWolfAttackState:Execute->El Dispatch" )
					_dispatchM:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), _CCharacter.player:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
					self.attack_animation_time = 0.0
				else
					print_logger( 2, "CWolfAttackState:Execute->El Dispatch es NULL" )
				end 
				
				if ( _CCharacter:is_fatigued() ) then 
					print_logger(0, "cwolfanimatedattackstate:execute->enemigo fatigado... ".._CCharacter.fatigue.." y cambio de estado..." )
					_CCharacter.logic_fsm:change_state(_CCharacter.flee_state)
				else
					_CCharacter.fatigue = _CCharacter.fatigue - 1
					print_logger(0, "cwolfattackstate:execute->más fatigado... ".._CCharacter.fatigue )
				end 
			else 
				self.attack_animation_time =  self.attack_animation_time + _elapsed_time
			end 
		
		-- Si el player NO es atacable lo volvemos a perseguir
		else 
			_CCharacter:move_to( _CCharacter.position, _elapsed_time )
			_CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
		end
	end
	
	function CWolfAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():clear_cycle( num, 0.7 )
		end
	end
	
	function CWolfAttackState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAttackState:OnMessage-> Missatge rebut: ".._Msg.msg)
		if ( _Msg.msg == msg_attack ) then
			--print_logger(0, "Missatge acceptat per el llob")
			if ( self.attack_animation_time < _CCharacter.animated_model:get_current_animation_duration("attack_still") ) then 
				_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			end
			return true
		end
		return false
	end
	
	function CWolfAttackState:__Finalize()
	
	end
