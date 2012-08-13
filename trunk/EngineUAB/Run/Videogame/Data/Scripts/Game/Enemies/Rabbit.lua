-- ------------------------------------------------------------------------
--	Clase CRabbit
-- ------------------------------------------------------------------------
class 'CRabbit' (CCharacter)
	
	-- ------------------------------
	--		Load States Methods
	-- ------------------------------
	function CRabbit:load_graphic_states()
		-- Idle State --
		self.animation_idle_state = CRabbitAnimatedIdleState("rabbit_animation_idle")
		if self.animation_idle_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado idle del conejito")
		end		
		
		-- Walk State --
		self.animation_walk_state = CRabbitAnimatedWalkState("rabbit_animation_walk")
		if self.animation_walk_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado Walk del conejito")
		end
		
		-- Pursuit State --
		self.animation_pursuit_state = CRabbitAnimatedPursuitState("rabbit_animation_pursuit")
		if self.animation_pursuit_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado PURSUIT del conejito")		
		end
		
		-- Attack State --
		self.animation_attack_state = CRabbitAnimatedAttackState("rabbit_animation_attack")
		if self.animation_attack_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado ATTACK del conejito")
		end
		
		-- -- Attack Run State --
		-- self.animation_run_attack_state = CRabbitAnimatedRunAttackState()
		-- if self.animation_run_attack_state == nil then
			-- print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado Run Attack del conejito")
		-- else
			-- print_logger(0, "CRabbit::load_graphic_states->Creat estado animado run del conejito")
		-- end
		
		-- -- Hit State --
		-- self.animation_hit_state = CRabbitAnimatedHitState()
		-- if self.animation_hit_state == nil then
			-- print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado Hit del conejito")
		-- else
			-- print_logger(0, "CRabbit::load_graphic_states->Creat estado animado run del conejito")
		-- end
		
		-- -- Deffense State --
		-- self.animation_deffense_state = CRabbitAnimatedDeffenseState()
		-- if self.animation_hit_state == nil then
			-- print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado deffense del conejito")
		-- else
			-- print_logger(0, "CRabbit::load_graphic_states->Creat estado animado run del conejito")
		-- end
		
		print_logger(0, "CRabbit::load_graphic_states->Animated states loaded")
	end 
	
	function CRabbit:load_logic_states()
		-- Idle State --
		self.idle_state = CRabbitIdleState("rabbit_idle")
		if self.idle_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado IDLE del conejito")
		end		
		
		-- -- Seek State -- Ir directamente a un destino
		-- self.seek_state = CRabbitSeekState()
		-- if self.seek_state == nil then
			-- print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado SEEK del conejito")
		-- else 
			-- print_logger(0, "CRabbit::load_logic_states->Creat estat SEEK del conejito")
		-- end	
		
		-- Flee State -- Huye directamente de un destino
		self.flee_state = CRabbitFleeState("rabbit_flee")
		if self.flee_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado FLEE del conejito")
		end	
		
		-- Pursuit State -- Persigue inteligentemente
		self.pursuit_state = CRabbitPursuitState("rabbit_pursuit")
		if self.pursuit_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado PURSUIT del conejito")
		end	
		
		-- -- Evade State -- Huye inteligentemente
		-- self.evade_state = CRabbitEvadeState()
		-- if self.evade_state == nil then
			-- print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado EVADE del conejito")
		-- else 
			-- print_logger(0, "CRabbit::load_logic_states->Creat estat EVADE del conejito")
		-- end	
		
		-- -- Wander State -- Actua como un errante
		-- self.wander_state = CRabbitWanderState()
		-- if self.wander_state == nil then
			-- print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado WANDER del conejito")
		-- else 
			-- print_logger(0, "CRabbit::load_logic_states->Creat estat WANDER del conejito")
		-- end	
		
		-- -- Path Follow State --	Sigue un camino
		-- self.path_follow_state = CRabbitPathFollowState()
		-- if self.path_follow_state == nil then
			-- print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado PATH FOLLOW del conejito")
		-- else 
			-- print_logger(0, "CRabbit::load_logic_states->Creat estat PATH FOLLOW del conejito")
		-- end	
		
		-- Attack State --	Entra en estado de ataque al player
		self.attack_state = CRabbitAttackState("rabbit_attack")
		if self.attack_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado ATTACK del conejito")
		end	
		
		-- -- -- Walk State --
		-- -- self.animation_walk_state = CRabbitWalkState()
		-- -- if self.walk_state == nil then
			-- -- print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado Walk del conejito")
		-- -- else
			-- -- print_logger(0, "CRabbit::load_logic_states->Creat estat Walk del conejito")
		-- -- end
		
		-- -- -- Run State --
		-- -- self.animation_run_state = CRabbitRunState()
		-- -- if self.animation_run_state == nil then
			-- -- print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado Run del conejito")
		-- -- else
			-- -- print_logger(0, "CRabbit::load_logic_states->Creat estat run del conejito")
		-- -- end
		
		-- -- -- Attack Run State --
		-- -- self.animation_run_attack_state = CRabbitRunAttackState()
		-- -- if self.animation_run_attack_state == nil then
			-- -- print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado Run Attack del conejito")
		-- -- else
			-- -- print_logger(0, "CRabbit::load_logic_states->Creat estat run del conejito")
		-- -- end
		
		-- Hit State --
		self.hit_state = CRabbitHitState("rabbit_hit")
		if self.hit_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado HIT del conejito")
		end
		
		-- -- Deffense State --
		-- self.deffense_state = CRabbitDeffenseState()
		-- if self.deffense_state == nil then
			-- print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado DEFFENSE del conejito")
		-- else
			-- print_logger(0, "CRabbit::load_logic_states->Creat estat DEFFENSE del conejito")
		-- end
		
		print_logger(0, "CRabbit::load_logic_states->Logic states loaded")
	end	
	
	-- ------------------------------
	--			Constructors
	-- ------------------------------	
	function CRabbit:__init( _id )
		CCharacter.__init( self, _id )
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		self.locked = true
		
		self.character_manager = get_game_process():get_character_manager()
		self.player = get_game_process():get_character_manager():get_player()
		
		if ( self.player == nil ) then
			print_logger ( 2, "CRabbit::__init()->player is nil" )
		end
		
		-- Cargamos estados
		self:load_logic_states()
		--self:load_graphic_states()
	end
	
	function CRabbit:__init( _id, _name )
		CCharacter.__init( self, _id, _name )
		--self.yaw = 0.0
		self.pitch = -math.pi / 8
		--self.roll = 0.0
		--self.position = Vect3f(3.0, 0.0, 0.0)
		--self.position2 = self.position
		self.locked = true
		
		self.character_manager = get_game_process():get_character_manager()
		self.player = get_game_process():get_character_manager():get_player()
		
		if ( self.player == nil ) then
			print_logger ( 2, "CRabbit::__init()->player is nil" )
		end
		
		-- Cargamos los estados		
		self:load_logic_states()
		--self:load_graphic_states()
	end
	
	function CRabbit:init()
		print_logger(1, "CRabbit:init->")
		local l_IsOk = false
		
		l_gfsm = self.graphic_fsm 
		if (l_gfsm == nil) then
			print_logger(2, "CRabbit:init()->No se ha podido obtener la máquina de estados gráfica.")
			l_IsOk = false
		else
			l_gfsm.current_state = self.animation_idle_state 
			l_IsOk = true
		end
		
		l_lfsm = self.logic_fsm 
		if (l_lfsm == nil) then
			print_logger(2, "CRabbit:init()->No se ha podido obtener la máquina de estados lógica.")
			l_IsOk = false
		else
			self:get_animation_model():clear_cycle( self:get_animation_id("run"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("attack_1"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("attack_2"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("idle"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("walk"), 0.3 )
			self:get_animation_model():clear_cycle( self:get_animation_id("hit"), 0.3 )
			l_lfsm.current_state = self.idle_state
			num = self:get_animation_id("idle")
			self:get_animation_model():blend_cycle( num, 0.3 )
			
			l_IsOk = l_IsOk and true
		end
		
		self.behaviors:add_behaviour( CSeek() )
		-- -- self.behaviors:add_behaviour( CFlee(self.properties.panic_distance) )
		-- -- self.behaviors:add_behaviour( CPursuit() )
		-- -- self.behaviors:add_behaviour( CEvade(self.properties.panic_distance) )
		-- -- self.behaviors:add_behaviour( CArrive() )
		-- -- self.behaviors:add_behaviour( CWander() )
		-- -- self.behaviors:add_behaviour( CCollisionAvoidance(5) )
		
		-- -- Flocking --
		-- -- self.behaviors:add_behaviour( CSeparation() )
		-- -- self.behaviors:add_behaviour( CAlignment() )
		-- -- self.behaviors:add_behaviour( CCohesion() )
		
		self.behaviors:seek_off()
		-- -- self.behaviors:flee_off()
		-- -- self.behaviors:arrive_off()
		-- -- self.behaviors:pursuit_off()
		-- -- self.behaviors:evade_off()
		-- -- self.behaviors:wander_off()
		-- -- self.behaviors:separation_off()
		-- -- self.behaviors:alignment_off()
		-- -- self.behaviors:cohesion_off()
		-- -- self.behaviors:collision_avoidance_off()
		-- -- self.behaviors:obstacle_wall_avoidance_off()
		
		-- --self.steering_entity.velocity = self.steering_entity.velocity + self.behaviors:update( 1, self.steering_entity )
		-- -- print_logger (0	, "CRabbit::Update()->ENEMY VELOCITY :"..self.steering_entity.velocity.x.." "..self.player.position.y.." "..self.steering_entity.velocity.y)
		-- --self.physic_controller.position = self.steering_entity.velocity * 1
		
		-- -- print_logger (0	, "CRabbit::Update()->PLAYER POSITION :"..self.player.position.x.." "..self.player.position.y.." "..self.player.position.y)
		
		self.position 				 	= self.physic_controller.position
		self.steering_entity.position 	= self.physic_controller.position
		self.behaviors.seek.target 		= self.player.position
		-- -- self.behaviors.flee.target 	= self.player.position
		-- -- self.behaviors.pursuit.target  = self.player.position
		-- -- self.behaviors.evade.target	= self.player.position
		-- -- self.behaviors.evade:update_pursuer_entity(self.player.steering_entity, 5)
		-- -- self.behaviors.arrive.target 	= self.player.position
		-- -- self.behaviors.wander.target 	=  Vect3f(-30,-30,0)
		-- -- self.behaviors.wander.target 	= self.player.position
		
		-- --self.behaviors.cohesion.update_parameters()
		
		-- self:move_to2( self.position, 0 )
		
		-- Nº de hits que el conejo ha realizado
		self.hits_done = 0
		
		-- nº de hits que se le permite recibir al enemigo antes de empezar a bloquear. cuando llega al valor se pasa a estado de defense
		-- cada ciertos golpeos pasaré a un estado de defensa. estas variables me permitiràn controlarlo. 
		self.received_hits_x_minut = 0				-- conteo de golpeos 
		self.total_received_hits_x_minut = 3		-- Total permitidos antes de pasar a estado defense
		
		return l_IsOk
	end
	
	function CRabbit:updateIA(_elapsed_time)
		-- Calculamos la fuerza hacia el objetivo
		local l_steering_force = self.behaviors:update( _elapsed_time, self.steering_entity ) 
		l_steering_force = Vect3f( l_steering_force.x, 0, l_steering_force.z )
		
		-- aceleración = fuerza/masa
		local l_acceleration  = l_steering_force / self.steering_entity.mass
		
		-- actualizamos la velocidad. Ya hemos comprobado en C++ su trucamiento con la max. velocidad
		self.steering_entity.velocity =  self.steering_entity.velocity + l_acceleration * _elapsed_time 
		self.steering_entity.velocity = Vect3f (self.steering_entity.velocity.x, 0, self.steering_entity.velocity.z) 
		
		-- nos aseguramos que el rabbit no excede de la velocidad máxima permitida
		local l_Vel = Vect3f( self.steering_entity.velocity.x, self.steering_entity.velocity.y, self.steering_entity.velocity.z )
		l_Vel = l_Vel:truncate(self.steering_entity.max_speed)
		self.steering_entity.velocity = l_Vel
		
		-- actualizamos la posición
		self.steering_entity.position = self.steering_entity.position + self.steering_entity.velocity * _elapsed_time 
		
		-- Actualizamos el Heahing y Side de la entidad solo si esta tiene velocidad
		-- print_logger ( 1, "CRabbit:updateIA->Squared_length : "..self.steering_entity.velocity:squared_length() )
		if ( self.steering_entity.velocity:squared_length() > 0.00000001 ) then
			self.steering_entity.heading = self.steering_entity.velocity
			-- Ahora actualizamos el heading (Vector unitario velocidad) y su perpendicular
			self.steering_entity.heading:normalize(1.0)
			local v = self.steering_entity.heading
			self.steering_entity.side = v:perpendicular()
		else
			self.steering_entity.heading = self.steering_entity:get_front()+-- Ahora actualizamos el heading (Vector unitario velocidad) y su perpendicular
			self.steering_entity.heading:normalize(1.0)
			local v = self.steering_entity.heading
			self.steering_entity.side = v:perpendicular()
		end
		
		
		-- Actualiza el heading del caracter para suabizarlo si está activado
		self.steering_entity:smoothing_on()
		if (self.steering_entity:is_smoothing_on()) then
			self.steering_entity.smoothing_heading = self.steering_entity.heading_smoother:update(self.steering_entity.heading);
		end
	end 
	
	-- ------------------------------
	--			Main Methods
	-- ------------------------------
	function CRabbit:update(_elapsed_time)
		-- print_logger(0, "CRabbit::update()->Actualizando enemigo...")
		-- Almacenamos el elapsed time pq después será impossible de guardar
		self.elapsed_time = _elapsed_time
		
		-- Asignamos como objetivo el player a los comportamientos
		-- self.behaviors.seek.target 				=  self.player.position
		-- self.behaviors.pursuit.target  			= self.player.position
		-- self.behaviors.pursuit:update_evader_entity( self.player.steering_entity )
		-- self.behaviors.collision_avoidance.target 	=  self.player.position
		-- self.behaviors.flee.target 				=  self.player.position
		-- self.behaviors.wander.target 				= self.behaviors.wander.target
		-- self.behaviors.arrive.target 				= self.player.position
		
		-- self:updateIA(_elapsed_time)
		
		-- l_gfsm = self.graphic_fsm 
		-- if l_gfsm == nil then
			-- print_logger(2, "CRabbit:update()->No se ha podido obtener la máquina de estados gráfica.")
		-- else
			-- if (self.enable == true ) then
				-- l_gfsm:update()
				-- --print_logger(0, "CRabbit::update()->Actualizando gfsm...")			
			-- else
				-- --print_logger(0, "CRabbit::update()->Enable = false")
				-- l_gfsm:change_state(self.animation_idle_state)
			-- end 
		-- end
		
		local l_lfsm = self.logic_fsm 
		if l_lfsm ~= nil then
			if (self.locked == false ) then
				l_lfsm:update()
			else
				l_lfsm:change_state(self.idle_state)
				self:move_to( self.position, _elapsed_time )
			end 
		end
	end
	
	-- ------------------------------
	--			Destructor
	-- ------------------------------
	function CRabbit:__Finalize()
	
	end
	
	-- ------------------------------
	--			Test Method
	-- ------------------------------
	-- function CRabbit:is_player_detected()
		-- local p = self.position
		-- --local l_Point = Vect2f(self.position.x, self.position.z)
		-- local l_PointA = self.locked
		-- if l_PointA == true then
			-- --print_logger(0, "IsPlayerDetected: TRUE")
		-- else
			-- --print_logger(0, "IsPlayerDetected: FALSE")
		-- end
		-- local point = self.position2
	-- end
	
	function CRabbit:is_fatigued()
		return self.fatigue <= 0
	end

	