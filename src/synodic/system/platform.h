#pragma once

enum class PlatformID
{
	WINDOWS,
	OSX,
	LINUX,
	ANDROID,
	UNKNOWN
};

#define SYNODIC_PLATFORM(X) SYNODIC_PLATFORM_PRIVATE_##X()

class Platform
{

public:
	constexpr static auto GetPlatform() -> PlatformID;

private:
#if defined(_WIN64)

	constexpr static PlatformID platform_ = PlatformID::WINDOWS;
#	define SYNODIC_PLATFORM_PRIVATE_WINDOWS() 1
#	define SYNODIC_PLATFORM_PRIVATE_OSX()	   0
#	define SYNODIC_PLATFORM_PRIVATE_LINUX()   0
#	define SYNODIC_PLATFORM_PRIVATE_ANDROID() 0
#	define SYNODIC_PLATFORM_PRIVATE_UNKNOWN() 0

#elif defined(__APPLE__)

	constexpr static PlatformID platform_ = PlatformID::OSX;
#	define SYNODIC_PLATFORM_PRIVATE_WINDOWS() 0
#	define SYNODIC_PLATFORM_PRIVATE_OSX()	   1
#	define SYNODIC_PLATFORM_PRIVATE_LINUX()   0
#	define SYNODIC_PLATFORM_PRIVATE_ANDROID() 0
#	define SYNODIC_PLATFORM_PRIVATE_UNKNOWN() 0

#elif defined(__linux__) && !defined(__ANDROID__)

	constexpr static PlatformID platform_ = PlatformID::LINUX;
#	define SYNODIC_PLATFORM_PRIVATE_WINDOWS() 0
#	define SYNODIC_PLATFORM_PRIVATE_OSX()	   0
#	define SYNODIC_PLATFORM_PRIVATE_LINUX()   1
#	define SYNODIC_PLATFORM_PRIVATE_ANDROID() 0
#	define SYNODIC_PLATFORM_PRIVATE_UNKNOWN() 0

#elif defined(__ANDROID__)

	constexpr static PlatformID platform_ = PlatformID::ANDROID;
#	define SYNODIC_PLATFORM_PRIVATE_WINDOWS() 0
#	define SYNODIC_PLATFORM_PRIVATE_OSX()	   0
#	define SYNODIC_PLATFORM_PRIVATE_LINUX()   0
#	define SYNODIC_PLATFORM_PRIVATE_ANDROID() 1
#	define SYNODIC_PLATFORM_PRIVATE_UNKNOWN() 0

#else

	constexpr static PlatformID platform_ = PlatformID::UNKNOWN;
#	define SYNODIC_PLATFORM_PRIVATE_WINDOWS() 0
#	define SYNODIC_PLATFORM_PRIVATE_OSX()	   0
#	define SYNODIC_PLATFORM_PRIVATE_LINUX()   0
#	define SYNODIC_PLATFORM_PRIVATE_ANDROID() 0
#	define SYNODIC_PLATFORM_PRIVATE_UNKNOWN() 1

#endif
};