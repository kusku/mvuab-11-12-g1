class 'CRiverTrigger' (CBoxTrigger)
	function CRiverTrigger:__init()
		CBoxTrigger.__init(self)
	end
	
	function CRiverTrigger:update(elapsed_time)
	end
	
	function CRiverTrigger:on_enter()
		change_to_game_over_gui_process()
		
		self.active = false
	end
	
	function CRiverTrigger:on_stay()
	end
	
	function CRiverTrigger:on_exit()
	end