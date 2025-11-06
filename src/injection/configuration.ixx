export module synodic.cppaste.injection:configuration;

import std;

export namespace cppaste
{

	// TODO: Constrain the configuration type
	template<typename T>
	concept is_configuration = true;

	/**
	 * @brief Configuration that bundles user-defined arguments which drive the injection process
	 */
	class Configuration
	{

		// TODO: Implement configuration of sources

		// TODO: Implement configuration of concepts

	};

	// TODO: Provide pre-defined configuration types

	using StackConfiguration = Configuration;

}
