class 'CWolfAnimatedAttackState' (CState)
	function CWolfAnimatedAttackState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfAnimatedAttackState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfAnimatedAttackState:Enter")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
		_CCharacter.animation_time = 0.0
	end
	
	function CWolfAnimatedAttackState:Execute(_CCharacter)
		-- print_logger(0, "CWolfAnimatedAttackState:Execute")
		if ( _CCharacter.animation_time > _CCharacter.animated_model:get_current_animation_duration("attack_still") ) then 
			print_logger(1, "ATTACK REALITZAT DEL LLOB: ".._CCharacter:get_id().." a la Caperucita")
			-- print_logger(1, "ATTACK SEND_MSG_IMMEDIATELY: "..SEND_MSG_IMMEDIATELY)
			-- print_logger(1, "_CCharacter.get_id() :".._CCharacter:get_id())
			-- print_logger(1, "_CCharacter.player.get_id() :".._CCharacter.player:get_id())
			-- print_logger(1, "msg_attack : "..msg_attack)
			_DispatchMgr:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), _CCharacter.player:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
			_CCharacter.animation_time = 0.0
			
			if ( _CCharacter:is_fatigued() ) then 
				print_logger(0, "CWolfAnimatedAttackState:Execute->Enemigo fatigado... ".._CCharacter.fatigue.." y cambio de estado..." )
				_CCharacter.graphic_fsm:change_state(_CCharacter.animation_flee_state)
			end 
		else 
			_CCharacter.animation_time =  _CCharacter.animation_time + _CCharacter.elapsed_time
		end 
		
		-- Si el player NO es atacable lo volvemos a perseguir
		if not ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
			_CCharacter.graphic_fsm:change_state(_CCharacter.animation_run_state)
		end
	end
	
	function CWolfAnimatedAttackState:OnExit(_CCharacter)
		--print_logger(0, "CWolfAnimatedAttackState:Exit")
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack_still")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfAnimatedAttackState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfAnimatedAttackState:OnMessage")
		if ( _Msg.Msg == msg_attack ) then
			print_logger(0, "Missatge acceptat per l'animació del llob")
			return true
		end
		return false
	end
	
	function CWolfAnimatedAttackState:__Finalize()
	
	end