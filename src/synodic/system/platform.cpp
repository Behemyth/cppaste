#include "platform.h"

constexpr PlatformID Platform::GetPlatform()
{
	return platform_;
}

constexpr bool Platform::IsDesktop()
{
	return platform_ == PlatformID::LINUX || platform_ == PlatformID::WINDOWS ||
		   platform_ == PlatformID::OSX;
}