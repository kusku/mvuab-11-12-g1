class 'CPlayerAttack3State' (CState)
	function CPlayerAttack3State:__init() 
		CState.__init(self)
		self.animation_callback = get_game_process():get_animation_callback_manager():get_callback("attack3")
	end

	function CPlayerAttack3State:OnEnter(_CCharacter)
		core:get_debug_gui_manager().debug_render:set_state_name("Attack 3")
		self.animation_time = _CCharacter.elapsed_time
		self.animation_callback:start_animation()
	end
	
	function CPlayerAttack3State:Execute(_CCharacter)
		if self.animation_callback:is_animation_finished() then
			if core:get_action_to_input():do_action('AttackPlayer') then
				_CCharacter.logic_fsm:change_state(_CCharacter.attack)
				_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
			else
				if get_game_process():get_time_between_clicks() < 0.2 then
					_CCharacter.logic_fsm:change_state(_CCharacter.attack)
					_CCharacter.graphic_fsm:change_state(_CCharacter.animated_attack)
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
		
		_CCharacter.physic_controller.yaw = _CCharacter.yaw
	end
	
	function CPlayerAttack3State:OnExit(_CCharacter)
		_soundM:play_event('Play_EFX_Sword')
		local enemy = get_game_process():get_character_manager():search_target_enemy(3.0, math.pi/6)
		if enemy ~= nil then
			_dispatchM:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), enemy:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
		end
	end
	
	function CPlayerAttack3State:OnMessage(_CCharacter, _Msg)
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
	
	function CPlayerAttack3State:__Finalize()
	end