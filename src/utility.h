
#pragma once

constexpr u64 MAX_CONVERT_TO_STRING_DIGITS = 32;

[[nodiscard]] inline i32 pow( i32 x, i32 y )
{
	return static_cast<i32>( powf( static_cast<float>( x ), static_cast<float>( y ) ) );
}

[[nodiscard]] inline u64 pow( u64 x, u64 y )
{
	return static_cast<u64>( pow( x, y ) );
}

template <typename T, typename... Args>
[[nodiscard]] T choose( T firstArg, Args... args )
{
	const T a[ sizeof...( args ) + 1 ] = { firstArg, args... };
	return a[ irandom( static_cast<u64>( sizeof...( args ) ) ) ];
}

[[nodiscard]] inline bool bytes_compare( const void *rhs, const void *lhs, u64 bytes )
{
	return memcmp( rhs, lhs, bytes );
}

[[nodiscard]] inline i32 compare_value( const char *lhs, const char *rhs );

[[nodiscard]] constexpr inline i32 compare_value( u16 lhs, u16 rhs )
{
	return lhs - rhs;
}

[[nodiscard]] constexpr inline i32 compare_value( u32 lhs, u32 rhs )
{
	return lhs - rhs;
}

[[nodiscard]] constexpr inline i32 compare_value( u64 lhs, u64 rhs )
{
	return lhs > rhs ? 1 : ( lhs < rhs ? - 1 : 0 );
}

[[nodiscard]] constexpr inline i32 compare_value( i16 lhs, i16 rhs )
{
	return lhs - rhs;
}

[[nodiscard]] constexpr inline i32 compare_value( i32 lhs, i32 rhs )
{
	return lhs - rhs;
}

[[nodiscard]] constexpr inline i32 compare_value( i64 lhs, i64 rhs )
{
	return lhs > rhs ? 1 : ( lhs < rhs ? - 1 : 0 );
}

[[nodiscard]] constexpr inline vec3 make_rgb_colour( u8 r, u8 g, u8 b )
{
	return { r / 255.f, g / 255.f, b / 255.f };
}

[[nodiscard]] constexpr inline vec3 make_rgb_colour( u32 colour )
{
	return { ( colour >> 24 ) / 255.f, ( ( colour >> 16 ) & 0xFF ) / 255.f, ( ( colour >> 8 ) & 0xFF ) / 255.f };
}

[[nodiscard]] constexpr inline vec4 make_rgba_colour( u8 r, u8 g, u8 b, u8 a = 255 )
{
	return { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
}

[[nodiscard]] constexpr inline vec4 make_rgba_colour( u32 colour )
{
	return { ( colour >> 24 ) / 255.f, ( ( colour >> 16 ) & 0xFF ) / 255.f, ( ( colour >> 8 ) & 0xFF ) / 255.f, ( colour & 0xFF ) / 255.f };
}

[[nodiscard]] constexpr inline vec3 make_hsv_colour( u8 h, u8 s, u8 v )
{
	return { h / 255.f, s / 255.f, v / 255.f };
}

[[nodiscard]] constexpr inline vec4 make_hsva_colour( u8 h, u8 s, u8 v, u8 a = 255 )
{
	return { h / 255.f, s / 255.f, v / 255.f, a / 255.f };
}

[[nodiscard]] constexpr inline vec4 make_hsva_colour( u32 colour )
{
	return { ( colour >> 24 ) / 255.f, ( ( colour >> 16 ) & 0xFF ) / 255.f, ( ( colour >> 8 ) & 0xFF ) / 255.f, ( colour & 0xFF ) / 255.f };
}

[[nodiscard]] inline bool time_alternate( f32 globalTime, f32 time )
{
	if ( time <= 0 ) return 0;
	return ( static_cast<i32>( globalTime / time ) & 1 ) == 0;
}

inline void read_data( void *dst, u8 *&src, u64 size, u8 *srcBarrier )
{
	assert( src + size <= srcBarrier );
	memcpy( dst, src, size );
	src += size;
}

template <typename T>
inline void read_data( T *dst, u8 *&src, u8 *srcBarrier )
{
	assert( src + sizeof( T ) <= srcBarrier );
	memcpy( dst, src, sizeof( T ) );
	src += sizeof( T );
}

inline void write_data( u8 *&dst, const void *src, u64 size, u8 *dstBarrier )
{
	assert( dst + size <= dstBarrier );
	memcpy( dst, src, size );
	dst += size;
}

template <typename T>
inline void write_data( u8 *&dst, const T *src, u8 *dstBarrier )
{
	assert( dst + sizeof( T ) <= dstBarrier );
	memcpy( dst, src, sizeof( T ) );
	dst += sizeof( T );
}

[[nodiscard]] i32 value_in_multiples( i32 value, i32 multiple );

[[nodiscard]] constexpr inline f32 lerp( f32 a, f32 b, f32 t )
{
	return a + t * ( b - a );
}

[[nodiscard]] constexpr inline f32 lerp( i32 a, i32 b, f32 t )
{
	return a + t * ( b - a );
}

[[nodiscard]] constexpr inline ivec2 lerp( ivec2 a, ivec2 b, f32 t )
{
	return to_ivec2( lerp( a.x, b.x, t ), lerp( a.y, b.y, t ) );
}

[[nodiscard]] constexpr inline ivec3 lerp( ivec3 a, ivec3 b, f32 t )
{
	return to_ivec3( lerp( a.x, b.x, t ), lerp( a.y, b.y, t ), lerp( a.z, b.z, t ) );
}

[[nodiscard]] constexpr inline ivec4 lerp( ivec4 a, ivec4 b, f32 t )
{
	return to_ivec4( lerp( a.x, b.x, t ), lerp( a.y, b.y, t ), lerp( a.z, b.z, t ), lerp( a.w, b.w, t ) );
}

[[nodiscard]] constexpr inline vec2 lerp( vec2 a, vec2 b, f32 t )
{
	return to_vec2( lerp( a.x, b.x, t ), lerp( a.y, b.y, t ) );
}

[[nodiscard]] constexpr inline vec3 lerp( vec3 a, vec3 b, f32 t )
{
	return to_vec3( lerp( a.x, b.x, t ), lerp( a.y, b.y, t ), lerp( a.z, b.z, t ) );
}

[[nodiscard]] constexpr inline vec4 lerp( vec4 a, vec4 b, f32 t )
{
	return to_vec4( lerp( a.x, b.x, t ), lerp( a.y, b.y, t ), lerp( a.z, b.z, t ), lerp( a.w, b.w, t ) );
}

[[nodiscard]] vec3 slerp( vec3 a, vec3 b, f32 t );

[[nodiscard]] constexpr inline vec3 merge_colour( vec3 colourA, vec3 colourB, f32 t )
{
	return vec3
	{
		lerp( colourA.x, colourB.x, t ),
		lerp( colourA.y, colourB.y, t ),
		lerp( colourA.z, colourB.z, t )
	};
}

[[nodiscard]] constexpr inline vec4 merge_colour( vec4 colourA, vec4 colourB, f32 t )
{
	return vec4
	{
		lerp( colourA.x, colourB.x, t ),
		lerp( colourA.y, colourB.y, t ),
		lerp( colourA.z, colourB.z, t ),
		lerp( colourA.w, colourB.w, t )
	};
}

/// @func map_range( range_0_value, range_0_min, range_0_max, range_1_min, range_1_max )
/// @desc Get the value between a range based on another set of ranges and a value
/// @param	range_0_value	Current value
/// @param	range_0_min		Min Value
/// @param	range_0_max		Max
/// @param	range_1_min		Target min value
/// @param	range_1_max		Target max value
/// @return	range_1_value	Target value
[[nodiscard]] constexpr inline f32 map_range( f32 range_0_value, f32 range_0_min, f32 range_0_max, f32 range_1_min, f32 range_1_max )
{
	return range_1_min + ( range_1_max - range_1_min ) * ( ( range_0_value - range_0_min ) / ( range_0_max - range_0_min ) );
}

/// @func perc_0_1( current, max )
/// @desc Get the percentage between 0 and 1.
/// @param	current
/// @param	max
/// @return	percent_0_1
[[nodiscard]] constexpr inline f32 perc_0_1( f32 current, f32 max )
{
	return current / max;
}

/// @func perc_0_1_inv( current, max )
/// @desc Get the inverse percentage between 0 and 1.
/// @param	current
/// @param	max
/// @return	percent_0_1
[[nodiscard]] constexpr inline f32 perc_0_1_inv( f32 current, f32 max )
{
	return 1 - perc_0_1( current, max );
}

/// @func perc_0_100( current, max )
/// @desc Get the percentage between 0-100
/// @param	current
/// @param	max
/// @return	percent_0_100
[[nodiscard]] constexpr inline f32 perc_0_100( f32 current, f32 max )
{
	return ( current / max ) * 100;
}

/// @func perc_in_range_0_1( current, min, max )
/// @desc Get the percentage between a range
/// @param	current
/// @param	min
/// @param	max
/// @return	percent_0_1
[[nodiscard]] constexpr inline f32 perc_in_range_0_1( f32 current, f32 min, f32 max )
{
	if ( max - min == 0 )
		return perc_0_1( current, min );

	return ( current - min ) / ( max - min );
}

/// @func perc_in_range_0_1_inv( current, min, max )
/// @desc Get the inverse percentage between a range
/// @param	current
/// @param	min
/// @param	max
/// @return	percent_0_1
[[nodiscard]] constexpr inline f32 perc_in_range_0_1_inv( f32 current, f32 min, f32 max )
{
	return 1 - perc_in_range_0_1( current, min, max );
}

/// @func perc_in_range_0_100( current, min, max )
/// @desc Get the percentage between a range
/// @param	current
/// @param	min
/// @param	max
/// @return	percent_0_100
[[nodiscard]] constexpr inline f32 perc_in_range_0_100( f32 current, f32 min, f32 max )
{
	if ( max - min == 0 )
		return perc_0_100( current, min );

	return ( ( current - min ) * 100 ) / ( max - min );
}

/// @func perc_value_0_1( value, perc_0_1 )
/// @desc Get a percentage of a value
/// @param	value
/// @param	perc_0_1
/// @return	perc_value
[[nodiscard]] constexpr inline f32 perc_value_0_1( f32 value, f32 perc_0_1 )
{
	return value * perc_0_1;
}

/// @func perc_value_0_100( value, perc_0_100 )
/// @desc Get a percentage of a value
/// @param	value
/// @param	perc_0_100
/// @return	perc_value
[[nodiscard]] constexpr inline f32 perc_value_0_100( f32 value, f32 perc_0_100 )
{
	return value * ( perc_0_100 / 100 );
}

/// @func value_in_range_0_1( percentage, min, max )
/// @desc Get the value based on a percentage_0-1
/// @param	percentage
/// @param	min
/// @param	max
/// @return	value
template <typename T>
[[nodiscard]] constexpr inline T value_in_range_0_1( f32 percentage, T min, T max )
{
	return static_cast<T>( percentage * ( max - min ) + min );
}

/// @func value_in_range_0_100( percentage, min, max )
/// @desc Get the value based on a percentage_0-100
/// @param	percentage
/// @param	min
/// @param	max
/// @return	value
[[nodiscard]] constexpr inline f32 value_in_range_0_100( f32 percentage, f32 min, f32 max )
{
	return ( percentage * ( max - min ) / 100 ) + min;
}

[[nodiscard]] inline f32 bilinear_interpolation( f32 tl, f32 tr, f32 br, f32 bl, i32 l, i32 r, i32 t, i32 b, i32 x, i32 y ) 
{
	f32 ld = static_cast<f32>( r - x );
	f32 td = static_cast<f32>( b - y );
	f32 bd = static_cast<f32>( y - t );
	f32 rd = static_cast<f32>( x - l );

	return 1.f / ( ( r - l ) * ( b - t ) ) *
	(
		tl * ld * td +
		tr * rd * td +
		br * rd * bd +
		bl * ld * bd
	);
}

[[nodiscard]] inline f32 bilinear_interpolation( f32 tl, f32 tr, f32 br, f32 bl, f32 l, f32 r, f32 t, f32 b, f32 x, f32 y ) 
{
	f32 ld = r - x;
	f32 td = b - y;
	f32 bd = y - t;
	f32 rd = x - l;

	return 1.f / ( ( r - l ) * ( b - t ) ) *
	(
		tl * ld * td +
		tr * rd * td +
		br * rd * bd +
		bl * ld * bd
	);
}

[[nodiscard]] vec4 bilinear_interpolation( vec4 tl, vec4 tr, vec4 br, vec4 bl, f32 x, f32 y );
[[nodiscard]] inline vec4 bilinear_interpolation( const vec4 v[ 4 ], f32 x, f32 y );

[[nodiscard]] inline u32 modulo( f32 x, u32 y )
{
	return static_cast<u32>( fmod( x, static_cast<f32>( y ) ) );
}

template <typename T>
[[nodiscard]] inline T sign( T value )
{
	return value > 0 ? 1 : ( value < 0 ? -1 : 0 );
}

template <>
[[nodiscard]] inline ivec2 sign( ivec2 value )
{
	return { sign( value.x ), sign( value.y ) };
}

template <typename T>
[[nodiscard]] inline T min( T a, T b )
{
	return a <= b ? a : b;
}

template <>
[[nodiscard]] inline ivec2 min( ivec2 a, ivec2 b )
{
	return { a.x <= b.x ? a.x : b.x, a.y <= b.y ? a.y : b.y };
}

template <>
[[nodiscard]] inline ivec3 min( ivec3 a, ivec3 b )
{
	return { a.x <= b.x ? a.x : b.x, a.y <= b.y ? a.y : b.y, a.z <= b.z ? a.z : b.z };
}

template <>
[[nodiscard]] inline ivec4 min( ivec4 a, ivec4 b )
{
	return { a.x <= b.x ? a.x : b.x, a.y <= b.y ? a.y : b.y, a.z <= b.z ? a.z : b.z, a.w <= b.w ? a.w : b.w };
}

template <>
[[nodiscard]] inline vec2 min( vec2 a, vec2 b )
{
	return { a.x <= b.x ? a.x : b.x, a.y <= b.y ? a.y : b.y };
}

template <>
[[nodiscard]] inline vec3 min( vec3 a, vec3 b )
{
	return { a.x <= b.x ? a.x : b.x, a.y <= b.y ? a.y : b.y, a.z <= b.z ? a.z : b.z };
}

template <>
[[nodiscard]] inline vec4 min( vec4 a, vec4 b )
{
	return { a.x <= b.x ? a.x : b.x, a.y <= b.y ? a.y : b.y, a.z <= b.z ? a.z : b.z, a.w <= b.w ? a.w : b.w };
}

template <typename T>
[[nodiscard]] inline T max( T a, T b )
{
	return a >= b ? a : b;
}

template <>
[[nodiscard]] inline ivec2 max( ivec2 a, ivec2 b )
{
	return { a.x >= b.x ? a.x : b.x, a.y >= b.y ? a.y : b.y };
}

template <>
[[nodiscard]] inline ivec3 max( ivec3 a, ivec3 b )
{
	return { a.x >= b.x ? a.x : b.x, a.y >= b.y ? a.y : b.y, a.z >= b.z ? a.z : b.z };
}

template <>
[[nodiscard]] inline ivec4 max( ivec4 a, ivec4 b )
{
	return { a.x >= b.x ? a.x : b.x, a.y >= b.y ? a.y : b.y, a.z >= b.z ? a.z : b.z, a.w >= b.w ? a.w : b.w };
}

template <>
[[nodiscard]] inline vec2 max( vec2 a, vec2 b )
{
	return { a.x >= b.x ? a.x : b.x, a.y >= b.y ? a.y : b.y };
}

template <>
[[nodiscard]] inline vec3 max( vec3 a, vec3 b )
{
	return { a.x >= b.x ? a.x : b.x, a.y >= b.y ? a.y : b.y, a.z >= b.z ? a.z : b.z };
}

template <>
[[nodiscard]] inline vec4 max( vec4 a, vec4 b )
{
	return { a.x >= b.x ? a.x : b.x, a.y >= b.y ? a.y : b.y, a.z >= b.z ? a.z : b.z, a.w >= b.w ? a.w : b.w };
}

i64 wrap( i64 value, i64 minValue, i64 maxValue, i64 mod );

[[nodiscard]] i32 random_range_wrapped( i32 min, i32 max, i32 wrap );
[[nodiscard]] f32 random_range_wrapped( f32 min, f32 max, f32 wrap );

template <typename T>
[[nodiscard]] inline T clamp( T v, T a, T b )
{
	return max( min( v, max( a, b ) ), min( a, b ) );
}

template <>
[[nodiscard]] inline ivec2 clamp( ivec2 v, ivec2 a, ivec2 b )
{
	ivec2 r;
	r.x = max( min( v.x, max( a.x, b.x ) ), min( a.x, b.x ) );
	r.y = max( min( v.y, max( a.y, b.y ) ), min( a.y, b.y ) );
	return r;
}

template <>
[[nodiscard]] inline ivec3 clamp( ivec3 v, ivec3 a, ivec3 b )
{
	ivec3 r;
	r.x = max( min( v.x, max( a.x, b.x ) ), min( a.x, b.x ) );
	r.y = max( min( v.y, max( a.y, b.y ) ), min( a.y, b.y ) );
	r.z = max( min( v.z, max( a.z, b.z ) ), min( a.z, b.z ) );
	return r;
}

template <>
[[nodiscard]] inline ivec4 clamp( ivec4 v, ivec4 a, ivec4 b )
{
	ivec4 r;
	r.x = max( min( v.x, max( a.x, b.x ) ), min( a.x, b.x ) );
	r.y = max( min( v.y, max( a.y, b.y ) ), min( a.y, b.y ) );
	r.z = max( min( v.z, max( a.z, b.z ) ), min( a.z, b.z ) );
	r.w = max( min( v.w, max( a.w, b.w ) ), min( a.w, b.w ) );
	return r;
}

template <>
[[nodiscard]] inline vec2 clamp( vec2 v, vec2 a, vec2 b )
{
	vec2 r;
	r.x = max( min( v.x, max( a.x, b.x ) ), min( a.x, b.x ) );
	r.y = max( min( v.y, max( a.y, b.y ) ), min( a.y, b.y ) );
	return r;
}

template <>
[[nodiscard]] inline vec3 clamp( vec3 v, vec3 a, vec3 b )
{
	vec3 r;
	r.x = max( min( v.x, max( a.x, b.x ) ), min( a.x, b.x ) );
	r.y = max( min( v.y, max( a.y, b.y ) ), min( a.y, b.y ) );
	r.z = max( min( v.z, max( a.z, b.z ) ), min( a.z, b.z ) );
	return r;
}

template <>
[[nodiscard]] inline vec4 clamp( vec4 v, vec4 a, vec4 b )
{
	vec4 r;
	r.x = max( min( v.x, max( a.x, b.x ) ), min( a.x, b.x ) );
	r.y = max( min( v.y, max( a.y, b.y ) ), min( a.y, b.y ) );
	r.z = max( min( v.z, max( a.z, b.z ) ), min( a.z, b.z ) );
	r.w = max( min( v.w, max( a.w, b.w ) ), min( a.w, b.w ) );
	return r;
}

template <typename T>
[[nodiscard]] inline T abs( T value )
{
	return value < 0 ? -value : value;
}

template <>
[[nodiscard]] inline vec2 abs( vec2 value )
{
	return { ::abs( value.x ), ::abs( value.y ) };
}

template <>
[[nodiscard]] inline vec3 abs( vec3 value )
{
	return { ::abs( value.x ), ::abs( value.y ), ::abs( value.z ) };
}

template <>
[[nodiscard]] inline vec4 abs( vec4 value )
{
	return { ::abs( value.x ), ::abs( value.y ), ::abs( value.z ), ::abs( value.w ) };
}

template <>
[[nodiscard]] inline ivec2 abs( ivec2 value )
{
	return { ::abs( value.x ), ::abs( value.y ) };
}

template <>
[[nodiscard]] inline ivec3 abs( ivec3 value )
{
	return { ::abs( value.x ), ::abs( value.y ), ::abs( value.z ) };
}

template <>
[[nodiscard]] inline ivec4 abs( ivec4 value )
{
	return { ::abs( value.x ), ::abs( value.y ), ::abs( value.z ), ::abs( value.w ) };
}

template <typename T>
[[nodiscard]] inline T closest_to_zero( T lhs, T rhs )
{
	return abs( lhs ) < abs( rhs ) ? lhs : rhs;
}

template <>
[[nodiscard]] inline vec2 closest_to_zero( vec2 lhs, vec2 rhs )
{
	return { closest_to_zero( lhs.x, rhs.x ), closest_to_zero( lhs.y, rhs.y ) };
}

template <>
[[nodiscard]] inline vec3 closest_to_zero( vec3 lhs, vec3 rhs )
{
	return { closest_to_zero( lhs.x, rhs.x ), closest_to_zero( lhs.y, rhs.y ), closest_to_zero( lhs.z, rhs.z ) };
}

template <>
[[nodiscard]] inline vec4 closest_to_zero( vec4 lhs, vec4 rhs )
{
	return { closest_to_zero( lhs.x, rhs.x ), closest_to_zero( lhs.y, rhs.y ), closest_to_zero( lhs.z, rhs.z ), closest_to_zero( lhs.w, rhs.w ) };
}

template <>
[[nodiscard]] inline ivec2 closest_to_zero( ivec2 lhs, ivec2 rhs )
{
	return { closest_to_zero( lhs.x, rhs.x ), closest_to_zero( lhs.y, rhs.y ) };
}

template <>
[[nodiscard]] inline ivec3 closest_to_zero( ivec3 lhs, ivec3 rhs )
{
	return { closest_to_zero( lhs.x, rhs.x ), closest_to_zero( lhs.y, rhs.y ), closest_to_zero( lhs.z, rhs.z ) };
}

template <>
[[nodiscard]] inline ivec4 closest_to_zero( ivec4 lhs, ivec4 rhs )
{
	return { closest_to_zero( lhs.x, rhs.x ), closest_to_zero( lhs.y, rhs.y ), closest_to_zero( lhs.z, rhs.z ), closest_to_zero( lhs.w, rhs.w ) };
}

template <typename T>
[[nodiscard]] inline T floorf( T value )
{
	return ::floorf( value );
}

template <>
[[nodiscard]] inline vec2 floorf( vec2 value )
{
	return { ::floorf( value.x ), ::floorf( value.y ) };
}

template <>
[[nodiscard]] inline vec3 floorf( vec3 value )
{
	return { ::floorf( value.x ), ::floorf( value.y ), ::floorf( value.z ) };
}

template <>
[[nodiscard]] inline vec4 floorf( vec4 value )
{
	return { ::floorf( value.x ), ::floorf( value.y ), ::floorf( value.z ), ::floorf( value.w ) };
}

template <typename T>
[[nodiscard]] inline T ceilf( T value )
{
	return ::ceilf( value );
}

template <>
[[nodiscard]] inline vec2 ceilf( vec2 value )
{
	return { ::ceilf( value.x ), ::ceilf( value.y ) };
}

template <>
[[nodiscard]] inline vec3 ceilf( vec3 value )
{
	return { ::ceilf( value.x ), ::ceilf( value.y ), ::ceilf( value.z ) };
}

template <>
[[nodiscard]] inline vec4 ceilf( vec4 value )
{
	return { ::ceilf( value.x ), ::ceilf( value.y ), ::ceilf( value.z ), ::ceilf( value.w ) };
}

[[nodiscard]] inline i32 next_power_of_2( i32 number )
{
	assert( number >= 0 );
	i32 power = 1;
	while ( power < number )
		power <<= 1;
	return power;
}

[[nodiscard]] inline u32 next_power_of_2( u32 number )
{
	u32 power = 1;
	while ( power < number )
		power <<= 1;
	return power;
}

[[nodiscard]] inline bool is_power_of_2( i8 number )
{
	return ( number & ( number - 1 ) ) == 0;
}

[[nodiscard]] inline bool is_power_of_2( i16 number )
{
	return ( number & ( number - 1 ) ) == 0;
}

[[nodiscard]] inline bool is_power_of_2( i32 number )
{
	return ( number & ( number - 1 ) ) == 0;
}

[[nodiscard]] inline bool is_power_of_2( i64 number )
{
	return ( number & ( number - 1 ) ) == 0;
}

[[nodiscard]] inline bool is_power_of_2( u8 number )
{
	return ( number & ( number - 1 ) ) == 0;
}

[[nodiscard]] inline bool is_power_of_2( u16 number )
{
	return ( number & ( number - 1 ) ) == 0;
}

[[nodiscard]] inline bool is_power_of_2( u32 number )
{
	return ( number & ( number - 1 ) ) == 0;
}

[[nodiscard]] inline bool is_power_of_2( u64 number )
{
	return ( number & ( number - 1 ) ) == 0;
}

[[nodiscard]] inline ivec2 irandom_range( ivec2 lhs, ivec2 rhs )
{
	return { irandom_range( lhs.x, rhs.x ), irandom_range( lhs.y, rhs.y ) };
}

[[nodiscard]] inline vec4 random_range( vec4 lhs, vec4 rhs )
{
	return { random_range( lhs.x, rhs.x ), random_range( lhs.y, rhs.y ), random_range( lhs.z, rhs.z ), random_range( lhs.w, rhs.w ) };
}

[[nodiscard]] f32 increment_real( f32 value );
[[nodiscard]] f32 decrement_real( f32 value );
[[nodiscard]] f64 increment_real( f64 value );
[[nodiscard]] f64 decrement_real( f64 value );

inline void set_magic_value( char( &destination )[ 4 ], const char magic[ 4 ] )
{
	destination[ 0 ] = magic[ 0 ];
	destination[ 1 ] = magic[ 1 ];
	destination[ 2 ] = magic[ 2 ];
	destination[ 3 ] = magic[ 3 ];
}

[[nodiscard]] inline bool has_magic_value( const char value[ 4 ], const char magic[ 4 ] )
{
	return value[ 0 ] == magic[ 0 ] &&
		value[ 1 ] == magic[ 1 ] &&
		value[ 2 ] == magic[ 2 ] &&
		value[ 3 ] == magic[ 3 ];
}

[[nodiscard]] vec3 rgb_to_hsv( vec3 colour );
[[nodiscard]] vec3 hsv_to_rgb( vec3 colour );

[[nodiscard]] inline vec4 rgb_to_hsv( vec4 colour )
{
	vec3 hsvColour = rgb_to_hsv( vec3{ colour.r, colour.g, colour.b } );
	return { hsvColour.h, hsvColour.s, hsvColour.v, colour.a };
}

[[nodiscard]] inline vec4 hsv_to_rgb( vec4 colour )
{
	vec3 rgbColour = hsv_to_rgb( vec3{ colour.h, colour.s, colour.v } );
	return { rgbColour.r, rgbColour.g, rgbColour.b, colour.a };
}

[[nodiscard]] inline vec4 time_colour_pulse( f32 time, f32 rate, vec4 start, vec4 end )
{
	return merge_colour( start, end, ( sinf( ( fmodf( time, rate ) / rate ) * M_TAU ) + 1.f ) / 2.f );
}

[[nodiscard]] u64 convert_to_u64( const char *input, const char **output = nullptr );

[[nodiscard]] inline u32 convert_to_u32( const char *input, const char **output = nullptr )
{
	return static_cast<u32>( convert_to_u64( input, output ) );
}

[[nodiscard]] inline u16 convert_to_u16( const char *input, const char **output = nullptr )
{
	return static_cast<u16>( convert_to_u64( input, output ) );
}

[[nodiscard]] inline u8 convert_to_u8( const char *input, const char **output = nullptr )
{
	return static_cast<u8>( convert_to_u64( input, output ) );
}

[[nodiscard]] i64 convert_to_i64( const char *input, const char **output = nullptr );

[[nodiscard]] inline i32 convert_to_i32( const char *input, const char **output = nullptr )
{
	return static_cast<i32>( clamp( convert_to_i64( input, output ), (i64)INT64_MIN, (i64)INT64_MAX ) );
}

[[nodiscard]] inline i16 convert_to_i16( const char *input, const char **output = nullptr )
{
	return static_cast<i16>( clamp( convert_to_i64( input, output ), (i64)INT16_MIN, (i64)INT16_MAX ) );
}

[[nodiscard]] inline i8 convert_to_i8( const char *input, const char **output = nullptr )
{
	return static_cast<i8>( clamp( convert_to_i64( input, output ), (i64)INT8_MIN, (i64)INT8_MAX ) );
}

[[nodiscard]] inline i32 convert_to_int( const char *input, const char **output = nullptr )
{
	return convert_to_i32( input, output );
}

[[nodiscard]] f32 convert_to_float( const char *input, const char **output = nullptr );
[[nodiscard]] bool convert_to_bool( const char *input, const char **output = nullptr );
[[nodiscard]] bool is_number_floating_point( const char *input );
[[nodiscard]] vec3 convert_to_vec3( const char *input, const char **output = nullptr );
[[nodiscard]] vec4 convert_to_vec4( const char *input, const char **output = nullptr );

[[nodiscard]] u32 convert_hex_string_to_u32( const char *input, const char **output = nullptr );

[[nodiscard]] inline vec3 convert_hex_string_to_rgb_colour( const char *input, const char **output = nullptr )
{
	return make_rgb_colour( convert_hex_string_to_u32( input, output ) );
}

[[nodiscard]] inline vec4 convert_hex_string_to_rgba_colour( const char *input, const char **output = nullptr )
{
	return make_rgba_colour( convert_hex_string_to_u32( input, output ) );
}

u64 convert_to_string( char *dest, u64 destSize, u64 value, i32 radix = 10, i32 trailing = 0 );

inline u64 convert_to_string( char *dest, u64 destSize, u32 value, i32 radix = 10, i32 trailing = 0 )
{
	return convert_to_string( dest, destSize, static_cast<u64>( value ), radix, trailing );
}

inline u64 convert_to_string( char *dest, u64 destSize, u16 value, i32 radix = 10, i32 trailing = 0 )
{
	return convert_to_string( dest, destSize, static_cast<u64>( value ), radix, trailing );
}

inline u64 convert_to_string( char *dest, u64 destSize, u8 value, i32 radix = 10, i32 trailing = 0 )
{
	return convert_to_string( dest, destSize, static_cast<u64>( value ), radix, trailing );
}

u64 convert_to_string( char *dest, u64 destSize, i64 value, i32 radix = 10, i32 trailing = 0 );

inline u64 convert_to_string( char *dest, u64 destSize, i32 value, i32 radix = 10, i32 trailing = 0 )
{
	return convert_to_string( dest, destSize, static_cast<i64>( value ), radix, trailing );
}

inline u64 convert_to_string( char *dest, u64 destSize, i16 value, i32 radix = 10, i32 trailing = 0 )
{
	return convert_to_string( dest, destSize, static_cast<i64>( value ), radix, trailing );
}

inline u64 convert_to_string( char *dest, u64 destSize, i8 value, i32 radix = 10, i32 trailing = 0 )
{
	return convert_to_string( dest, destSize, static_cast<i64>( value ), radix, trailing );
}

u64 convert_to_string( char *dest, u64 destSize, f32 value, i32 fracDigits = 2, bool clipZeroFrac = false );
u64 convert_to_string( char *dest, u64 destSize, bool value );
u64 convert_to_string( char *dest, u64 destSize, ivec3 value );
u64 convert_to_hex_string( char *dest, u64 destSize, vec3 value, bool uppercase = true );
u64 convert_to_hex_string( char *dest, u64 destSize, vec4 value, bool uppercase = true );

[[nodiscard]] const char *convert_to_string( u64 value, i32 radix = 10, i32 trailing = 0 );
[[nodiscard]] const char *convert_to_string( u32 value, i32 radix = 10, i32 trailing = 0 );
[[nodiscard]] const char *convert_to_string( u16 value, i32 radix = 10, i32 trailing = 0 );
[[nodiscard]] const char *convert_to_string( u8 value, i32 radix = 10, i32 trailing = 0 );

[[nodiscard]] const char *convert_to_string( i64 value, i32 radix = 10, i32 trailing = 0 );
[[nodiscard]] const char *convert_to_string( i32 value, i32 radix = 10, i32 trailing = 0 );
[[nodiscard]] const char *convert_to_string( i16 value, i32 radix = 10, i32 trailing = 0 );
[[nodiscard]] const char *convert_to_string( i8 value, i32 radix = 10, i32 trailing = 0 );

[[nodiscard]] const char *convert_to_string( f32 value, i32 fracDigits, bool clipZeroFrac );
[[nodiscard]] const char *convert_to_string( bool value );

[[nodiscard]] const char *convert_to_hex_string( vec3 value, bool uppercase = true );
[[nodiscard]] const char *convert_to_hex_string( vec4 value, bool uppercase = true );

#ifdef INCLUDE_ZLIB

namespace zlib
{
	enum ZLIB_RESULT
	{
		ZLIB_OK,
		ZLIB_STREAM_END,
		ZLIB_NEED_DICT,
		ZLIB_STREAM_ERROR,
		ZLIB_DATA_ERROR,
		ZLIB_MEM_ERROR,
		ZLIB_BUF_ERROR,
		ZLIB_VERSION_ERROR,
		ZLIB_INPUT_ERROR,
		ZLIB_UNKNOWN,
	};

	i32 compress( u8 *dest, u64 *destSize, const u8 *source, u64 sourceSize, const char **error = nullptr );
	i32 uncompress( u8 *dest, u64 *destSize, const u8 *source, u64 sourceSize, const char **error = nullptr );
	[[nodiscard]] u32 compress_bound( u64 size );
}

#endif // INCLUDE_ZLIB

namespace base64
{
	bool encode( u8 *dest, u64 *destSize, const u8 *source, u64 sourceSize, const char **error = nullptr );
	bool decode( u8 *dest, u64 *destSize, const u8 *source, u64 sourceSize, const char **error = nullptr );
	[[nodiscard]] u64 encode_bound( u64 size );
}

void delay( i32 msWait );