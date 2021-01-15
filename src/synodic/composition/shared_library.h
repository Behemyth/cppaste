#pragma once

#include <filesystem>

#include "module.h"


namespace cppaste
{

	template<typename T>
	concept shared_library = true;
	
	class SharedLibrary : public StaticInterface<SharedLibrary>
	{
		uint16_t Open(const std::filesystem::path& path)
		{
			
		}

		void Close(uint16_t handle)
		{
			
		}
		
	};

}
