#pragma once

enum class PlatformID { WINDOWS, OSX, LINUX, ANDROID, UNKNOWN };

class Platform {

public:

	constexpr static PlatformID GetPlatform();
	constexpr static bool IsDesktop();

private:

	#if defined(__linux__) && !defined(__ANDROID__)

	constexpr static PlatformID platform_ = PlatformID::LINUX;

	#elif defined(_WIN64)

	constexpr static PlatformID platform_ = PlatformID::WINDOWS;

	#elif defined(__APPLE__)

	constexpr static PlatformID platform_ = PlatformID::OSX;

	#elif defined(__ANDROID__) 

	constexpr static PlatformID platform_ = PlatformID::ANDROID;

	#else

	constexpr static PlatformID platform_ = PlatformID::UNKNOWN;

	#endif

};