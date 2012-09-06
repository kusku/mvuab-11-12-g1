class 'CWolfPursuitState' (CState)
	function CWolfPursuitState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfPursuitState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfPursuitState:Enter")
		
		local l_player = get_game_process():get_character_manager():get_player()
		_CCharacter.behaviors.pursuit.target = l_player.position
		_CCharacter.behaviors.pursuit:update_evader_entity( l_player.steering_entity )
		_CCharacter.behaviors:pursuit_on()
	end
	
	function CWolfPursuitState:Execute(_CCharacter, _elapsed_time)
		--print_logger(0, "CWolfPursuitState:Execute")
		_CCharacter.behaviors:pursuit_off()
		
		local l_player = get_game_process():get_character_manager():get_player()
		if ( is_player_detected( _CCharacter, l_player ) ) then 
			-- if ( is_enemy_prepared_to_attack ( _CCharacter, l_player ) ) then
			if ( is_player_attackable ( _CCharacter, l_player ) ) then
				_CCharacter.behaviors:pursuit_off()
				_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
				_CCharacter:face_to( _CCharacter.steering_entity.position, _elapsed_time )
				_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
				_CCharacter.logic_fsm:change_state(_CCharacter.still_attack_state)
			else
				_CCharacter.behaviors.pursuit.target = l_player.position
				_CCharacter.behaviors.pursuit:update_evader_entity( l_player.steering_entity )
				_CCharacter.behaviors:pursuit_on()
				
				-- Rotamos al objetivo y movemos
				_CCharacter:face_to( _CCharacter.steering_entity.position, _elapsed_time )
				_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
			end
		else
			_CCharacter.logic_fsm:change_state(_CCharacter.idle_state)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_idle_state)
		end 
	end
	
	function CWolfPursuitState:OnExit(_CCharacter)
		--print_logger(0, "CWolfPursuitState:Exit")
		_CCharacter.behaviors:pursuit_off()
		_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
	end
	
	function CWolfPursuitState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfPursuitState:OnMessage")
		if ( _Msg.msg == msg_attack ) then
			_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_hit_state)
			return true
		end
		return false
	end
	
	function CWolfPursuitState:__Finalize()
	
	end