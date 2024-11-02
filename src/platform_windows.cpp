
#ifdef __PLATFORM_WINDOWS__
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#undef min
#undef max
#undef far
#undef near
#endif

bool platform_init( MemoryArena *inMemory )
{
	platform = inMemory->permanent.allocate<Platform>( 1, true );
	platform->memory = *inMemory;
	memory = &platform->memory;
	platform->startCycles = __rdtsc();
	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );
	platform->cycleFrequency = freq.QuadPart;
	return true;
}

void platform_delay( i32 msWait )
{
	#ifdef _WIN32
		Sleep( msWait );
	#else
		sleep( msWait );
	#endif
}

u64 platform_get_seed()
{
	return time( nullptr ) + __rdtsc();
}

u64 platform_get_tick_counter()
{
	return __rdtsc();
}

u64 platform_get_tick_frequency()
{
	return platform->cycleFrequency;
}