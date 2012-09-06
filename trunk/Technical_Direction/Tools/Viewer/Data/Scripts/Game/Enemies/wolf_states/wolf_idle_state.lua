class 'CWolfIdleState' (CState)
	function CWolfIdleState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfIdleState:OnEnter(_CCharacter)
		-- print_logger(0, "CWolfIdleState:Enter")
		
		self.action_time = 0
		self.total_animation_time = 0
	end
	
	function CWolfIdleState:Execute(_CCharacter, _elapsed_time)
		-- print_logger(0, "CWolfIdleState:Execute")
		
		-- local l_distance = get_distance_to_player(_CCharacter, _CCharacter.player)
		-- print_logger (1, "Distancia al player: "..l_distance)
		
		local l_player = get_game_process():get_character_manager():get_player()
		if ( is_player_detected( _CCharacter, get_game_process():get_character_manager():get_player() ) ) then 
			_CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_run_state)

			-- -- No detecto player --> no hago nada o patrullo. TODO!!
		else
			_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
			_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
		end
		l_player = nil
	end
	
	function CWolfIdleState:OnExit(_CCharacter)
		--print_logger(0, "CWolfIdleState:Exit")
		-- _CCharacter.behaviors:pursuit_off()
	end
	
	function CWolfIdleState:OnMessage(_CCharacter, _Msg)
		-- print_logger(0, "CWolfIdleState:OnMessage")	
		if ( _Msg.msg == msg_attack ) then
			_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_hit_state)
			return true
		end
		return false
	end
	
	function CWolfIdleState:__Finalize()
	
	end
	