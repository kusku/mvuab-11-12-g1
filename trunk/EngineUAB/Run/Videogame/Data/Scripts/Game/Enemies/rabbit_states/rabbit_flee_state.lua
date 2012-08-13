
class 'CRabbitFleeState' (CState)
	function CRabbitFleeState:__init(name) 
		CState.__init(self, name)
		print_logger(0, "Inicio del estado flee de la caperucita")
	end

	function CRabbitFleeState:OnEnter(_CCharacter)
		print_logger(0, "CRabbitFleeState:Enter")
		
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
		
		self.walk_animation_time = 0.0
		self.max_animation_time = math.random(2.5, 4.2) 
		print_logger(1, "Ara evading tant de temps "..self.max_animation_time )
	end
	
	-- ---------------------------------------------------------------------------------------------------------
	--	Caso especial de Flee. Realmente no se aleja de pánico sinó de cansancio pero me es útil igualmente
	-- ---------------------------------------------------------------------------------------------------------
	function CRabbitFleeState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitFleeState:Execute")
		l_positionA = Vect2f(_CCharacter.position.x, _CCharacter.position.z)
		l_positionB = Vect2f(_CCharacter.player.position.x, _CCharacter.player.position.z)
	
		local l_DesiredFleePoint = 0.0
		l_DesiredFleePoint = (l_positionA - l_positionB)
		-- Compruebo que no me aleje más de la distancia de espera por cansado
		local l_distance = l_positionA:sq_distance(l_positionB)
		if ( l_distance <= _CCharacter.properties.tired_distance ) then 
			-- print_logger(0 , "CRabbitFleeState:Execute->posició d'evasió:"..l_DesiredFleePoint.x.." "..l_DesiredFleePoint.y.." "..l_DesiredFleePoint.z)
		
			-- local num = _CCharacter:get_animation_id("run")
			-- _CCharacter:get_animation_model():clear_cycle( num, 0.3 )
			
			l_DesiredFleePoint:normalize(1.0);
			l_DesiredFleePoint = l_DesiredFleePoint * _CCharacter.properties.speed
			
			-- print_logger(0 , "CRabbitFleeState:Execute->posició d'evasió:"..l_DesiredFleePoint.x.." "..l_DesiredFleePoint.y.." "..l_DesiredFleePoint.z)
			l_DesiredFleePoint = Vect3f(l_DesiredFleePoint.x,0.0,l_DesiredFleePoint.y)
		else
			local num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
			
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		
			-- print_logger ( 0, "L'latra popcipel distancia "..l_distance)
			-- print_logger ( 1, "L'latra popcipel distancia del llob : "..(_CCharacter.properties.tired_distance))
		
			l_DesiredFleePoint = _CCharacter.position
		end 
		
		_CCharacter:face_to( _CCharacter.player.position, _CCharacter.elapsed_time )
		_CCharacter:move_to( l_DesiredFleePoint, _CCharacter.elapsed_time )
		if ( self.walk_animation_time >= self.max_animation_time ) then
			print_logger(0 , "CRabbitFleeState:Execute->Revertir estado al anterior:")
			_CCharacter.logic_fsm:revert_to_previous_state()
		end
		
		self.walk_animation_time = self.walk_animation_time + _CCharacter.elapsed_time
	end
	
	function CRabbitFleeState:OnExit(_CCharacter)
		print_logger(0, "CRabbitFleeState:Exit")
		_CCharacter.animation_time = 0.0
		_CCharacter.max_animation_time = 0.0
		
		if not ( _CCharacter == nil ) then
			local num = _CCharacter:get_animation_id("run")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
			num = _CCharacter:get_animation_id("idle")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
			
		end
	end
	
	function CRabbitFleeState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CRabbitFleeState:OnMessage")
		if ( _Msg.msg == msg_attack ) then
			_CCharacter.logic_fsm:change_state(_CCharacter.hit_state)
			return true
		end
		return false
	end
	
	function CRabbitFleeState:__Finalize()
	
	end