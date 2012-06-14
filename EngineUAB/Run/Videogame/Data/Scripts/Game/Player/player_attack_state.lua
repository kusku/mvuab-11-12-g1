class 'CPlayerAttackState' (CState)
	function CPlayerAttackState:__init() 
		CState.__init(self)
	end

	function CPlayerAttackState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack1")
			_CCharacter:get_animation_model():execute_action( num, 0.3 )
		end
	end
	
	function CPlayerAttackState:Execute(_CCharacter)
		--[[local user_data = get_game_process():get_character_manager():shoot_player_raycast()
		if get_game_process():get_character_manager():exist_enemy_user_data(user_data) then
			print_logger(0, "tocat")
		else
			print_logger(0, "no tocat")
		end]]--
	end
	
	function CPlayerAttackState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack1")
			_CCharacter:get_animation_model():clear_cycle( num, 0.1 )
		end
		
		local enemy = get_game_process():get_character_manager():search_target_enemy(8.0, math.pi/6)
		if enemy ~= nil then
			
			-- Test jordi missatges entre entitats
			print_logger(1, "Player ha atacat a un enemic nº : " ..enemy:get_id())
			_DispatchMgr:dispatch_state_message( SEND_MSG_IMMEDIATELY, _CCharacter:get_id(), enemy:get_id(), msg_attack, NO_ADDITIONAL_INFO ) 
			
			-- TODO:: AQuest codi hauria d'anar en el OnMessage del llob!!
			-- enemy.properties.life = enemy.properties.life - 10
			-- if enemy.properties.life <= 0 then
				-- enemy.enable = false
			-- end
		end
	end
	
	function CPlayerAttackState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CPlayerAttackState:OnMessage")	
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
	
	function CPlayerAttackState:__Finalize()
	end