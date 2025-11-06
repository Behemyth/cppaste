export module example.platform.windows;

import std;
import synodic.cppaste.composition;
import example.platform.interface;

export namespace example
{
	/**
	 * @brief Windows platform implementation
	 */
	struct WindowsPlatform
	{
		static consteval auto Enabled() -> bool
		{
#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
			return true;
#else
			return false;
#endif
		}

		static consteval auto Priority() -> std::uint8_t
		{
			return 100;	 // Highest priority when enabled
		}

		constexpr auto GetPlatformName() const -> std::string_view
		{
			return "Windows";
		}

		constexpr auto GetExecutableExtension() const -> std::string_view
		{
			return ".exe";
		}

		constexpr auto GetDynamicLibraryExtension() const -> std::string_view
		{
			return ".dll";
		}

		void PrintInfo(std::ostream& out) const
		{
			out << "=== Windows Platform Properties ===\n"
				<< "Platform Name: " << GetPlatformName() << '\n'
				<< "Executable Extension: " << GetExecutableExtension() << '\n'
				<< "Dynamic Library Extension: " << GetDynamicLibraryExtension() << '\n';
		}
	};
}
