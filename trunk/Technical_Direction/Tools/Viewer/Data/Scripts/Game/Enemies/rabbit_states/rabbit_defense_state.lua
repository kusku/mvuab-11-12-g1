class 'CRabbitDefenseState' (CState)
	function CRabbitDefenseState:__init(name) 
		CState.__init(self,name)
		self.type = "CRabbitDefenseState"
		-- print_logger(0, "Inicio del estado DEFENSE del rabbit")
	end

	function CRabbitDefenseState:OnEnter(_CCharacter)
		-- print_logger(0, "CRabbitDefenseState:Enter")
		
		-- Tiempo  en este estado
		self.action_time = 0.0
		
		-- Tiempo máximo en este estado
		self.max_action_time = 6
				
		-- Nombre de la animación
		self.active_animation_name = _CCharacter:get_animation_id("deffense")
		
		-- Me dice si bloqueo
		self.hit_blocked = false										
		
		-- Me dice la distancia que recorro cuando paga y bloqueo hacia atras
		self.hit_distance = _CCharacter.properties.impact_distance + 2
		
		-- Me dice el total de bloqueos que haré hasta que me pueda volver a golpear
		self.total_hit_blocked = math.random(1, 4)  		
		-- print_logger (1, "nº hits totals x blojar"..self.total_hit_blocked )
		
		-- me dice el nº de veces que el player me pega mientras bloqueo
		self.hit_blocked_count = 0			
		
		-- Metemos más velocidad al ataque i menos massa para acelerar más 
		self.old_max_speed = _CCharacter.steering_entity.max_speed
		self.old_mass = _CCharacter.steering_entity.mass
		_CCharacter.steering_entity.max_speed = 1
		_CCharacter.steering_entity.mass = 0.00500
	end
	
	function CRabbitDefenseState:Execute(_CCharacter, _elapsed_time)
		-- print_logger(0, "CRabbitDefenseState:Execute")
		_CCharacter:face_to( _CCharacter.player.position, _elapsed_time )
	
		-- Si bloqueo un golpeo debo retroceder
		if ( self.hit_blocked ) then 
			
			-- Calculo la distancia que no sea mayor que el máximo que pueda tirar hacia atras al bloquear
			local l_distance = get_distance_to_player(_CCharacter, _CCharacter.player)
			
			-- Si aun no he hecho el retroceso lo sigo moviendo
			if ( l_distance <= self.hit_distance ) then 
				print_logger(1, "ara fa ")
				_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
			
			-- Si llego al destino paro el retroceso
			else
				print_logger(1, "ja no")
				-- _CCharacter.behaviors:flee_off()
				_CCharacter.behaviors:seek_off()
				_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
				_CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )
				self.hit_blocked = false
			end 
		-- Si no bloqueado el golpe aun...
		else 
			-- Si és atacable miro si llegué al màximo de lo que permito que me golpeen y bloqueo
			if ( is_player_attackable( _CCharacter, _CCharacter.player ) ) then
				-- Si asoleixo el total de hits bloquejats retorno al anterior estat
				if ( self.hit_blocked_count == self.total_hit_blocked ) then 
					print_logger (1, "retorno")
					_CCharacter.received_hits_x_minut = 0
					_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0 )
					--_CCharacter.logic_fsm:revert_to_previous_state()
					_CCharacter.logic_fsm:change_state(_CCharacter.attack_state)
					return
				end 
			
				-- Solo hago la acció si estoy dentro de la distancia de impacto
				local l_distance = get_distance_to_player(_CCharacter, _CCharacter.player)
		
				if ( l_distance <= ( _CCharacter.properties.impact_distance * 2 ) ) then
					_CCharacter:get_animation_model():execute_action( self.active_animation_name, 0 )
				else 
					_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
					_CCharacter:get_animation_model():clear_cycle( self.active_animation_name, 0 )
					
				end 

				-- print_logger (1, "temps total en defense ".._CCharacter.action_time)
				if ( self.action_time > self.max_action_time ) then
					-- nos volvemos
					-- print_logger(0, "CRabbitDefenseState:Execute->Nos Volvemos")
					_CCharacter.logic_fsm:change_state(_CCharacter.attack_state)
					-- self.logic_fsm:revert_to_previous_state()		-- No sembla xutar bé... sembla q sigui el causant que peti
				else 
					-- Incrementamos el tiempo que llevamos en este estado
					-- print_logger(0, "CRabbitDefenseState:Execute->Incremento tiempo")
					self.action_time = self.action_time + _elapsed_time
				end 
				
				
			-- Si el player NO es atacable lo volvemos a preparar o a perseguir
			else 
				_CCharacter.received_hits_x_minut = 0
				--_CCharacter.logic_fsm:revert_to_previous_state()
				_CCharacter.logic_fsm:change_state(_CCharacter.attack_state)
			end
		end
	end
	
	function CRabbitDefenseState:OnExit(_CCharacter)
		-- print_logger(0, "CRabbitDefenseState:Exit")
		_CCharacter.steering_entity.velocity = Vect3f(0,0,0)
		_CCharacter.behaviors:seek_off()
		-- Restauramos la velocidad original
		_CCharacter.steering_entity.max_speed = self.old_max_speed
		_CCharacter.steering_entity.mass = self.old_mass
	end
	
	function CRabbitDefenseState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CRabbitDefenseState:OnMessage-> Missatge rebut: ".._Msg.msg)
		if ( _Msg.msg == msg_attack ) then
			-- Solo me pongo en hit si realmente he finalizado el estado de ataque
			
			-- Me dice que acabo de bloquear un golpe
			self.hit_blocked = true

			-- _CCharacter.behaviors:flee_on()
			_CCharacter.behaviors:seek_on()
			local l_front = _CCharacter.steering_entity:get_front()
			l_front:normalize(1.0)
			l_front = l_front:rotate_y(math.pi)
			-- _CCharacter.behaviors.flee.target = _CCharacter.player.position
			-- l_target = Vect3f(l_front.x,l_front.y,l_front.z):normalize(1)
			-- l_target = l_target * 2
			-- _CCharacter.behaviors.flee.target = l_target
			
			l_front = _CCharacter.steering_entity.position + l_front * self.hit_distance
			-- _CCharacter.behaviors.flee.target = l_front
			_CCharacter.behaviors.seek.target = l_front
			-- _CCharacter.steering_entity.velocity = Vect3f(0,0,0)
					
			-- _CCharacter:move_to2( _CCharacter.steering_entity.velocity, _elapsed_time )

			-- Cuento el nº de hits que lleva mientras bloqueo
			self.hit_blocked_count = self.hit_blocked_count + 1
			print_logger (1, "self.hit_blocked_count : "..self.hit_blocked_count)
			return true
		end
		return false
	end
	
	function CRabbitDefenseState:__Finalize()
	
	end
