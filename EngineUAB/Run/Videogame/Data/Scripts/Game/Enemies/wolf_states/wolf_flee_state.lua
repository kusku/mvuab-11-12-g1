
class 'CWolfFleeState' (CState)
	function CWolfFleeState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado run de la caperucita")
	end

	function CWolfFleeState:OnEnter(_CCharacter)
		print_logger(0, "CWolfFleeState:Enter")
		
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():blend_cycle( num, 0.3 )
		end
		
		self.walk_animation_time = 0.0
		self.max_animation_time = math.random(4.0, 8.0) 
		print_logger(1, "Ara evading tant de temps "..self.max_animation_time )
	end
	
	function CWolfFleeState:Execute(_CCharacter)
		-- print_logger(0, "CWolfFleeState:Execute")
		l_positionA = Vect2f(_CCharacter.position.x, _CCharacter.position.z)
		l_positionB = Vect2f(_CCharacter.player.position.x, _CCharacter.player.position.z)
	
		DesiredFleePoint = (l_positionA - l_positionB)
		DesiredFleePoint:normalize(1.0);
		DesiredFleePoint = DesiredFleePoint * _CCharacter.properties.speed
		
		-- print_logger(0 , "CWolfFleeState:Execute->posició d'evasió:"..DesiredFleePoint.x.." "..DesiredFleePoint.y.." "..DesiredFleePoint.z)
		DesiredFleePoint = Vect3f(DesiredFleePoint.x,0.0,DesiredFleePoint.y)
		_CCharacter:move_to( DesiredFleePoint, _CCharacter.elapsed_time )
		_CCharacter:face_to( _CCharacter.player.position, _CCharacter.elapsed_time )
		if ( self.walk_animation_time >= self.max_animation_time ) then
			print_logger(0 , "CWolfFleeState:Execute->Revertir estado al anterior:")
			_CCharacter.logic_fsm:revert_to_previous_state()
		end
		
		self.walk_animation_time = self.walk_animation_time + _CCharacter.elapsed_time
	end
	
	function CWolfFleeState:OnExit(_CCharacter)
		print_logger(0, "CWolfFleeState:Exit")
		_CCharacter.animation_time = 0.0
		_CCharacter.max_animation_time = 0.0
		
		if not ( _CCharacter == nil ) then
			num = _CCharacter:get_animation_id("walk")
			_CCharacter:get_animation_model():clear_cycle( num, 0.3 )
		end
	end
	
	function CWolfFleeState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfFleeState:OnMessage")
		if ( _Msg.Msg == msg_attack ) then
			print_logger(0, "Missatge acceptat per el llob")
			-- If depend tipus d'atac... treu més o menys vida... --
			_CCharacter:rest_life( 1 )
			print_logger(0, "WOLF life : ".._CCharacter.properties.life)
			return true
		end
		return false
	end
	
	function CWolfFleeState:__Finalize()
	
	end