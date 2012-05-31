--loadfile "CaperucitaStates.lua"

class 'CCaperucitaIdleState' (CState)
	function CCaperucitaIdleState:__init() 
		CState.__init(self)
		print_logger(0, "Inicio del estado idle de la caperucita")
	end

	function CCaperucitaIdleState:Enter(_State)
	
	end
	
	function CCaperucitaIdleState:Execute(_State)
	
	end
	
	function CCaperucitaIdleState:Exit(_State)
	
	end
	
	function CCaperucitaIdleState:OnMessage(_State)
	
	end
	
	function CCaperucitaIdleState:__Finalize()
	
	end
	
	
class 'CPlayer' (CCharacter)
	 --Idle = CCaperucitaIdleState()
	
	function CPlayer:__init() 
		CCharacter.__init(self,0)
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		self.position = Vect3f(0.0, 0.0, 0.0)
		--self.animated_model.pitch = -90.0
		self.locked = false
	end
	
	function CPlayer:update(elapsed_time)
		if not self.locked then
		
			local action_2_input = core:get_action_to_input()
			
			local l_move_player = false
			local l_d = 0.0
			local l_dir = Vect3f(0.0, 0.0, 0.0)
			local l_position = Vect3f(0.0, 0.0, 0.0)
			
			local l_yaw = self.physic_controller.yaw
			l_d = action_2_input:do_action_mouse('YawPlayer')
				l_yaw = l_yaw + l_d
				if l_yaw > 2*math.pi then
					l_yaw = l_yaw - 2*math.pi
				elseif l_yaw < -2*math.pi then
					l_yaw = l_yaw + 2*math.pi
				end
			
			if action_2_input:do_action('MovePlayerUp') then
				if action_2_input:do_action('MovePlayerLeft') then
					l_dir = Vect3f(math.cos(l_yaw + math.pi/4), 0.0, math.sin( l_yaw + math.pi/4 ))
					l_position = l_position + l_dir
				elseif action_2_input:do_action('MovePlayerRight') then
					l_dir = Vect3f(math.cos(l_yaw - math.pi/4), 0.0, math.sin(l_yaw - math.pi/4))
					l_position = l_position + l_dir
				else
					l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
					l_position = l_position + l_dir
				end
				
				l_move_player = true
			elseif action_2_input:do_action('MovePlayerDown') then
				if action_2_input:do_action('MovePlayerLeft') then
					l_dir = Vect3f(math.cos(l_yaw - math.pi/4), 0.0, math.sin(l_yaw - math.pi/4))
					l_position = l_position - l_dir
				elseif action_2_input:do_action('MovePlayerRight') then
					l_dir = Vect3f(math.cos(l_yaw + math.pi/4), 0.0, math.sin(l_yaw + math.pi/4))
					l_position = l_position - l_dir
				else
					l_dir = Vect3f(math.cos(l_yaw), 0.0, math.sin(l_yaw))
					l_position = l_position - l_dir
				end
				
				l_move_player = true
			end
			
			l_position = l_position * 10.0 * elapsed_time
			
			self.physic_controller.yaw = l_yaw
			controller = self.physic_controller:move(l_position, elapsed_time)
			
			self.position = self.physic_controller.position
			self.position = Vect3f(self.position.x, self.position.y - self.physic_controller.height + 0.4, self.position.z)
			self.yaw = l_yaw
			
			self.animated_model.position = self.position
			if l_move_player then
				self.animated_model.yaw = -(l_yaw * 180.0 / math.pi) + 90.0
			end
		else
			self.physic_controller:move(Vect3f(0.0,0.0,0.0), elapsed_time)
			self.position = self.physic_controller.position
			self.animated_model.position = self.position
		end
	end