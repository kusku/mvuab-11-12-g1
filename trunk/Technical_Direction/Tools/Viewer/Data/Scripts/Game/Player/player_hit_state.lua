class 'CPlayerHitState' (CState)
	function CPlayerHitState:__init(name) 
		CState.__init(self, name)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerHitState:OnEnter(_CCharacter)
		if core:is_debug_mode() then
			core:get_debug_gui_manager().debug_render:set_state_name("Hit")
		end
		
		_CCharacter:hit_to_player()
		self.animation_time = 0.0
	end
	
	function CPlayerHitState:Execute(_CCharacter, _elapsed_time)
		if self.animation_time > _CCharacter.animated_model:get_current_animation_duration("hit") - 0.1 then
			_CCharacter.logic_fsm:change_state(_CCharacter.idle)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
		else
			self.animation_time = self.animation_time + _CCharacter.elapsed_time
		end
	
		--Actualiza el personaje
		_CCharacter.physic_controller:move(Vect3f(0.0, 0.0, 0.0), _CCharacter.elapsed_time)
	end
	
	function CPlayerHitState:OnExit(_CCharacter)
	end
	
	function CPlayerHitState:OnMessage(_CCharacter, _Msg)
		return false
	end
	
	function CPlayerHitState:__Finalize()
	end