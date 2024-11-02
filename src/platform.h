
#pragma once

struct Platform
{
	bool running;
	u64 startCycles;
	u64 cycleFrequency;
	u64 frame;
	MemoryArena memory;
	char workingDirectory[ MAX_FILEPATH ];
	Splitmix64 splitmix64;
	Xoshiro256starstar xoshiro256starstar;
	Xoshiro256plus xoshiro256plus;
};

bool platform_init( MemoryArena *memory );
u64 platform_get_seed();
void platform_delay( i32 msWait );