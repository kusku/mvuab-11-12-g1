class 'CPlayerTargetAttack3State' (CState)
	function CPlayerTargetAttack3State:__init(name) 
		CState.__init(self, name)
		self.action_2_input = core:get_action_to_input()
	end

	function CPlayerTargetAttack3State:OnEnter(_CCharacter)
		core:get_debug_gui_manager().debug_render:set_state_name("Target attack 3")
		self.animation_time = 0.0
	end
	
	function CPlayerTargetAttack3State:Execute(_CCharacter)
		if self.animation_time > _CCharacter.animated_model:get_current_animation_duration("attack1") - 0.1 then
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
		_soundM:play_event('Play_EFX_Sword')
		
		local l_front = get_game_process().player_camera:get_direction()
		local enemy = get_game_process():get_character_manager():search_target_enemy(3.0, math.pi/6, l_front)
		if enemy ~= nil then
			_dispatchM:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), enemy:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
		end
	end
	
	function CPlayerTargetAttack3State:OnMessage(_CCharacter, _Msg)
		if ( _Msg.msg == msg_attack ) then
			local l_Enemy = get_game_process():get_character_manager():get_enemy_by_id(_Msg.sender)
			local l_received_pain = math.random( ( l_Enemy.properties.strong/ 2 ), l_Enemy.properties.strong) 
			local l_received_pain_to_hit = l_Enemy.properties.strong * 0.95		-- Busco el 95% de la fuerza total
			if ( l_received_pain >= l_received_pain_to_hit  ) then
				--_CCharacter.logic_fsm:change_state(_CCharacter.hit)
				--_CCharacter.graphic_fsm:change_state(_CCharacter.animated_hit)
				_CCharacter:hit_to_player()
			end
			print_logger(1, "PLAYER LIFE :".._CCharacter.properties.life)
			return true
		end
		return false
	end
	
	function CPlayerTargetAttack3State:__Finalize()
	end
	