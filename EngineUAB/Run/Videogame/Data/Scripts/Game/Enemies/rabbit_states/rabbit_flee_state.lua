
class 'CRabbitFleeState' (CState)
	function CRabbitFleeState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CRabbitFleeState:OnEnter(_CCharacter)
		print_logger(0, "CRabbitFleeState:Enter")
		
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
		
		self.walk_animation_time = 0.0
		self.max_animation_time = math.random(0.5, 1.2) 
		print_logger(1, "Ara evading tant de temps "..self.max_animation_time )
	end
	
	function CRabbitFleeState:Execute(_CCharacter)
		-- print_logger(0, "CRabbitFleeState:Execute")
		l_positionA = Vect2f(_CCharacter.position.x, _CCharacter.position.z)
		l_positionB = Vect2f(_CCharacter.player.position.x, _CCharacter.player.position.z)
	
		DesiredFleePoint = (l_positionA - l_positionB)
		DesiredFleePoint:normalize(1.0);
		DesiredFleePoint = DesiredFleePoint * _CCharacter.properties.speed
		
		-- print_logger(0 , "CRabbitFleeState:Execute->posició d'evasió:"..DesiredFleePoint.x.." "..DesiredFleePoint.y.." "..DesiredFleePoint.z)
		DesiredFleePoint = Vect3f(DesiredFleePoint.x,0.0,DesiredFleePoint.y)
		_CCharacter:move_to( DesiredFleePoint, _CCharacter.elapsed_time )
		_CCharacter:face_to( _CCharacter.player.position, _CCharacter.elapsed_time )
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
			num = _CCharacter:get_animation_id("walk")
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