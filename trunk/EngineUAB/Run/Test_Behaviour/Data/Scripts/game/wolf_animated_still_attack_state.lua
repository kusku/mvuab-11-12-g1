class 'CWolfAnimatedStillAttackState' (CState)
	function CWolfAnimatedStillAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfAnimatedStillAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedStillAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedStillAttackState:Execute(_CCharacter)
		--print_logger(0, "CWolfAnimatedStillAttackState:Execute")
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			-- Si el player es atacable
			if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				_CCharacter:move_to( _CCharacter.player.position, _CCharacter.elapsed_time )
				_CCharacter.graphic_fsm:change_state(_CCharacter.animation_still_attack_state)
				
				--let the wife know I'm home
				-- Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              -- pMiner->ID(),        //ID of sender
                              -- ent_Elsa,            //ID of recipient
                              -- Msg_HiHoneyImHome,   //the message
                              -- NO_ADDITIONAL_INFO);    
							  
			else
				_CCharacter.graphic_fsm:change_state(_CCharacter.animation_run_state)
				--face_to_player ( _CCharacter, _CCharacter.player, _CCharacter.elapsed_time )
				--_CCharacter:move_to ( _CCharacter.player.position, _CCharacter.elapsed_time )
			end
		else
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_idle_state)
		end
	end
	
	function CWolfAnimatedStillAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedStillAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedStillAttackState:OnMessage(_CCharacter)
		print_logger(0, "CWolfAnimatedStillAttackState:OnMessage")
	end
	
	function CWolfAnimatedStillAttackState:__Finalize()
	
	end