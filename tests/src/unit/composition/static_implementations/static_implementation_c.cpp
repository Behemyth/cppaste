
#include "static_implementation_c.h"

#include "synodic/system/platform.h"

// This should never compile, as it provides an example of a platform specific library
// The testing framework will always be on a known platform, so use Platform::UNKNOWN
#if SYNODIC_PLATFORM(UNKNOWN)

int StaticImplementationC::Get() const
{
	return potato();
}

#endif