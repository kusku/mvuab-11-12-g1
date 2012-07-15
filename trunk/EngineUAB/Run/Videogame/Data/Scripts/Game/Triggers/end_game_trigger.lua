class 'CEndGameTrigger' (CBoxTrigger)
	function CEndGameTrigger:__init()
		CBoxTrigger.__init(self)
	end
	
	function CEndGameTrigger:update(elapsed_time)
	end
	
	function CEndGameTrigger:on_enter()
		change_to_end_gui_process()
	end
	
	function CEndGameTrigger:on_execute()
	end
	
	function CEndGameTrigger:on_exit()
	end