export module example.platform.macos;

import std;
import synodic.cppaste.composition;
import example.platform.interface;

export namespace example
{
	/**
	 * @brief macOS platform implementation
	 */
	struct MacOSPlatform
	{
		static consteval auto Enabled() -> bool
		{
#if defined(__APPLE__) && defined(__MACH__)
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
			return "macOS";
		}

		constexpr auto GetExecutableExtension() const -> std::string_view
		{
			return ""; // No extension on macOS
		}

		constexpr auto GetDynamicLibraryExtension() const -> std::string_view
		{
			return ".dylib";
		}

		void PrintInfo(std::ostream& out) const
		{
			out << "=== macOS Platform Properties ===\n"
				<< "Platform Name: " << GetPlatformName() << '\n'
				<< "Executable Extension: (none)\n"
				<< "Dynamic Library Extension: " << GetDynamicLibraryExtension() << '\n';
		}
	};
}
