	
-- class 'Bar' (CFoo)
	-- -- Bar's constructor, notice the super() method on the first row
	-- function Bar:__init(name)
	 -- super(self, name); -- Call for Foo::Foo(self, name)
	 -- print("A " .. self.name .. " a day keeps the doctor away.");
	-- end

	-- -- Bar's overloading method for Think()
	-- function Bar:Think()
	 -- print(self.name .. " is cool.");
	-- end

	-- -- Baz's constructor, the same method for super() applies as to Bar
	-- function Baz:__init(name)
	 -- super(self, name); -- Call for Foo::Foo(self, name)
	 -- print("A " .. self.name .. " is a wonderful thing, don't you think?");
	-- end


-- class 'Baz' (Foo)
	-- -- Baz's constructor, the same method for super() applies as to Bar
	-- function Baz:__init(name)
	 -- super(self, name); -- Call for Foo::Foo(self, name)
	 -- print("A " .. self.name .. " is a wonderful thing, don't you think?");
	-- end



class 'CEnemy' (CCharacter)
	function CEnemy:__init()
		CCharacter.__init(self,0)
		print_logger(0, "Inicio el enemic")
	end
	
	function CPlayer:update(elapsed_time)
		print_logger(0, "update")
	end
	
	function CEnemy:__Finalize()
	
	end
	

