#pragma once

#include <filesystem>


namespace synodic
{

	///--------------------------------------------------------------------------------
	/// @brief Application unaware extensions
	///--------------------------------------------------------------------------------

	class Plugin
	{
	};

	template<class T>
	void ImportSymbol(const std::filesystem::path& library, std::string& name)
	{
		//boost::shared_ptr<boost::dll::shared_library> p =
		//	boost::make_shared<boost::dll::shared_library>(lib, mode);
		//return std::shared_ptr<T>(p, boost::addressof(p->get<T>(name)));
	}

}
