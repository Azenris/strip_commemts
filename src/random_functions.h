
#ifndef _HG_RANDOM_FUNCTIONS
#define _HG_RANDOM_FUNCTIONS

void random_set_seed( u64 seed );
void random_set_seed( const u64 seed[ 9 ] );
void random_get_state( u64 state[ 9 ] );

/// @func irandom( max )
/// @desc Return a random number ranged from 0 to max (inclusive)
/// @param	{u32}	max (inclusive)
/// @return	{u32}	random_number
[[nodiscard]] u32 irandom( u32 max );

/// @func irandom_range( min, max )
/// @desc Return a random number ranged from min to max (inclusive)
/// @param	{u32}	min (inclusive)
/// @param	{u32}	max (inclusive)
/// @return	{u32}	random_number
[[nodiscard]] u32 irandom_range( u32 min, u32 max );

/// @func irandom( max )
/// @desc Return a random number ranged from 0 to max (inclusive)
/// @param	{i32}	max (inclusive)
/// @return	{i32}	random_number
[[nodiscard]] i32 irandom( i32 max );

/// @func irandom_range( min, max )
/// @desc Return a random number ranged from min to max (inclusive)
/// @param	{i32}	min (inclusive)
/// @param	{i32}	max (inclusive)
/// @return	{i32}	random_number
[[nodiscard]] i32 irandom_range( i32 min, i32 max );

/// @func irandom64( max )
/// @desc Return a random number ranged from 0 to max (inclusive)
/// @param	{i64}	max (inclusive)
/// @return	{i64}	random_number
[[nodiscard]] i64 irandom( i64 max );

/// @func irandom_range64( min, max )
/// @desc Return a random number ranged from min to max (inclusive)
/// @param	{i64}	min (inclusive)
/// @param	{i64}	max (inclusive)
/// @return	{i64}	random_number
[[nodiscard]] i64 irandom_range( i64 min, i64 max );

/// @func irandom64( max )
/// @desc Return a random number ranged from 0 to max (inclusive)
/// @param	{u64}	max (inclusive)
/// @return	{u64}	random_number
[[nodiscard]] u64 irandom( u64 max );

/// @func irandom_range64( min, max )
/// @desc Return a random number ranged from min to max (inclusive)
/// @param	{u64}	min (inclusive)
/// @param	{u64}	max (inclusive)
/// @return	{u64}	random_number
[[nodiscard]] u64 irandom_range( u64 min, u64 max );

/// @func random( max )
/// @desc Return a random number ranged from 0 to max (inclusive)
/// @param	{f32}	max (inclusive)
/// @return	{f32}	random_number
[[nodiscard]] f32 random( f32 max );

/// @func random_range( min, max )
/// @desc Return a random number ranged from min to max (inclusive)
/// @param	{f32}	min (inclusive)
/// @param	{f32}	max (inclusive)
/// @return	{f32}	random_number
[[nodiscard]] f32 random_range( f32 min, f32 max );

/// @func random_f32_0_1()
/// @desc Return a random number ranged from [0, 1)
/// @return	{f32}	random_number
[[nodiscard]] f32 random_f32_0_1();

/// @func random_f64_0_1()
/// @desc Return a random number ranged from [0, 1)
/// @return	{f64}	random_number
[[nodiscard]] f64 random_f64_0_1();

/// @func proc( chance )
/// @desc Percent chance to proc something (inclusive)
/// @param	{i32}	chance (0-100)
/// @return	{bool}	proc
[[nodiscard]] bool iproc( i32 chance );

/// @func proc( chance )
/// @desc Percent chance to proc something (inclusive)
/// @param	{i64}	chance (0-100)
/// @return	{bool}	proc
[[nodiscard]] bool iproc( i64 chance );

/// @func proc( chance )
/// @desc Percent chance to proc something (inclusive)
/// @param	{u64}	chance (0-100)
/// @return	{bool}	proc
[[nodiscard]] bool iproc( u64 chance );

/// @func proc( chance )
/// @desc Percent chance to proc something (inclusive)
/// @param	{f32}	chance : upto 5 decimal places max (0-100)
/// @return	{bool}	proc
[[nodiscard]] bool proc( f32 chance );

// -------------------------------------------------------

// This code is splitmix64 & xoshiro256starstar & xoshiro256plus
// It has been slightly changed to fit my project.
// For the original code see the links:
// https://prng.di.unimi.it/splitmix64.c
// https://xoshiro.di.unimi.it/xoshiro256starstar.c
// https://xoshiro.di.unimi.it/xoshiro256plus.c
// For the license see third_party/THIRD_PARTY_LICENSES

struct Splitmix64
{
	u64 state;

	u64 next()
	{
		u64 z = ( state += 0x9e3779b97f4a7c15 );
		z = ( z ^ ( z >> 30 ) ) * 0xbf58476d1ce4e5b9;
		z = ( z ^ ( z >> 27 ) ) * 0x94d049bb133111eb;
		return z ^ ( z >> 31 );
	}
};

struct Xoshiro256starstar
{
	u64 state[ 4 ];

	u64 next();

	/* This is the jump function for the generator. It is equivalent
	   to 2^128 calls to next(); it can be used to generate 2^128
	   non-overlapping subsequences for parallel computations. */
	void jump();

	/* This is the long-jump function for the generator. It is equivalent to
	   2^192 calls to next(); it can be used to generate 2^64 starting poi32s,
	   from each of which jump() will generate 2^64 non-overlapping
	   subsequences for parallel distributed computations. */
	void long_jump();
};

struct Xoshiro256plus
{
	u64 state[ 4 ];

	u64 next();
	f32 nextf();
	f64 nextd();

	/* This is the jump function for the generator. It is equivalent
	   to 2^128 calls to next(); it can be used to generate 2^128
	   non-overlapping subsequences for parallel computations. */
	void jump();

	/* This is the long-jump function for the generator. It is equivalent to
	   2^192 calls to next(); it can be used to generate 2^64 starting poi32s,
	   from each of which jump() will generate 2^64 non-overlapping
	   subsequences for parallel distributed computations. */
	void long_jump();
};

#endif // _HG_RANDOM_FUNCTIONS

// ---------------------------------------------------------------------------------------

#if defined( RANDOM_FUNCTIONS_IMPLEMENTATION )

// This code is splitmix64 & xoshiro256starstar & xoshiro256plus
// It has been slightly changed to fit my project.
// For the original code see the links:
// https://prng.di.unimi.it/splitmix64.c
// https://xoshiro.di.unimi.it/xoshiro256starstar.c
// https://xoshiro.di.unimi.it/xoshiro256plus.c
// For the license see third_party/THIRD_PARTY_LICENSES

[[nodiscard]] static inline u64 rotl( const u64 x, const i32 k )
{
	return ( x << k ) | ( x >> ( 64 - k ) );
}

// -------------------------------------------------------

u64 Xoshiro256starstar::next()
{
	const u64 result = rotl( state[ 1 ] * 5, 7 ) * 9;

	const u64 t = state[ 1 ] << 17;

	state[ 2 ] ^= state[ 0 ];
	state[ 3 ] ^= state[ 1 ];
	state[ 1 ] ^= state[ 2 ];
	state[ 0 ] ^= state[ 3 ];

	state[ 2 ] ^= t;

	state[ 3 ] = rotl( state[ 3 ], 45 );

	return result;
}

void Xoshiro256starstar::jump()
{
	static const u64 JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

	u64 s0 = 0;
	u64 s1 = 0;
	u64 s2 = 0;
	u64 s3 = 0;

	for ( i32 i = 0; i < sizeof JUMP / sizeof *JUMP; ++i )
	{
		for ( i32 b = 0; b < 64; ++b )
		{
			if ( JUMP[ i ] & UINT64_C( 1 ) << b )
			{
				s0 ^= state[ 0 ];
				s1 ^= state[ 1 ];
				s2 ^= state[ 2 ];
				s3 ^= state[ 3 ];
			}

			next();
		}
	}

	state[ 0 ] = s0;
	state[ 1 ] = s1;
	state[ 2 ] = s2;
	state[ 3 ] = s3;
}

void Xoshiro256starstar::long_jump()
{
	static const u64 LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

	u64 s0 = 0;
	u64 s1 = 0;
	u64 s2 = 0;
	u64 s3 = 0;

	for ( i32 i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; ++i )
	{
		for ( i32 b = 0; b < 64; ++b )
		{
			if ( LONG_JUMP[ i ] & UINT64_C( 1 ) << b)
			{
				s0 ^= state[ 0 ];
				s1 ^= state[ 1 ];
				s2 ^= state[ 2 ];
				s3 ^= state[ 3 ];
			}

			next();
		}
	}

	state[ 0 ] = s0;
	state[ 1 ] = s1;
	state[ 2 ] = s2;
	state[ 3 ] = s3;
}

// -------------------------------------------------------

u64 Xoshiro256plus::next()
{
	const u64 result = state[ 0 ] + state[ 3 ];

	const u64 t = state[ 1 ] << 17;

	state[ 2 ] ^= state[ 0 ];
	state[ 3 ] ^= state[ 1 ];
	state[ 1 ] ^= state[ 2 ];
	state[ 0 ] ^= state[ 3 ];

	state[ 2 ] ^= t;

	state[ 3 ] = rotl( state[ 3 ], 45 );

	return result;
}

inline f32 Xoshiro256plus::nextf()
{
	return static_cast<f32>( next() >> 11 ) * ( 1.f / ( UINT64_C( 1 ) << 53 ) );
}

inline f64 Xoshiro256plus::nextd()
{
	return static_cast<double>( next() >> 11 ) * ( 1. / ( UINT64_C( 1 ) << 53 ) );
}

void Xoshiro256plus::jump()
{
	static const u64 JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

	u64 s0 = 0;
	u64 s1 = 0;
	u64 s2 = 0;
	u64 s3 = 0;

	for ( i32 i = 0; i < sizeof JUMP / sizeof *JUMP; ++i )
	{
		for ( i32 b = 0; b < 64; ++b )
		{
			if ( JUMP[ i ] & UINT64_C( 1 ) << b )
			{
				s0 ^= state[ 0 ];
				s1 ^= state[ 1 ];
				s2 ^= state[ 2 ];
				s3 ^= state[ 3 ];
			}

			next();
		}
	}

	state[ 0 ] = s0;
	state[ 1 ] = s1;
	state[ 2 ] = s2;
	state[ 3 ] = s3;
}

void Xoshiro256plus::long_jump()
{
	static const u64 LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

	u64 s0 = 0;
	u64 s1 = 0;
	u64 s2 = 0;
	u64 s3 = 0;

	for ( i32 i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; ++i )
	{
		for ( i32 b = 0; b < 64; ++b )
		{
			if ( LONG_JUMP[ i ] & UINT64_C( 1 ) << b )
			{
				s0 ^= state[ 0 ];
				s1 ^= state[ 1 ];
				s2 ^= state[ 2 ];
				s3 ^= state[ 3 ];
			}

			next();
		}
	}

	state[ 0 ] = s0;
	state[ 1 ] = s1;
	state[ 2 ] = s2;
	state[ 3 ] = s3;
}

// --------------------------------------------------------------------------------------------------------------------------------
// End of modified xoshiro256starstar & splitmix64
// --------------------------------------------------------------------------------------------------------------------------------

void random_set_seed( u64 seed )
{
	platform->splitmix64.state = seed;
	platform->xoshiro256starstar.state[ 0 ] = platform->splitmix64.next();
	platform->xoshiro256starstar.state[ 1 ] = platform->splitmix64.next();
	platform->xoshiro256starstar.state[ 2 ] = platform->splitmix64.next();
	platform->xoshiro256starstar.state[ 3 ] = platform->splitmix64.next();
	platform->xoshiro256plus.state[ 0 ] = platform->splitmix64.next();
	platform->xoshiro256plus.state[ 1 ] = platform->splitmix64.next();
	platform->xoshiro256plus.state[ 2 ] = platform->splitmix64.next();
	platform->xoshiro256plus.state[ 3 ] = platform->splitmix64.next();
}

void random_set_seed( const u64 seed[ 9 ] )
{
	platform->splitmix64.state = seed[ 0 ];
	platform->xoshiro256starstar.state[ 0 ] = seed[ 1 ];
	platform->xoshiro256starstar.state[ 1 ] = seed[ 2 ];
	platform->xoshiro256starstar.state[ 2 ] = seed[ 3 ];
	platform->xoshiro256starstar.state[ 3 ] = seed[ 4 ];
	platform->xoshiro256plus.state[ 0 ] = seed[ 5 ];
	platform->xoshiro256plus.state[ 1 ] = seed[ 6 ];
	platform->xoshiro256plus.state[ 2 ] = seed[ 7 ];
	platform->xoshiro256plus.state[ 3 ] = seed[ 8 ];
}

void random_get_state( u64 state[ 9 ] )
{
	state[ 0 ] = platform->splitmix64.state;
	state[ 1 ] = platform->xoshiro256starstar.state[ 0 ];
	state[ 2 ] = platform->xoshiro256starstar.state[ 1 ];
	state[ 3 ] = platform->xoshiro256starstar.state[ 2 ];
	state[ 4 ] = platform->xoshiro256starstar.state[ 3 ];
	state[ 5 ] = platform->xoshiro256plus.state[ 0 ];
	state[ 6 ] = platform->xoshiro256plus.state[ 1 ];
	state[ 7 ] = platform->xoshiro256plus.state[ 2 ];
	state[ 8 ] = platform->xoshiro256plus.state[ 3 ];
}

// -------------------------------------------------------

[[nodiscard]] u32 irandom( u32 max )
{
	return static_cast<u32>( platform->xoshiro256plus.nextf() * ( max + 1 ) );
}

[[nodiscard]] u32 irandom_range( u32 min, u32 max )
{
	if ( min > max )
	{
		u32 temp = min;
		min = max;
		max = temp;
	}

	return min + static_cast<u32>( platform->xoshiro256plus.nextf() * ( max - min + 1 ) );
}

[[nodiscard]] i32 irandom( i32 max )
{
	return static_cast<i32>( platform->xoshiro256plus.nextf() * ( max + sign( max ) ) );
}

[[nodiscard]] i32 irandom_range( i32 min, i32 max )
{
	if ( min > max )
	{
		i32 temp = min;
		min = max;
		max = temp;
	}

	return min + static_cast<i32>( platform->xoshiro256plus.nextf() * ( max - min + 1 ) );
}

[[nodiscard]] i64 irandom( i64 max )
{
	return static_cast<i64>( platform->xoshiro256plus.nextf() * ( max + sign( max ) ) );
}

[[nodiscard]] i64 irandom_range( i64 min, i64 max )
{
	if ( min > max )
	{
		i64 temp = min;
		min = max;
		max = temp;
	}

	return min + static_cast<i64>( platform->xoshiro256plus.nextf() * ( max - min + 1 ) );
}

[[nodiscard]] u64 irandom( u64 max )
{
	return static_cast<u64>( platform->xoshiro256plus.nextf() * ( max + 1 ) );
}

[[nodiscard]] u64 irandom_range( u64 min, u64 max )
{
	if ( min > max )
	{
		u64 temp = min;
		min = max;
		max = temp;
	}

	return min + static_cast<u64>( platform->xoshiro256plus.nextf() * ( max - min + 1 ) );
}

[[nodiscard]] f32 random( f32 max )
{
	return platform->xoshiro256plus.nextf() * max;
}

[[nodiscard]] f32 random_range( f32 min, f32 max )
{
	if ( min > max )
	{
		f32 temp = min;
		min = max;
		max = temp;
	}

	return min + static_cast<f32>( platform->xoshiro256plus.nextf() * ( max - min ) );
}

[[nodiscard]] f32 random_f32_0_1()
{
	return platform->xoshiro256plus.nextf();
}

[[nodiscard]] f64 random_f64_0_1()
{
	return platform->xoshiro256plus.nextd();
}

[[nodiscard]] bool iproc( i32 chance )
{
	return chance > irandom( 99 );
}

[[nodiscard]] bool iproc( i64 chance )
{
	return chance > irandom( 99 );
}

[[nodiscard]] bool iproc( u64 chance )
{
	return chance > irandom( 99 );
}

[[nodiscard]] bool proc( f32 chance )
{
	return static_cast<i32>( chance * 100000 ) > irandom( 99 * 100000 );
}

#endif