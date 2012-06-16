class 'CPlayerTargetAttack3State' (CState)
	function CPlayerTargetAttack3State:__init() 
		CState.__init(self)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerTargetAttack3State:OnEnter(_CCharacter)
		core:get_debug_gui_manager().debug_render:set_state_name("Target attack 3")
		self.animation_time = 0.0
	end
	
	function CPlayerTargetAttack3State:Execute(_CCharacter)
		if self.animation_time > _CCharacter.animated_model:get_current_animation_duration("attack1") - 0.02 then
			if core:get_action_to_input():do_action('AttackPlayer') and not _CCharacter.locked then
					_CCharacter.logic_fsm:change_state(_CCharacter.target_attack)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
			else
				if get_game_process():get_time_between_clicks() < 0.3 then
					if not _CCharacter.locked then
						_CCharacter.logic_fsm:change_state(_CCharacter.target_attack)
						_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
					end
				else
					_CCharacter.logic_fsm:change_state(_CCharacter.idle)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_idle)
				end
			end
		else
			self.animation_time = self.animation_time + _CCharacter.elapsed_time
		end
		
		--Movimiento del player hacia adelante
		local l_dir = Vect3f(0.0, 0.0, 0.0)
		if not _CCharacter.locked then
			if core:get_action_to_input():do_action('MovePlayerUp') then
				l_dir = Vect3f(math.cos(_CCharacter.yaw), 0.0, math.sin(_CCharacter.yaw))
			end
		end
		
		--Le aplica la velocidad al movimiento
		l_dir = l_dir * 2.0 * _CCharacter.elapsed_time
		
		--Mueve el controller físico
		_CCharacter.physic_controller:move(l_dir, _CCharacter.elapsed_time)
	end
	
	function CPlayerTargetAttack3State:OnExit(_CCharacter)
		local enemy = get_game_process():get_character_manager():search_target_enemy(3.0, math.pi/6)
		if enemy ~= nil then
			_DispatchMgr:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), enemy:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
		end
	end
	
	function CPlayerTargetAttack3State:OnMessage(_CCharacter, _Msg)
		if ( _Msg.msg == msg_attack ) then
			_CCharacter.logic_fsm:change_state(_CCharacter.hit)
			_CCharacter.graphic_fsm:change_state(_CCharacter.animated_hit)
			return true
		end
		return false
	end
	
	function CPlayerTargetAttack3State:__Finalize()
	end
	