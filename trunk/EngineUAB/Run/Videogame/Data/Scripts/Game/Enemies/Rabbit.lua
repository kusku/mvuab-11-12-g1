-- ------------------------------------------------------------------------
--	Clase CRabbit
-- ------------------------------------------------------------------------
class 'CRabbit' (CCharacter)
	
	-- ------------------------------
	--		Load States Methods
	-- ------------------------------
	function CRabbit:load_graphic_states()
		-- print_logger(0, "CRabbit::load_graphic_states->Loading animated states...")
		
		-- Idle State --
		self.animation_idle_state = CRabbitAnimatedIdleState("rabbit_idle_animation")
		if self.animation_idle_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado IDLE del conejito")
		end		
		
		-- Idle 2 State --
		self.animation_idle2_state = CRabbitAnimatedIdleState("rabbit_idle2_animation")
		if self.animation_idle2_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado IDLE2 del conejito")
		end	
		
		-- Walk State --
		self.animation_walk_state = CRabbitAnimatedWalkState("rabbit_walk_animation")
		if self.animation_walk_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado WALK del conejito")
		end
		
		-- Left lateral State --
		self.animation_left_lateral_state = CRabbitAnimatedLeftLateralState("rabbit_left_lateral_animation")
		if self.animation_left_lateral_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado LEFT LATERAL del conejito")
		end
		
		-- Right lateral State --
		self.animation_right_lateral_state = CRabbitAnimatedRightLateralState("rabbit_right_lateral_animation")
		if self.animation_right_lateral_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado RIGHT LATERAL del conejito")
		end
		
		-- Run State --
		self.animation_run_state = CRabbitAnimatedRunState("rabbit_run_animation")
		if self.animation_run_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado RUN del conejito")		
		end
		
		-- Still Attack State --
		self.animation_still_attack_state = CRabbitAnimatedStillAttackState("rabbit_still_attack_animation")
		if self.animation_still_attack_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado STILL ATTACK del conejito")
		end
		
		-- Run Attack State --
		self.animation_run_attack_state = CRabbitAnimatedRunAttackState("rabbit_run_attack_animation")
		if self.animation_run_attack_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado RUN ATTACK del conejito")
		end
		
		-- Hit State --
		self.animation_hit_state = CRabbitAnimatedHitState("rabbit_hit_animation")
		if self.animation_hit_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado HIT del conejito")
		end
		
		-- Defense State --
		self.animation_defense_state = CRabbitAnimatedDefenseState("rabbit_defense_animation")
		if self.animation_defense_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado DEFENSE del conejito")
		end
		
		-- Death State --
		self.animation_death_state = CRabbitAnimatedDeathState("rabbit_death_animation")
		if self.animation_death_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado DEATH del conejito")
		end
		
		-- Jump State --
		self.animation_jump_state = CRabbitAnimatedJumpState("rabbit_jump_animation")
		if self.animation_jump_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado JUMP del conejito")
		end
		
		-- print_logger(0, "CRabbit::load_graphic_states->Animated states loaded")
	end 
	
	function CRabbit:load_logic_states()
		-- print_logger(0, "CRabbit::load_logic_states->Loading logic states...")
		
		-- Idle State --
		self.idle_state = CRabbitIdleState("rabbit_idle")
		if self.idle_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado IDLE del conejito")
		end		
		
		-- Seek State -- Ir directamente a un destino
		self.seek_state = CRabbitSeekState("rabbit_seek")
		if self.seek_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado SEEK del conejito")
		end	
		
		-- Flee State -- Huye directamente de un destino
		self.flee_state = CRabbitFleeState("rabbit_flee")
		if self.flee_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado FLEE del conejito")
		else 
			print_logger(0, "CRabbit::load_logic_states->Creat estat FLEE del conejito")
		end	
		
		-- Tired State -- Cuando el rabbit se cansa de pegar ostias
		self.tired_state = CRabbitTiredState("rabbit_tired")
		if self.tired_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado TIRED del conejito")
		end	
		
		-- -- Pursuit State -- Persigue intelijentemente
		self.pursuit_state = CRabbitPursuitState("rabbit_pursuit")
		if self.pursuit_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado PURSUIT del conejito")
		end	
		
		-- Attack State --	Entra en estado de ataque al player
		self.prepared_to_attack_state = CRabbitPreparedToAttackState("rabbit_prepared_to_attack")
		if self.prepared_to_attack_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado de PREPARED ATTACK del conejito")
		end	
		
		-- Attack State --	Entra en estado de ataque al player
		self.attack_state = CRabbitAttackState("rabbit_attack")
		if self.attack_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado ATTACK del conejito")
		end	
		
		-- Attack 1 State --  Entra en estado de ataque 1 al player
		self.still_attack_state = CRabbitStillAttackState("rabbit_still_attack")
		if self.still_attack_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado STILL ATTACK del conejito")
		end	
		
		-- Attack 2 State --	Entra en estado de ataque 1 al player
		self.run_attack_state = CRabbitRunAttackState("rabbit_run_attack")
		if self.run_attack_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado RUN ATTACK del conejito")
		end	
		
		-- Defense State --
		self.defense_state = CRabbitDefenseState("rabbit_defense")
		if self.defense_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado DEFENSE del conejito")
		end
		
		-- Jump State --
		self.jump_state = CRabbitJumpState("rabbit_jump")
		if self.jump_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado JUMP del conejito")
		end
		
		-- Hit State --
		self.hit_state = CRabbitHitState("CRabbitHitState")
		if self.hit_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado HIT del conejito")
		end
		
		print_logger(0, "CRabbit::load_logic_states->Logic states loaded")
	end	
	
	-- ------------------------------
	--			Constructors
	-- ------------------------------	
	function CRabbit:__init( _id )
		-- print_logger(0, "CRabbit::__init->Llamada al constructor __init( _id )")
		CCharacter.__init( self, _id )
		self.yaw = 0.0
		self.pitch = -math.pi / 8
		self.roll = 0.0
		self.locked = false
		
		self.character_manager = get_game_process():get_character_manager()
		self.player = get_game_process():get_character_manager():get_player()
		
		if ( self.player == nil ) then
			print_logger ( 2, "CRabbit::__init()->player is nil" )
		end
		
		-- Cargamos estados
		self:load_logic_states()
		-- self:load_graphic_states()
	end
	
	function CRabbit:__init( _id, _name )
		-- print_logger(0, "CRabbit:init()->Llamada al constructor __init( _id, _name)")
		CCharacter.__init( self, _id, _name )
		--self.yaw = 0.0
		self.pitch = -math.pi / 8
		--self.roll = 0.0
		--self.position = Vect3f(3.0, 0.0, 0.0)
		--self.position2 = self.position
		self.locked = false
		
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
		print_logger(0, "CRabbit::init()->Inicializamos estados del enemigo")
		local l_IsOk = false

		self:get_animation_model():clear_cycle( self:get_animation_id("run"), 0.3 )
		self:get_animation_model():clear_cycle( self:get_animation_id("attack_1"), 0.3 )
		self:get_animation_model():clear_cycle( self:get_animation_id("attack_2"), 0.3 )
		self:get_animation_model():clear_cycle( self:get_animation_id("idle"), 0.3 )
		self:get_animation_model():clear_cycle( self:get_animation_id("walk"), 0.3 )
		self:get_animation_model():clear_cycle( self:get_animation_id("hit"), 0.3 )
		
		l_gfsm = self.graphic_fsm 
		if (l_gfsm ~= nil) then
			l_gfsm.current_state = self.animation_idle_state 
			l_IsOk = true
		end

		l_lfsm = self.logic_fsm 
		if (l_lfsm ~= nil) then
			l_lfsm.current_state = self.idle_state
			num = self:get_animation_id("idle")
			self:get_animation_model():blend_cycle( num, 0.3 )
			l_IsOk = l_IsOk and true
		end
		
		self.behaviors:add_behaviour( CSeek() )
		self.behaviors:add_behaviour( CFlee(self.properties.panic_distance) )
		self.behaviors:add_behaviour( CPursuit() )
		self.behaviors:add_behaviour( CEvade(self.properties.panic_distance) )
		-- self.behaviors:add_behaviour( CArrive() )
		self.behaviors:add_behaviour( CWander() )
		self.behaviors:add_behaviour( CCollisionAvoidance(5) )
		
		-- Flocking --
		self.behaviors:add_behaviour( CSeparation() )
		self.behaviors:add_behaviour( CAlignment() )
		self.behaviors:add_behaviour( CCohesion() )
		
		self.behaviors:seek_off()
		self.behaviors:flee_off()
		self.behaviors:arrive_off()
		self.behaviors:pursuit_off()
		self.behaviors:evade_off()
		self.behaviors:wander_off()
		self.behaviors:separation_off()
		self.behaviors:alignment_off()
		self.behaviors:cohesion_off()
		self.behaviors:collision_avoidance_off()
		self.behaviors:obstacle_wall_avoidance_off()
		
		-- --self.steering_entity.velocity = self.steering_entity.velocity + self.behaviors:update( 1, self.steering_entity )
		-- -- print_logger (0	, "CRabbit::Update()->ENEMY VELOCITY :"..self.steering_entity.velocity.x.." "..self.player.position.y.." "..self.steering_entity.velocity.y)
		-- --self.physic_controller.position = self.steering_entity.velocity * 1
		
		-- -- print_logger (0	, "CRabbit::Update()->PLAYER POSITION :"..self.player.position.x.." "..self.player.position.y.." "..self.player.position.y)
		
		self.position 				 	= self.physic_controller.position
		self.steering_entity.position 	= self.physic_controller.position
		self.behaviors.seek.target 		= self.player.position
		self.behaviors.flee.target 		= self.player.position
		self.behaviors.pursuit.target  	= self.player.position
		self.behaviors.evade.target		= self.player.position
		self.behaviors.evade:update_pursuer_entity(self.player.steering_entity, 5)
		-- self.behaviors.arrive.target 	= self.player.position
		-- self.behaviors.wander.target 	=  Vect3f(-30,-30,0)
		self.behaviors.wander.target 	= self.player.position
		
		self:move_to2( self.position, 0 )
		
		-- Nº de hits que el conejo ha realizado
		self.hits_done = 0
		
		-- nº de hits que se le permite recibir al enemigo antes de empezar a bloquear. cuando llega al valor se pasa a estado de defense
		-- cada ciertos golpeos pasaré a un estado de defensa. estas variables me permitiràn controlarlo. 
		self.received_hits_x_minut = 0				-- conteo de golpeos 
		self.total_received_hits_x_minut = 3		-- Total permitidos antes de pasar a estado defense
		
		return l_IsOk
	end
	
	function CRabbit:update_ia(_elapsed_time)
		-- print_logger(0, "CRabbit::updateIA()->Actualizando IA enemigo...")
		-- Calculamos la fuerza hacia el objetivo
		if ( self.steering_entity == nil ) then
			print_logger(2, "CRabbit::updateIA()->Error en self.steering_entity == nil")
			return
		end 
		local l_steering_force = self.behaviors:update( _elapsed_time, self.steering_entity ) 
		local l_steering_force2 = Vect3f( l_steering_force.x, 0, l_steering_force.z )
		
		-- aceleración = fuerza/masa
		local l_acceleration  = l_steering_force2 / self.steering_entity.mass
		-- print_logger(1, "CRabbit::update()->Acceleracio X: "..l_acceleration.x.." la Y : "..l_acceleration.y.." la Z : "..l_acceleration.z )
		
		-- actualizamos la velocidad. Ya hemos comprobado en C++ su trucamiento con la max. velocidad
		local v = Vect3f(self.steering_entity.velocity.x, self.steering_entity.velocity.y, self.steering_entity.velocity.z)
		-- print_logger(1, "CRabbit::update()->Velocitat X: "..v.x.." la Y : "..v.y.." la Z : "..v.z )
		self.steering_entity.velocity =  self.steering_entity.velocity + l_acceleration * _elapsed_time 
		self.steering_entity.velocity = Vect3f (self.steering_entity.velocity.x, 0, self.steering_entity.velocity.z) 
		
		-- nos aseguramos que el rabbit no excede de la velocidad máxima permitida
		local l_Vel = Vect3f( self.steering_entity.velocity.x, self.steering_entity.velocity.y, self.steering_entity.velocity.z )
		-- self.steering_entity.velocity = l_Vel:truncate(self.steering_entity.max_speed)
		self.steering_entity.velocity = l_Vel
		
		-- actualizamos la posición
		self.steering_entity.position = self.steering_entity.position + self.steering_entity.velocity * _elapsed_time 
		
		-- Actualizamos el Heahing y Side de la entidad solo si esta tiene velocidad
		-- print_logger ( 1, "CRabbit:updateIA->Squared_length : "..self.steering_entity.velocity:squared_length() )
		if ( self.steering_entity.velocity:squared_length() > 0.00000001 ) then
			self.steering_entity.heading = self.steering_entity.velocity
			-- Ahora actualizamos el heading (Vector unitario velocidad) y su perpendicular
			self.steering_entity.heading:normalize(1.0)
			local vec = self.steering_entity.heading
			self.steering_entity.side = vec:perpendicular()
		else
			self.steering_entity.heading = self.steering_entity:get_front() 	-- Ahora actualizamos el heading (Vector unitario velocidad) y su perpendicular
			self.steering_entity.heading:normalize(1.0)
			local vec = self.steering_entity.heading
			self.steering_entity.side = vec:perpendicular()
		end
		
		-- Actualiza el heading del caracter para suabizarlo si está activado
		self.steering_entity:smoothing_off()
		-- self.steering_entity:smoothing_on()
		-- if (self.steering_entity:is_smoothing_on()) then
			-- self.steering_entity.smoothing_heading = self.steering_entity.heading_smoother:update(self.steering_entity.heading);
		-- end
	end 
	
	-- ------------------------------
	--			Main Methods
	-- ------------------------------
	function CRabbit:update(_elapsed_time)
		-- print_logger(0, "CRabbit::update()->Actualizando enemigo...")
		-- Almacenamos el elapsed time pq después será impossible de guardar
		self.elapsed_time = _elapsed_time
		
		-- Test ---
		-- Asignamos como objetivo el player a los comportamientos
		-- self.behaviors.seek.target 				=  self.player.position
		-- self.behaviors.pursuit.target  			= self.player.position
		-- self.behaviors.pursuit:update_evader_entity( self.player.steering_entity )
		-- self.behaviors.collision_avoidance.target 	=  self.player.position
		-- self.behaviors.flee.target 				=  self.player.position
		-- self.behaviors.wander.target 				= self.behaviors.wander.target
		-- self.behaviors.arrive.target 				= self.player.position
		
		-- self:update_ia(_elapsed_time)
		
		--Actualizamos los estados en caso de cambiar
		self.logic_fsm:update()
		self.graphic_fsm:update()
		
		-- self:face_to( self.steering_entity.position, self.elapsed_time )
		-- self:move_to2( self.steering_entity.velocity, self.elapsed_time )
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
	
	-- function CRabbit:is_fatigued()
		-- return self.fatigue <= 0
	-- end

	