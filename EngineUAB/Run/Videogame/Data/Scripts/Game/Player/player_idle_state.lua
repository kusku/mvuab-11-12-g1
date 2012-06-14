class 'CPlayerIdleState' (CState)
	function CPlayerIdleState:__init() 
		CState.__init(self)
	end

	function CPlayerIdleState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
	end
	
	function CPlayerIdleState:Execute(_CCharacter)
	end
	
	function CPlayerIdleState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CPlayerIdleState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CPlayerIdleState:OnMessage")	
		if ( _Msg.Msg == msg_attack ) then
			print_logger(0, "Missatge acceptat per la caperucita... aquí faria el que vull, en principi restà vida...")
			-- If depend tipus d'atac... treu més o menys vida... --
			_CCharacter:rest_life( 1 )
			--_CCharacter.graphic_fsm:change_state(_CCharacter.hit_state)
			print_logger(0, "Player life : ".._CCharacter.properties.life)
			
			return true
		end
		return false
	end
	
	function CPlayerIdleState:__Finalize()
	end