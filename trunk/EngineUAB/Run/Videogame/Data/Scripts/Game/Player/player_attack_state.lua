class 'CPlayerAttackState' (CState)
	function CPlayerAttackState:__init() 
		CState.__init(self)
	end

	function CPlayerAttackState:OnEnter(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack")
			_CCharacter:get_animation_model():execute_action( num, 0.3 )
		end
	end
	
	function CPlayerAttackState:Execute(_CCharacter)
		local user_data = get_game_process():get_character_manager():shoot_player_raycast()
		if get_game_process():get_character_manager():exist_enemy_user_data(user_data) then
			print_logger(0, "tocat")
		else
			print_logger(0, "no tocat")
		end
	end
	
	function CPlayerAttackState:OnExit(_CCharacter)
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("attack")
			_CCharacter:get_animation_model():clear_cycle( num, 0.1 )
		end
		
		local enemy = get_game_process():get_character_manager():is_player_near_enemy(2.0)
		if enemy ~= nil then
			enemy.properties.life = enemy.properties.life - 10
			if enemy.properties.life <= 0 then
				enemy.enable = false
			end
		end
	end
	
	function CPlayerAttackState:OnMessage(_CCharacter)
	end
	
	function CPlayerAttackState:__Finalize()
	end