class 'CWolfPathFollowState' (CState)
	function CWolfPathFollowState:__init(name) 
		CState.__init(self, name)
	end

	function CWolfPathFollowState:OnEnter(_CCharacter)
		--print_logger(0, "CWolfPathFollowState:Enter")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfPathFollowState:Execute(_CCharacter, _elapsed_time)
		--print_logger(0, "CWolfPathFollowState:Execute")
	end
	
	function CWolfPathFollowState:OnExit(_CCharacter)
		--print_logger(0, "CWolfPathFollowState:Exit")
		if not ( _CCharacter == nil ) then
		end
	end
	
	function CWolfPathFollowState:OnMessage(_CCharacter, _Msg)
		print_logger(0, "CWolfPathFollowState:OnMessage")
		return false
	end
	
	function CWolfPathFollowState:__Finalize()
	
	end