-- ------------------------------------------------------------------------
--	Clase CRabbit
-- ------------------------------------------------------------------------
class 'CRabbit' (CCharacter)
	
	-- ------------------------------
	--		Load States Methods
	-- ------------------------------
	function CRabbit:load_graphic_states()
		-- Idle State --
		self.animation_idle_state = CRabbitAnimatedIdleState()
		if self.animation_idle_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado idle del conejito")
		end		
		
		-- Walk State --
		self.animation_walk_state = CRabbitAnimatedWalkState()
		if self.animation_walk_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado Walk del conejito")
		end
		
		-- Pursuit State --
		self.animation_pursuit_state = CRabbitAnimatedPursuitState()
		if self.animation_pursuit_state == nil then
			print_logger(2, "CRabbit::load_graphic_states->Error al cargar un estado animado PURSUIT del conejito")		
		end
		
		-- Attack State --
		self.animation_attack_state = CRabbitAnimatedAttackState()
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
		self.idle_state = CRabbitIdleState()
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
		self.flee_state = CRabbitFleeState()
		if self.flee_state == nil then
			print_logger(2, "CRabbit::load_logic_states->Error al cargar un estado FLEE del conejito")
		end	
		
		-- Pursuit State -- Persigue inteligentemente
		self.pursuit_state = CRabbitPursuitState()
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
		self.attack_state = CRabbitAttackState()
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
		self.hit_state = CRabbitHitState()
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
		
		self.fatigue = 2
		
		return l_IsOk
	end
	
	-- ------------------------------
	--			Main Methods
	-- ------------------------------
	function CRabbit:update(_elapsed_time)
		self.elapsed_time = _elapsed_time

		local l_lfsm = self.logic_fsm 
		if l_lfsm ~= nil then
			if (self.locked == false ) then
				l_lfsm:update()
			else
				l_lfsm:change_state(self.idle_state)
				self:move_to( Vect3f(0.0, 0.0, 0.0), _elapsed_time )
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
	function CRabbit:is_player_detected()
		local p = self.position
		--local l_Point = Vect2f(self.position.x, self.position.z)
		local l_PointA = self.locked
		if l_PointA == true then
			--print_logger(0, "IsPlayerDetected: TRUE")
		else
			--print_logger(0, "IsPlayerDetected: FALSE")
		end
		
		local point = self.position2
	end

	
	function CRabbit:is_fatigued()
		return self.fatigue <= 0
	end

	