class 'CRabbitAttackState' (CState)
	function CRabbitAttackState:__init(name) 
		CState.__init(self, name)
		-- print_logger(0, "Inicio del estado gestión de ataques del rabbit")
	end

	function CRabbitAttackState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitAttackState:Enter")
		
		-- Permite saber el tiempo de la animación
		self.action_animation_time = 0.0
		
		-- Permite saber si el caracter ha realizado los golpes para cansarse. Ponemos a 2 el hits_done para obligar a que cuando entra en este estado haga una minima pausa antes de atacar
		--_CCharacter.hits_done = 0
		_CCharacter.total_hits_done_to_tired = 2
		_CCharacter.behaviors:pursuit_off()
		_CCharacter.behaviors:seek_off()
		_CCharacter.behaviors:separation_on()
		_CCharacter.behaviors:collision_avoidance_on()
		_CCharacter.behaviors:obstacle_wall_avoidance_on()
	end
	
	function CRabbitAttackState:Execute(_CCharacter, _elapsed_time)
		print_logger(0, "CRabbitAttackState:Execute")
		if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
		
			if  ( self:is_fatigued(_CCharacter) ) then 
				-- print_logger (1, "fatigat després de fer n cops seguits...")
				_CCharacter.logic_fsm:change_state(_CCharacter.tired_state)
			elseif ( _CCharacter.received_hits_x_minut == total_received_hits_x_minut ) then 
				-- print_logger (1, "hits x minut rebuts i per tant bloquejaré...")
				_CCharacter.logic_fsm:change_state(_CCharacter.defense_state)
			else 
				_CCharacter.active_action_state = self:GetRandomAttackName()
				print_logger ( 0, "Attack Random Sel·leccionat ".._CCharacter.active_action_state ) 
				
				if  ( _CCharacter.active_action_state == "attack_1" ) then
					_CCharacter.logic_fsm:change_state(_CCharacter.still_attack_state)
				
				elseif ( _CCharacter.active_action_state == "attack_2" ) then
					_CCharacter.logic_fsm:change_state(_CCharacter.run_attack_state)
				
				elseif ( _CCharacter.active_action_state == "defense" ) then
					_CCharacter.logic_fsm:change_state(_CCharacter.defense_state)
					
				-- elseif ( _CCharacter.active_action_state == "jump" ) then
					-- _CCharacter.logic_fsm:change_state(_CCharacter.jump_state)
				
				elseif ( _CCharacter.active_action_state == "go_in_to_frustum" ) then
					local l_angle = 22		-- 22,5 graus de fustrum
					go_in_to_frustum(_CCharacter, l_angle, _elapsed_time)
				
				end 
			
			end 	-- end fatigue
			
		-- Si el player NO es atacable lo volvemos a preparar o a perseguir
		else 
			_CCharacter.logic_fsm:change_state(_CCharacter.prepared_to_attack_state)
		end
	end
	
	function CRabbitAttackState:OnExit(_CCharacter)
		-- print_logger(0, "CRabbitAttackState:Exit")
		-- if not ( _CCharacter == nil ) then
			-- num = _CCharacter:get_animation_id(self.active_animation_name)
			-- _CCharacter:get_animation_model():clear_cycle( num, 0.7 )
		-- end
		_CCharacter.behaviors:separation_off()
		_CCharacter.behaviors:collision_avoidance_off()
		_CCharacter.behaviors:obstacle_wall_avoidance_off()
	end
	
	function CRabbitAttackState:OnMessage(_CCharacter, _Msg)
		-- print_logger(0, "CRabbitAttackState:OnMessage-> Missatge rebut: ".._Msg.msg)
		if ( _Msg.msg == msg_attack ) then
			-- Solo me pongo en hit si realmente he finalizado el estado de ataqueç
			-- print_logger ( 1, "ARA REBO!" )
			if ( self.action_animation_time < _CCharacter.animated_model:get_current_animation_duration(self.active_animation_name) ) then 
				_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			else
				_CCharacter:rest_life(  _CCharacter.player.properties.strong )
			end
			return true
		end
		return false
	end
	
	function CRabbitAttackState:__Finalize()
	
	end

	-- --------------------------------------------------------------------------------------------------------
	--	GetRandomAttackName : Devuelve si atacamos de lejos o de cerca y su apropiada animación. También
	-- 						puede devolver si solo se mueve sin atacar para engañar un poco al player
	-- --------------------------------------------------------------------------------------------------------
	function CRabbitAttackState:GetRandomAttackName()
		local AttackType = math.random(1, 12) 
		if ( AttackType == 1 ) then
			return "attack_1"
		elseif ( AttackType == 2 ) then
			return "attack_1"
		elseif ( AttackType == 3 ) then
			return "attack_1"
		elseif ( AttackType == 4 ) then
			return "attack_2"
		elseif ( AttackType == 5 ) then
			return "attack_2"
		elseif ( AttackType == 6 ) then
			return "attack_2"
		elseif ( AttackType == 7 ) then
			return "defense"
		elseif ( AttackType == 8 ) then
			return "jump"
			
		-- Más probabilidades de ir al fustrum que no atacar
		elseif ( AttackType == 9 ) then
			return "go_in_to_frustum"
		elseif ( AttackType == 10 ) then
			return "go_in_to_frustum"
		elseif ( AttackType == 11 ) then
			return "go_in_to_frustum"
		elseif ( AttackType == 12 ) then
			return "go_in_to_frustum"
		end
	end 
	
	function CRabbitAttackState:is_fatigued( _CCharacter )
		-- print_logger ( 1, "hits done : ".._CCharacter.hits_done.." and hits let : ".._CCharacter.total_hits_done_to_tired)
		if ( _CCharacter.hits_done == _CCharacter.total_hits_done_to_tired ) then
			_CCharacter.hits_done = 0
			return true
		else 
			return false
		end 
	end 