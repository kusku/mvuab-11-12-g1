class 'CPlayer' (CCharacter)
	function CPlayer:__init()
		CCharacter.__init(self)
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		self.position = Vect3f(0.0, 0.0, 0.0)
	end
	
	function CPlayer:update(elapsed_time)
		local action_2_input = core:get_action_to_input()
		
		local l_d = 0.0
		local l_dir = Vect3f(0.0, 0.0, 0.0)
		local l_position = Vect3f(0.0, 0.0, 0.0)
		
		local l_yaw = self.physic_controller.yaw
		--if action_2_input:do_action('YawViewerCam', l_d) then
			l_yaw = l_yaw + l_d
			if l_yaw > 2*math.pi then
				l_yaw = l_yaw - 2*math.pi
			elseif l_yaw < -2*math.pi then
				l_yaw = l_yaw + 2*math.pi
			end
		--end
		
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
		end
		
		l_position = l_position * 10.0 * elapsed_time
		
		self.physic_controller.yaw = l_yaw
		controller = self.physic_controller:move(l_position, elapsed_time)
		
		self.position = self.physic_controller.position
		self.position = Vect3f(self.position.x, self.position.y - self.physic_controller.height, self.position.z)
	end