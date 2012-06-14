class 'CWolfIdleState' (CState)
	function CWolfIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CWolfIdleState:OnEnter(_CCharacter)
		-- print_logger(0, "CWolfIdleState:Enter")
		-- if not ( _CCharacter == nil ) then
		-- end
	end
	
	function CWolfIdleState:Execute(_CCharacter)
		--print_logger(0, "CWolfIdleState:Execute")
		-- local l_distance = get_distance_to_player(_CCharacter, _CCharacter.player)
		-- print_logger (1, "Distancia al player: "..l_distance)
		if ( is_player_detected( _CCharacter, _CCharacter.player ) ) then 
			-- Is Attacable --
			if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				_CCharacter.graphic_fsm:change_state(_CCharacter.attack_state)
				
			-- Pursuit --
			else
				-- player no atacable i lo perseguimos pq lo hemos detectado
				
				-- 1) Caso en que está lejísimo. Seguramente debe hacer un rodeo y usar los waypoints
				-- if ( ( l_distance > _CCharacter.properties.distance_chase ) and ( l_distance <= _CCharacter.properties.distance_path_follow ) ) then
					-- _CCharacter.logic_fsm:change_state(_CCharacter.path_follow_state)
					-- print_logger (1, "ara molt lluny")
				-- -- 2) Caso en que está cerca y lo detecto pero no demasiado
				-- elseif ( ( l_distance <= _CCharacter.properties.detection_distance ) and ( l_distance > _CCharacter.properties.distance_chase ) ) then
					_CCharacter:move_to( _CCharacter.player.position, _CCharacter.elapsed_time)
					_CCharacter.logic_fsm:change_state(_CCharacter.pursuit_state)
					-- print_logger (1, "ara not tant lluny")
				-- -- 3) Caso en que está cerquisimo y voy a por él
				-- elseif ( l_distance < _CCharacter.properties.distance_chase ) then
					-- _CCharacter.logic_fsm:change_state(_CCharacter.seek_state)
					-- print_logger (1, "ara molt aprop")
				-- end 
			end
		
		-- No detecto player --> no hago nada o patrullo. TODO!!
		else
			-- print_logger(0 , "posición :".._CCharacter.position.x.." ".._CCharacter.position.y.." ".._CCharacter.position.z)
			_CCharacter:move_to( _CCharacter.position, _CCharacter.elapsed_time)
		end
	end
	
	function CWolfIdleState:OnExit(_CCharacter)
		--print_logger(0, "CWolfIdleState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfIdleState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfIdleState:OnMessage")	
		if ( _Msg.Msg == msg_attack ) then
			print_logger(0, "Missatge acceptat per el llob")
			-- If depend tipus d'atac... treu més o menys vida... --
			_CCharacter:rest_life( 1 )
			print_logger(0, "Player life : ".._CCharacter.properties.life)
			return true
		end
		return false
	end
	
	function CWolfIdleState:__Finalize()
	
	end
	