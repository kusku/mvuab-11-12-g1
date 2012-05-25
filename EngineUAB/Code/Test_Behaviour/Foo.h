#pragma once

#include <luabind\luabind.hpp>
#include <string>

#include "Base.h"
#include "Core.h"
#include "Logger\Logger.h"

class CFoo
{
public:
		CFoo(luabind::object self, const char* name) 
			: m_Self(self)
			, m_Name(name)
		{}

		~CFoo(void)	{}

		void Think() {	LOGGER->AddNewLog( ELL_INFORMATION, "ha llegado hasta el think!!!");	}

		void Kill(luabind::object target) 
		{
			 // Check if our target is a valid object
			 if (target && luabind::type(target) != LUA_TNIL) 
			 {
				 // Convert the luabind::object into our own class type
				 // notice the pointer form so we'll get just that, rather than a copy
				 CFoo* target_foo = luabind::object_cast<CFoo*>(target);
				 std::cout << m_Name << " attacks " << target_foo->m_Name << "!" << std::endl;

				 // Lets alter the target's properties
				 target_foo->m_Name = "Dead";
			 }
			 else
			 {
				 LOGGER->AddNewLog( ELL_ERROR, "FALLO PARA LLEGAR A Kill!!!");
			 }
		}



	// Class attributes
	luabind::object m_Self; // Reference to our own Lua object
	std::string m_Name; // Our object's name

};

