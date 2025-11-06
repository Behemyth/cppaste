export module example.platform.linux;

import std;
import synodic.cppaste.composition;
import example.platform.interface;

export namespace example
{
	/**
	 * @brief Linux platform implementation
	 */
	struct LinuxPlatform
	{
		static consteval auto Enabled() -> bool
		{
#if defined(__linux__) || defined(__gnu_linux__)
			return true;
#else
			return false;
#endif
		}

		static consteval auto Priority() -> std::uint8_t
		{
			return 100; // Highest priority when enabled
		}

		constexpr auto GetPlatformName() const -> std::string_view
		{
			return "Linux";
		}

		constexpr auto GetExecutableExtension() const -> std::string_view
		{
			return ""; // No extension on Linux
		}

		constexpr auto GetDynamicLibraryExtension() const -> std::string_view
		{
			return ".so";
		}

		void PrintInfo(std::ostream& out) const
		{
			out << "=== Linux Platform Properties ===\n"
				<< "Platform Name: " << GetPlatformName() << '\n'
				<< "Executable Extension: (none)\n"
				<< "Dynamic Library Extension: " << GetDynamicLibraryExtension() << '\n';
		}
	};
}
