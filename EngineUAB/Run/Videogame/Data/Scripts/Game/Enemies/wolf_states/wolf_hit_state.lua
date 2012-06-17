class 'CWolfHitState' (CState)
	function CWolfHitState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado still attack de la caperucita")
	end

	function CWolfHitState:OnEnter(_CCharacter)
		print_logger(0, "CWolfHitState:Enter")
		if not ( _CCharacter == nil ) then
			local num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():blend_cycle( num, 0.01 )
		end
		self.hit_animation_time = 0.0
	end
	
	function CWolfHitState:Execute(_CCharacter)
		-- print_logger(0, "CWolfHitState:Execute")
		if ( self.hit_animation_time >= _CCharacter.animated_model:get_current_animation_duration("hit") ) then
			print_logger(0, "CWolfHitState:Execute->Fi del hit")
			_CCharacter.logic_fsm:revert_to_previous_state()
		end
		self.hit_animation_time = self.hit_animation_time + _CCharacter.elapsed_time
	end
	
	function CWolfHitState:OnExit(_CCharacter)
		print_logger(0, "CWolfHitState:Exit")
		_CCharacter.hit_animation_time = 0.0
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("hit")
			_CCharacter:get_animation_model():clear_cycle( num, 0.7 )
		end
	end
	
	function CWolfHitState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfHitState:OnMessage")
		if ( _Msg.msg == msg_attack ) then
			print_logger(0, "Missatge acceptat per el llob")
			-- If depend tipus d'atac... treu més o menys vida... --
			-- - -- Ja l'he fet en l'attack _CCharacter:rest_life( 1 )
			print_logger(0, "WOLF life : ".._CCharacter.properties.life)

			return true
		end
		return false
	end
	
	function CWolfHitState:__Finalize()
	
	end