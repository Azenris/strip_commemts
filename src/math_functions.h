
#ifndef _HG_MATH_FUNCTIONS
#define _HG_MATH_FUNCTIONS

constexpr const f32 M_TAU				= 6.28318530717958647692f;
constexpr const f32 M_PI				= 3.14159265358979323846f;
constexpr const f32 M_PI_2				= 1.57079632679489661923f;
constexpr const f32 M_PI_4				= 0.785398163397448309616f;
constexpr const f32 M_1_PI				= 0.318309886183790671538f;
constexpr const f32 M_2_PI				= 0.636619772367581343076f;
constexpr const f32 M_2_SQRTPI			= 1.12837916709551257390f;
constexpr const f32 M_SQRT2				= 1.41421356237309504880f;
constexpr const f32 M_SQRT1_2			= 0.707106781186547524401f;
constexpr const f32 M_E					= 2.71828182845904523536f;
constexpr const f32 M_LOG2E				= 1.44269504088896340736f;
constexpr const f32 M_LOG10E			= 0.434294481903251827651f;
constexpr const f32 M_LN2				= 0.693147180559945309417f;
constexpr const f32 M_LN10				= 2.30258509299404568402f;
constexpr const f32 M_FLOAT_EPSILON		= 1.192092896e-07f;

#pragma pack(push, 1)

struct vec2
{
	f32 x;
	f32 y;
};

#pragma warning(push)
#pragma warning(disable : 4201)

struct vec3
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
			f32 z;
		};
		struct
		{
			f32 r;
			f32 g;
			f32 b;
		};
		struct
		{
			f32 h;
			f32 s;
			f32 v;
		};
	};
};

struct vec4
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
			f32 z;
			f32 w;
		};
		struct
		{
			f32 r;
			f32 g;
			f32 b;
			f32 a;
		};
		struct
		{
			f32 h;
			f32 s;
			f32 v;
		};
	};
};

#pragma warning(pop)

struct ivec2
{
	i32 x;
	i32 y;
};

struct ivec3
{
	i32 x;
	i32 y;
	i32 z;
};

struct ivec4
{
	i32 x;
	i32 y;
	i32 z;
	i32 w;
};

struct mat2
{
	f32 data[ 4 ];
	// Column Major
	// f32 m_XY
	// f32 m_11; f32 m_12;
	// f32 m_21; f32 m_22;
	// []
	// 0, 2
	// 1, 3
};

struct mat3
{
	f32 data[ 9 ];
	// Column Major
	// f32 m_XY
	// f32 m_11; f32 m_12; f32 m_13;
	// f32 m_21; f32 m_22; f32 m_23;
	// f32 m_31; f32 m_32; f32 m_33;
	// []
	// 0, 3, 6
	// 1, 4, 7
	// 2, 5, 8
};

struct alignas( 16 ) mat4
{
	f32 data[ 16 ];
	// Column Major
	// f32 m_XY
	// f32 m_11; f32 m_12; f32 m_13; f32 m_14;
	// f32 m_21; f32 m_22; f32 m_23; f32 m_24;
	// f32 m_31; f32 m_32; f32 m_33; f32 m_34;
	// f32 m_41; f32 m_42; f32 m_43; f32 m_44;
	// []
	// 0, 4,  8, 12
	// 1, 5,  9, 13
	// 2, 6, 10, 14
	// 3, 7, 11, 15
};

struct Circle
{
	vec3 position;
	f32 radius;
};

struct Quaternion
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
};

struct Cube
{
	i32 l;
	i32 t;
	i32 r;
	i32 b;
	i32 zB;
	i32 zT;
};

#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( vec2 ) == sizeof( f32 ) * 2 );
static_assert( sizeof( vec3 ) == sizeof( f32 ) * 3 );
static_assert( sizeof( vec4 ) == sizeof( f32 ) * 4 );
static_assert( sizeof( ivec2 ) == sizeof( i32 ) * 2 );
static_assert( sizeof( ivec3 ) == sizeof( i32 ) * 3 );
static_assert( sizeof( ivec4 ) == sizeof( i32 ) * 4 );
static_assert( sizeof( Circle ) == sizeof( f32 ) * 4 );
static_assert( sizeof( Quaternion ) == sizeof( f32 ) * 4 );
static_assert( sizeof( mat2 ) == sizeof( f32 ) * 4 );
static_assert( sizeof( mat3 ) == sizeof( f32 ) * 9 );
static_assert( sizeof( mat4 ) == sizeof( f32 ) * 16 );

// utility //////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline f32 deg_to_rad( f32 degrees )
{
	return degrees * ( M_PI / 180.0f );
}

[[nodiscard]] constexpr inline f32 rad_to_deg( f32 radians )
{
	return radians * ( 180.0f / M_PI );
}

[[nodiscard]] constexpr inline vec3 deg_to_rad( vec3 degrees )
{
	return { deg_to_rad( degrees.x ), deg_to_rad( degrees.y ), deg_to_rad( degrees.z ) };
}

[[nodiscard]] constexpr inline vec3 rad_to_deg( vec3 radians )
{
	return { rad_to_deg( radians.x ), rad_to_deg( radians.y ), rad_to_deg( radians.z ) };
}

[[nodiscard]] inline f32 vec_to_rad( vec2 vec )
{
	f32 rad = atan2f( vec.y, vec.x );
	if ( rad < 0 )
		rad += 2 * M_PI;
	return rad;
}

[[nodiscard]] inline f32 vec_to_deg( vec2 vec )
{
	f32 rad = atan2f( vec.y, vec.x );
	if ( rad < 0 )
		rad += 2 * M_PI;
	return rad_to_deg( rad );
}

// @param {f32}	alpha	xy plane angle in radians
// @param {f32}	beta	relative z angle in radians
[[nodiscard]] inline vec3 angles_to_vec3( f32 alpha, f32 beta )
{
	return vec3
	{
		.x = cosf( alpha ) * cosf( beta ),
		.y = sinf( alpha ) * cosf( beta ) * -1.f,
		.z = sinf( beta )
	};
}

[[nodiscard]] inline vec2 deg_to_vec2( i32 deg )
{
	f32 rad = deg_to_rad( static_cast<f32>( deg ) );
	return { cosf( rad ), sinf( rad ) };
}

[[nodiscard]] inline vec2 deg_to_vec2( f32 deg )
{
	f32 rad = deg_to_rad( deg );
	return { cosf( rad ), sinf( rad ) };
}

[[nodiscard]] inline vec2 rad_to_vec2( f32 rad )
{
	return { cosf( rad ), sinf( rad ) };
}

// This division rounds down when a < 0 eg. -1/2 = -1 not 0
[[nodiscard]] constexpr inline i32 divide( i32 a, i32 b )
{
	return a / b + ( -( a % b < 0 ) & ( b < 0 ? 1 : -1 ) );
}

void calculate_view( vec3 position, vec3 rotation, mat4 *view );
void calculate_view( ivec3 position, ivec3 rotation, mat4 *view );

[[nodiscard]] constexpr inline bool is_valid_cube( Cube cube )
{
	return cube.l <= cube.r && cube.t <= cube.b && cube.zB <= cube.zT;
}

// vec2 /////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline f32 *ptr( vec2 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline const f32 *ptr( const vec2 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline vec2 to_vec2( vec3 vec ) { return { vec.x, vec.y }; }
[[nodiscard]] constexpr inline vec2 to_vec2( vec4 vec ) { return { vec.x, vec.y }; }
[[nodiscard]] constexpr inline vec2 to_vec2( ivec2 vec ) { return { (f32)vec.x, (f32)vec.y }; }
[[nodiscard]] constexpr inline vec2 to_vec2( ivec3 vec ) { return { (f32)vec.x, (f32)vec.y }; }
[[nodiscard]] constexpr inline vec2 to_vec2( ivec4 vec ) { return { (f32)vec.x, (f32)vec.y }; }
[[nodiscard]] constexpr inline vec2 to_vec2( f32 x, f32 y ) { return { x, y }; }
[[nodiscard]] constexpr inline vec2 to_vec2( f32 x, i32 y ) { return { x, (f32)y }; }
[[nodiscard]] constexpr inline vec2 to_vec2( i32 x, f32 y ) { return { (f32)x, y }; }
[[nodiscard]] constexpr inline vec2 to_vec2( i32 x, i32 y ) { return { (f32)x, (f32)y }; }

[[nodiscard]] constexpr inline bool operator == ( const vec2 &lhs, vec2 rhs )
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

[[nodiscard]] constexpr inline bool operator != ( const vec2 &lhs, vec2 rhs )
{
	return lhs.x != rhs.x || lhs.y != rhs.y;
}

[[nodiscard]] constexpr inline vec2 operator + ( const vec2 &lhs, vec2 rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator + ( const vec2 &lhs, f32 rhs )
{
	return { lhs.x + rhs, lhs.y + rhs };
}

[[nodiscard]] constexpr inline vec2 operator + ( f32 lhs, vec2 rhs )
{
	return { lhs + rhs.x, lhs + rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator - ( const vec2 &lhs, vec2 rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator - ( const vec2 &lhs, f32 rhs )
{
	return { lhs.x - rhs, lhs.y - rhs };
}

[[nodiscard]] constexpr inline vec2 operator - ( const vec2 &lhs, ivec2 rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator * ( const vec2 &lhs, vec2 rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator * ( const vec2 &lhs, ivec2 rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator * ( const vec2 &lhs, f32 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs };
}

[[nodiscard]] constexpr inline vec2 operator / ( const vec2 &lhs, vec2 rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator / ( const vec2 &lhs, ivec2 rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator / ( const vec2 &lhs, f32 rhs )
{
	return { lhs.x / rhs, lhs.y / rhs };
}

[[nodiscard]] constexpr inline vec2 operator / ( f32 lhs, vec2 rhs )
{
	return { lhs / rhs.x, lhs / rhs.y };
}

inline vec2 &operator += ( vec2 &lhs, vec2 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline vec2 &operator += ( vec2 &lhs, f32 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline vec2 &operator -= ( vec2 &lhs, vec2 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline vec2 &operator -= ( vec2 &lhs, f32 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline vec2 &operator *= ( vec2 &lhs, vec2 rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline vec2 &operator *= ( vec2 &lhs, f32 rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline vec2 &operator /= ( vec2 &lhs, vec2 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline vec2 &operator /= ( vec2 &lhs, f32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

[[nodiscard]] constexpr inline vec2 operator - ( vec2 vec )
{
	return { -vec.x, -vec.y };
}

[[nodiscard]] constexpr inline f32 dot( const vec2 &lhs, vec2 rhs )
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

[[nodiscard]] inline f32 magnitude( const vec2 &vec )
{
	return sqrtf( vec.x * vec.x + vec.y * vec.y );
}

[[nodiscard]] inline f32 length( const vec2 &vec )
{
	return sqrtf( vec.x * vec.x + vec.y * vec.y );
}

[[nodiscard]] constexpr inline f32 length_squared( const vec2 &vec )
{
	return vec.x * vec.x + vec.y * vec.y;
}

[[nodiscard]] constexpr inline vec2 normal( const vec2 &vec )
{
	return vec / magnitude( vec );
}

inline void normalise( vec2 &vec )
{
	vec /= magnitude( vec );
}

[[nodiscard]] inline vec2 vec2_rotate( vec2 point, f32 s, f32 c, vec2 pivotPoint )
{
	point -= pivotPoint;
	return { ( point.x * c - point.y * s ) + pivotPoint.x, ( point.x * s + point.y * c ) + pivotPoint.y };
}

[[nodiscard]] inline f32 distance( vec2 lhs, vec2 rhs )
{
	return length( lhs - rhs );
}

[[nodiscard]] inline f32 distance_sq( vec2 lhs, vec2 rhs )
{
	return length_squared( lhs - rhs );
}

[[nodiscard]] inline vec2 direction( vec2 lhs, vec2 rhs )
{
	return normal( lhs - rhs );
}

// vec3 /////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline f32 *ptr( vec3 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline const f32 *ptr( const vec3 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline vec3 to_vec3( vec2 vec ) { return { vec.x, vec.y, 0.f }; }
[[nodiscard]] constexpr inline vec3 to_vec3( vec2 vec, f32 z ) { return { vec.x, vec.y, z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( vec4 vec ) { return { vec.x, vec.y, vec.z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( ivec2 vec ) { return { (f32)vec.x, (f32)vec.y, 0.f }; }
[[nodiscard]] constexpr inline vec3 to_vec3( ivec2 vec, f32 z ) { return { (f32)vec.x, (f32)vec.y, z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( ivec2 vec, i32 z ) { return { (f32)vec.x, (f32)vec.y, (f32)z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( ivec3 vec ) { return { (f32)vec.x, (f32)vec.y, (f32)vec.z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( ivec4 vec ) { return { (f32)vec.x, (f32)vec.y, (f32)vec.z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( f32 x, f32 y, f32 z ) { return { x, y, z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( f32 x, f32 y, i32 z ) { return { x, y, (f32)z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( f32 x, i32 y, f32 z ) { return { x, (f32)y, z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( f32 x, i32 y, i32 z ) { return { x, (f32)y, (f32)z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( i32 x, f32 y, f32 z ) { return { (f32)x, y, z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( i32 x, f32 y, i32 z ) { return { (f32)x, y, (f32)z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( i32 x, i32 y, f32 z ) { return { (f32)x, (f32)y, z }; }
[[nodiscard]] constexpr inline vec3 to_vec3( i32 x, i32 y, i32 z ) { return { (f32)x, (f32)y, (f32)z }; }

[[nodiscard]] constexpr inline bool operator == ( const vec3 &lhs, const vec3 &rhs )
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

[[nodiscard]] constexpr inline bool operator != ( const vec3 &lhs, const vec3 &rhs )
{
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

[[nodiscard]] constexpr inline vec3 operator + ( const vec3 &lhs, const vec3 &rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

[[nodiscard]] constexpr inline vec3 operator + ( const vec3 &lhs, f32 rhs )
{
	return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs };
}

[[nodiscard]] constexpr inline vec3 operator - ( const vec3 &lhs, const vec3 &rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

[[nodiscard]] constexpr inline vec3 operator - ( const vec3 &lhs, f32 rhs )
{
	return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs };
}

[[nodiscard]] constexpr inline vec3 operator - ( const vec3 &lhs, const ivec3 &rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

[[nodiscard]] constexpr inline vec3 operator * ( const vec3 &lhs, const vec3 &rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

[[nodiscard]] constexpr inline vec3 operator * ( const vec3 &lhs, const ivec3 &rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

[[nodiscard]] constexpr inline vec3 operator * ( const vec3 &lhs, f32 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

[[nodiscard]] constexpr inline vec3 operator / ( const vec3 &lhs, const vec3 &rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
}

[[nodiscard]] constexpr inline vec3 operator / ( const vec3 &lhs, const ivec3 &rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
}

[[nodiscard]] constexpr inline vec3 operator / ( const vec3 &lhs, f32 rhs )
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
}

[[nodiscard]] constexpr inline vec3 operator / ( f32 lhs, const vec3 &rhs )
{
	return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z };
}

inline vec3 &operator += ( vec3 &lhs, const vec3 &rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline vec3 &operator += ( vec3 &lhs, f32 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline vec3 &operator -= ( vec3 &lhs, const vec3 &rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline vec3 &operator -= ( vec3 &lhs, f32 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline vec3 &operator -= ( vec3 &lhs, const ivec3 &rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline vec3 &operator *= ( vec3 &lhs, const vec3 &rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline vec3 &operator *= ( vec3 &lhs, f32 rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline vec3 &operator /= ( vec3 &lhs, const vec3 &rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline vec3 &operator /= ( vec3 &lhs, f32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

[[nodiscard]] constexpr inline vec3 operator - ( const vec3 &vec )
{
	return { -vec.x, -vec.y, -vec.z };
}

[[nodiscard]] constexpr inline vec3 cross_product( const vec3 &lhs, const vec3 &rhs )
{
	return { lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x };
}

[[nodiscard]] constexpr inline f32 dot( const vec3 &lhs, const vec3 &rhs )
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

[[nodiscard]] inline f32 magnitude( const vec3 &vec )
{
	return sqrtf( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z );
}

[[nodiscard]] inline f32 length( const vec3 &vec )
{
	return sqrtf( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z );
}

[[nodiscard]] constexpr inline f32 length_squared( const vec3 &vec )
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

[[nodiscard]] constexpr inline vec3 normal( const vec3 &vec )
{
	return vec / magnitude( vec );
}

inline void normalise( vec3 &vec )
{
	vec /= magnitude( vec );
}

[[nodiscard]] inline f32 distance( const vec3 &lhs, const vec3 &rhs )
{
	return length( lhs - rhs );
}

[[nodiscard]] inline f32 distance_sq( const vec3 &lhs, const vec3 &rhs )
{
	return length_squared( lhs - rhs );
}

[[nodiscard]] inline vec3 direction( const vec3 &lhs, const vec3 &rhs )
{
	return normal( lhs - rhs );
}

// vec4 /////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline f32 *ptr( vec4 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline const f32 *ptr( const vec4 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline vec4 to_vec4( vec2 vec, f32 z, f32 w ) { return { vec.x, vec.y, z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( vec3 vec, f32 w ) { return { vec.x, vec.y, vec.z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( ivec2 vec, f32 z, f32 w ) { return { (f32)vec.x, (f32)vec.y, z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( ivec2 vec, i32 z, i32 w ) { return { (f32)vec.x, (f32)vec.y, (f32)z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( ivec3 vec, f32 w ) { return { (f32)vec.x, (f32)vec.y, (f32)vec.z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( ivec3 vec, i32 w ) { return { (f32)vec.x, (f32)vec.y, (f32)vec.z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( ivec4 vec ) { return { (f32)vec.x, (f32)vec.y, (f32)vec.z, (f32)vec.w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( f32 x, f32 y, f32 z, f32 w ) { return { x, y, z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( f32 x, f32 y, f32 z, i32 w ) { return { x, y, z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( f32 x, f32 y, i32 z, f32 w ) { return { x, y, (f32)z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( f32 x, f32 y, i32 z, i32 w ) { return { x, y, (f32)z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( f32 x, i32 y, f32 z, f32 w ) { return { x, (f32)y, z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( f32 x, i32 y, f32 z, i32 w ) { return { x, (f32)y, z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( f32 x, i32 y, i32 z, f32 w ) { return { x, (f32)y, (f32)z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( f32 x, i32 y, i32 z, i32 w ) { return { x, (f32)y, (f32)z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( i32 x, f32 y, f32 z, f32 w ) { return { (f32)x, y, z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( i32 x, f32 y, f32 z, i32 w ) { return { (f32)x, y, z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( i32 x, f32 y, i32 z, f32 w ) { return { (f32)x, y, (f32)z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( i32 x, f32 y, i32 z, i32 w ) { return { (f32)x, y, (f32)z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( i32 x, i32 y, f32 z, f32 w ) { return { (f32)x, (f32)y, z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( i32 x, i32 y, f32 z, i32 w ) { return { (f32)x, (f32)y, z, (f32)w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( i32 x, i32 y, i32 z, f32 w ) { return { (f32)x, (f32)y, (f32)z, w }; }
[[nodiscard]] constexpr inline vec4 to_vec4( i32 x, i32 y, i32 z, i32 w ) { return { (f32)x, (f32)y, (f32)z, (f32)w }; }

[[nodiscard]] constexpr inline bool operator == ( const vec4 &lhs, const vec4 &rhs )
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

[[nodiscard]] constexpr inline bool operator != ( const vec4 &lhs, const vec4 &rhs )
{
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
}

[[nodiscard]] constexpr inline vec4 operator + ( const vec4 &lhs, const vec4 &rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

[[nodiscard]] constexpr inline vec4 operator + ( const vec4 &lhs, vec2 rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.x, lhs.w + rhs.y };
}

[[nodiscard]] constexpr inline vec4 operator + ( const vec4 &lhs, f32 rhs )
{
	return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs };
}

[[nodiscard]] constexpr inline vec4 operator + ( const vec4 &lhs, ivec2 rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.x, lhs.w + rhs.y };
}

[[nodiscard]] constexpr inline vec4 operator - ( const vec4 &lhs, const vec4 &rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

[[nodiscard]] constexpr inline vec4 operator - ( const vec4 &lhs, vec2 rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.x, lhs.w - rhs.y };
}

[[nodiscard]] constexpr inline vec4 operator - ( const vec4 &lhs, f32 rhs )
{
	return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs };
}

[[nodiscard]] constexpr inline vec4 operator - ( const vec4 &lhs, ivec2 rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.x, lhs.w - rhs.y };
}

[[nodiscard]] constexpr inline vec4 operator * ( const vec4 &lhs, const vec4 &rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
}

[[nodiscard]] constexpr inline vec4 operator * ( const vec4 &lhs, const ivec4 &rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
}

[[nodiscard]] constexpr inline vec4 operator * ( const vec4 &lhs, f32 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
}

[[nodiscard]] constexpr inline vec4 operator * ( const vec4 &lhs, vec2 rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.x, lhs.w * rhs.y };
}

[[nodiscard]] constexpr inline vec4 operator * ( const vec4 &lhs, ivec2 rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.x, lhs.w * rhs.y };
}

[[nodiscard]] constexpr inline vec4 operator / ( const vec4 &lhs, const vec4 &rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w };
}

[[nodiscard]] constexpr inline vec4 operator / ( const vec4 &lhs, const ivec4 &rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w };
}

[[nodiscard]] constexpr inline vec4 operator / ( const vec4 &lhs, f32 rhs )
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
}

[[nodiscard]] constexpr inline vec4 operator / ( const vec4 &lhs, vec2 rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.x, lhs.w / rhs.y };
}

[[nodiscard]] constexpr inline vec4 operator / ( const vec4 &lhs, ivec2 rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.x, lhs.w / rhs.y };
}

[[nodiscard]] constexpr inline vec4 operator / ( f32 lhs, const vec4 &rhs )
{
	return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w };
}

inline vec4 &operator += ( vec4 &lhs, const vec4 &rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline vec4 &operator += ( vec4 &lhs, vec2 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline vec4 &operator += ( vec4 &lhs, f32 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline vec4 &operator -= ( vec4 &lhs, const vec4 &rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline vec4 &operator -= ( vec4 &lhs, f32 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline vec4 &operator *= ( vec4 &lhs, const vec4 &rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline vec4 &operator *= ( vec4 &lhs, f32 rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline vec4 &operator /= ( vec4 &lhs, const vec4 &rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline vec4 &operator /= ( vec4 &lhs, f32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

[[nodiscard]] constexpr inline vec4 operator - ( const vec4 &vec )
{
	return { -vec.x, -vec.y, -vec.z, -vec.w };
}

[[nodiscard]] constexpr inline f32 dot( const vec4 &lhs, const vec4 &rhs )
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

[[nodiscard]] inline f32 magnitude( const vec4 &vec )
{
	return sqrtf( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w );
}

[[nodiscard]] inline f32 length( const vec4 &vec )
{
	return sqrtf( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w );
}

[[nodiscard]] constexpr inline f32 length_squared( const vec4 &vec )
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}

[[nodiscard]] inline vec4 normal( const vec4 &vec )
{
	return vec / magnitude( vec );
}

inline void normalise( vec4 &vec )
{
	vec /= magnitude( vec );
}

[[nodiscard]] inline f32 distance( const vec4 &lhs, const vec4 &rhs )
{
	return length( lhs - rhs );
}

[[nodiscard]] inline f32 distance_sq( const vec4 &lhs, const vec4 &rhs )
{
	return length_squared( lhs - rhs );
}

[[nodiscard]] inline vec4 direction( const vec4 &lhs, const vec4 &rhs )
{
	return normal( lhs - rhs );
}

// ivec2 ////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline i32 *ptr( ivec2 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline const i32 *ptr( const ivec2 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline ivec2 to_ivec2( ivec3 vec ) { return { vec.x, vec.y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( ivec4 vec ) { return { vec.x, vec.y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( vec2 vec ) { return { (i32)vec.x, (i32)vec.y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( vec3 vec ) { return { (i32)vec.x, (i32)vec.y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( vec4 vec ) { return { (i32)vec.x, (i32)vec.y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( u16 x, u16 y ) { return { (i32)x, (i32)y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( u32 x, u32 y ) { return { (i32)x, (i32)y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( f32 x, f32 y ) { return { (i32)x, (i32)y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( i32 x, f32 y ) { return { x, (i32)y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( i32 x, i32 y ) { return { x, y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( f32 x, i32 y ) { return { (i32)x, y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( u32 x, f32 y ) { return { (i32)x, (i32)y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( f32 x, u32 y ) { return { (i32)x, (i32)y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( u32 x, i32 y ) { return { (i32)x, y }; }
[[nodiscard]] constexpr inline ivec2 to_ivec2( i32 x, u32 y ) { return { x, (i32)y }; }

[[nodiscard]] constexpr inline bool operator == ( ivec2 lhs, ivec2 rhs )
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

[[nodiscard]] constexpr inline bool operator == ( ivec2 lhs, i32 rhs )
{
	return lhs.x == rhs && lhs.y == rhs;
}

[[nodiscard]] constexpr inline bool operator != ( ivec2 lhs, ivec2 rhs )
{
	return lhs.x != rhs.x || lhs.y != rhs.y;
}

[[nodiscard]] constexpr inline bool operator != ( ivec2 lhs, i32 rhs )
{
	return lhs.x != rhs || lhs.y != rhs;
}

[[nodiscard]] constexpr inline bool operator < ( ivec2 lhs, ivec2 rhs )
{
	return lhs.x < rhs.x && lhs.y < rhs.y;
}

[[nodiscard]] constexpr inline bool operator > ( ivec2 lhs, ivec2 rhs )
{
	return lhs.x > rhs.x && lhs.y > rhs.y;
}

[[nodiscard]] constexpr inline bool operator <= ( ivec2 lhs, ivec2 rhs )
{
	return lhs.x <= rhs.x && lhs.y <= rhs.y;
}

[[nodiscard]] constexpr inline bool operator >= ( ivec2 lhs, ivec2 rhs )
{
	return lhs.x >= rhs.x && lhs.y >= rhs.y;
}

[[nodiscard]] constexpr inline ivec2 operator + ( ivec2 lhs, ivec2 rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

[[nodiscard]] constexpr inline ivec2 operator + ( ivec2 lhs, i32 rhs )
{
	return { lhs.x + rhs, lhs.y + rhs };
}

[[nodiscard]] constexpr inline ivec2 operator + ( i32 lhs, ivec2 rhs )
{
	return { lhs + rhs.x, lhs + rhs.y };
}

[[nodiscard]] constexpr inline ivec4 operator + ( ivec2 lhs, const ivec4 &rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.x + rhs.z, lhs.y + rhs.w };
}

[[nodiscard]] constexpr inline ivec2 operator - ( ivec2 lhs, ivec2 rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

[[nodiscard]] constexpr inline ivec2 operator - ( ivec2 lhs, i32 rhs )
{
	return { lhs.x - rhs, lhs.y - rhs };
}

[[nodiscard]] constexpr inline vec2 operator - ( ivec2 lhs, f32 rhs )
{
	return { lhs.x - rhs, lhs.y - rhs };
}

[[nodiscard]] constexpr inline ivec2 operator * ( ivec2 lhs, ivec2 rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator * ( ivec2 lhs, vec2 rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}

[[nodiscard]] constexpr inline ivec2 operator * ( ivec2 lhs, u8 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs };
}

[[nodiscard]] constexpr inline ivec2 operator * ( ivec2 lhs, u16 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs };
}

[[nodiscard]] constexpr inline ivec2 operator * ( ivec2 lhs, u32 rhs )
{
	return { lhs.x * (i32)rhs, lhs.y * (i32)rhs };
}

[[nodiscard]] constexpr inline ivec2 operator * ( ivec2 lhs, i32 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs };
}

[[nodiscard]] constexpr inline vec2 operator * ( ivec2 lhs, f32 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs };
}

[[nodiscard]] constexpr inline ivec2 operator / ( ivec2 lhs, ivec2 rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

[[nodiscard]] constexpr inline vec2 operator / ( ivec2 lhs, vec2 rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

[[nodiscard]] constexpr inline ivec2 operator / ( ivec2 lhs, i32 rhs )
{
	return { lhs.x / rhs, lhs.y / rhs };
}

[[nodiscard]] constexpr inline vec2 operator / ( ivec2 lhs, f32 rhs )
{
	return { lhs.x / rhs, lhs.y / rhs };
}

[[nodiscard]] constexpr inline ivec2 divide( ivec2 lhs, ivec2 rhs )
{
	return { divide( lhs.x, rhs.x ), divide( lhs.y, rhs.y ) };
}

[[nodiscard]] constexpr inline ivec2 operator % ( ivec2 lhs, i32 rhs )
{
	return { lhs.x % rhs, lhs.y % rhs };
}

inline ivec2 &operator += ( ivec2 &lhs, ivec2 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline ivec2 &operator += ( ivec2 &lhs, i32 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline ivec2 &operator -= ( ivec2 &lhs, ivec2 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline ivec2 &operator -= ( ivec2 &lhs, i32 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline ivec2 &operator *= ( ivec2 &lhs, ivec2 rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline ivec2 &operator *= ( ivec2 &lhs, i32 rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline ivec2 &operator /= ( ivec2 &lhs, ivec2 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline ivec2 &operator /= ( ivec2 &lhs, i32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline ivec2 &operator %= ( ivec2 &lhs, i32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

[[nodiscard]] constexpr inline ivec2 operator - ( ivec2 vec )
{
	return { -vec.x, -vec.y };
}

[[nodiscard]] constexpr inline i32 dot( ivec2 lhs, ivec2 rhs )
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

[[nodiscard]] inline i32 magnitude( const ivec2 &vec )
{
	return (i32)sqrtf( (f32)vec.x * (f32)vec.x + (f32)vec.y * (f32)vec.y );
}

[[nodiscard]] inline i32 length( const ivec2 &vec )
{
	return (i32)sqrtf( (f32)vec.x * (f32)vec.x + (f32)vec.y * (f32)vec.y );
}

[[nodiscard]] constexpr inline i32 length_squared( const ivec2 &vec )
{
	return vec.x * vec.x + vec.y * vec.y;
}

[[nodiscard]] inline ivec2 normal( const ivec2 &vec )
{
	return vec / magnitude( vec );
}

inline void normalise( ivec2 &vec )
{
	vec /= magnitude( vec );
}

[[nodiscard]] inline i32 distance( ivec2 lhs, ivec2 rhs )
{
	return length( lhs - rhs );
}

[[nodiscard]] inline i32 distance_sq( ivec2 lhs, ivec2 rhs )
{
	return length_squared( lhs - rhs );
}

[[nodiscard]] inline ivec2 direction( ivec2 lhs, ivec2 rhs )
{
	return normal( lhs - rhs );
}

// ivec3 ////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline i32 *ptr( ivec3 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline const i32 *ptr( const ivec3 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline ivec3 to_ivec3( ivec2 vec ){ return { vec.x, vec.y, 0 }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( ivec2 vec, i32 z ) { return { vec.x, vec.y, z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( ivec4 vec ) { return { vec.x, vec.y, vec.z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( vec2 vec ) { return { (i32)vec.x, (i32)vec.y, 0 }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( vec3 vec ) { return { (i32)vec.x, (i32)vec.y, (i32)vec.z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( vec4 vec ) { return { (i32)vec.x, (i32)vec.y, (i32)vec.z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( i32 x, i32 y, i32 z ) { return { x, y, z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( i32 x, i32 y, f32 z ) { return { x, y, (i32)z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( i32 x, f32 y, i32 z ) { return { x, (i32)y, z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( i32 x, f32 y, f32 z ) { return { x, (i32)y, (i32)z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( f32 x, i32 y, i32 z ) { return { (i32)x, y, z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( f32 x, i32 y, f32 z ) { return { (i32)x, y, (i32)z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( f32 x, f32 y, i32 z ) { return { (i32)x, (i32)y, z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( u32 x, u32 y, u32 z ) { return { (i32)x, (i32)y, (i32)z }; }
[[nodiscard]] constexpr inline ivec3 to_ivec3( f32 x, f32 y, f32 z ) { return { (i32)x, (i32)y, (i32)z }; }

[[nodiscard]] constexpr inline bool operator == ( const ivec3 &lhs, const ivec3 &rhs )
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

[[nodiscard]] constexpr inline bool operator == ( const ivec3 &lhs, i32 rhs )
{
	return lhs.x == rhs && lhs.y == rhs && lhs.z == rhs;
}

[[nodiscard]] constexpr inline bool operator != ( const ivec3 &lhs, const ivec3 &rhs )
{
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

[[nodiscard]] constexpr inline bool operator != ( const ivec3 &lhs, i32 rhs )
{
	return lhs.x != rhs || lhs.y != rhs || lhs.z != rhs;
}

[[nodiscard]] constexpr inline ivec3 operator + ( const ivec3 &lhs, const ivec3 &rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

[[nodiscard]] constexpr inline ivec3 operator + ( const ivec3 &lhs, i32 rhs )
{
	return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs };
}

[[nodiscard]] constexpr inline ivec3 operator - ( const ivec3 &lhs, const ivec3 &rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

[[nodiscard]] constexpr inline ivec3 operator - ( const ivec3 &lhs, i32 rhs )
{
	return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs };
}

[[nodiscard]] constexpr inline ivec3 operator * ( const ivec3 &lhs, const ivec3 &rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

[[nodiscard]] constexpr inline ivec3 operator * ( const ivec3 &lhs, i32 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

[[nodiscard]] constexpr inline vec3 operator * ( const ivec3 &lhs, f32 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

[[nodiscard]] constexpr inline ivec3 operator / ( const ivec3 &lhs, const ivec3 &rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
}

[[nodiscard]] constexpr inline vec3 operator / ( const ivec3 &lhs, const vec3 &rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
}

[[nodiscard]] constexpr inline ivec3 operator / ( const ivec3 &lhs, i32 rhs )
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
}

[[nodiscard]] constexpr inline ivec3 divide( const ivec3 &lhs, const ivec3 &rhs )
{
	return { divide( lhs.x, rhs.x ), divide( lhs.y, rhs.y ), divide( lhs.z, rhs.z ) };
}

[[nodiscard]] constexpr inline ivec3 operator % ( ivec3 lhs, i32 rhs )
{
	return { lhs.x % rhs, lhs.y % rhs, lhs.z % rhs };
}

inline ivec3 &operator += ( ivec3 &lhs, const ivec3 &rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline ivec3 &operator += ( ivec3 &lhs, i32 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline ivec3 &operator -= ( ivec3 &lhs, const ivec3 &rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline ivec3 &operator -= ( ivec3 &lhs, i32 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline ivec3 &operator *= ( ivec3 &lhs, const ivec3 &rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline ivec3 &operator *= ( ivec3 &lhs, i32 rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline ivec3 &operator /= ( ivec3 &lhs, const ivec3 &rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline ivec3 &operator /= ( ivec3 &lhs, i32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline ivec3 &operator %= ( ivec3 &lhs, i32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

[[nodiscard]] constexpr inline ivec3 operator - ( const ivec3 &vec )
{
	return { -vec.x, -vec.y, -vec.z };
}

[[nodiscard]] constexpr inline i32 dot( const ivec3 &lhs, const ivec3 &rhs )
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

[[nodiscard]] inline i32 magnitude( const ivec3 &vec )
{
	return (i32)sqrtf( (f32)vec.x * (f32)vec.x + (f32)vec.y * (f32)vec.y + (f32)vec.z * (f32)vec.z );
}

[[nodiscard]] inline i32 length( const ivec3 &vec )
{
	return (i32)sqrtf( (f32)vec.x * (f32)vec.x + (f32)vec.y * (f32)vec.y + (f32)vec.z * (f32)vec.z );
}

[[nodiscard]] constexpr inline i32 length_squared( const ivec3 &vec )
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

[[nodiscard]] inline ivec3 normal( const ivec3 &vec )
{
	return vec / magnitude( vec );
}

inline void normalise( ivec3 &vec )
{
	vec /= magnitude( vec );
}

[[nodiscard]] inline i32 distance( const ivec3 &lhs, const ivec3 &rhs )
{
	return length( lhs - rhs );
}

[[nodiscard]] inline i32 distance_sq( const ivec3 &lhs, const ivec3 &rhs )
{
	return length_squared( lhs - rhs );
}

[[nodiscard]] inline ivec3 direction( const ivec3 &lhs, const ivec3 &rhs )
{
	return normal( lhs - rhs );
}

// ivec4 ////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline i32 *ptr( ivec4 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline const i32 *ptr( const ivec4 &vec )
{
	return &vec.x;
}

[[nodiscard]] constexpr inline ivec4 to_ivec4( ivec2 vec ) { return { vec.x, vec.y, 0 , 0 }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( ivec3 vec ) { return { vec.x, vec.y, vec.z, 0 }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( vec2 vec ) { return { (i32)vec.x, (i32)vec.y, 0, 0 }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( vec3 vec ) { return { (i32)vec.x, (i32)vec.y, (i32)vec.z, 0 }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( vec4 vec ) { return { (i32)vec.x, (i32)vec.y, (i32)vec.z, (i32)vec.w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( u32 x, u32 y, u32 z, u32 w ) { return { (i32)x, (i32)y, (i32)z, (i32)w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( f32 x, f32 y, f32 z, u32 w ) { return { (i32)x, (i32)y, (i32)z, (i32)w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( i32 x, i32 y, i32 z, u32 w ) { return { x, y, z, (i32)w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( i32 x, i32 y, i32 z, i32 w ) { return { x, y, z, w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( i32 x, f32 y, i32 z, i32 w ) { return { x, (i32)y, z, w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( f32 x, i32 y, i32 z, u32 w ) { return { (i32)x, y, z, (i32)w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( f32 x, f32 y, i32 z, u32 w ) { return { (i32)x, (i32)y, z, (i32)w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( f32 x, f32 y, f32 z, i32 w ) { return { (i32)x, (i32)y, (i32)z, w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( f32 x, f32 y, f32 z, f32 w ) { return { (i32)x, (i32)y, (i32)z, (i32)w }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( i32 x, i32 y, ivec2 vec1 ) { return { x, y, vec1.x, vec1.y }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( ivec2 vec0, vec2 vec1 ) { return { vec0.x, vec0.y, (i32)vec1.x, (i32)vec1.y }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( ivec2 vec0, ivec2 vec1 ) { return { vec0.x, vec0.y, vec1.x, vec1.y }; }
[[nodiscard]] constexpr inline ivec4 to_ivec4( ivec3 vec0, i32 w ) { return { vec0.x, vec0.y, vec0.z, w }; }

[[nodiscard]] constexpr inline bool operator == ( const ivec4 &lhs, const ivec4 &rhs )
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

[[nodiscard]] constexpr inline bool operator == ( const ivec4 &lhs, i32 rhs )
{
	return lhs.x == rhs && lhs.y == rhs && lhs.z == rhs && lhs.w == rhs;
}

[[nodiscard]] constexpr inline bool operator != ( const ivec4 &lhs, const ivec4 &rhs )
{
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
}

[[nodiscard]] constexpr inline bool operator != ( const ivec4 &lhs, i32 rhs )
{
	return lhs.x != rhs || lhs.y != rhs || lhs.z != rhs || lhs.w != rhs;
}

[[nodiscard]] constexpr inline ivec4 operator + ( const ivec4 &lhs, const ivec4 &rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

[[nodiscard]] constexpr inline ivec4 operator + ( const ivec4 &lhs, ivec2 rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.x, lhs.w + rhs.y };
}

[[nodiscard]] constexpr inline ivec4 operator + ( const ivec4 &lhs, i32 rhs )
{
	return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs };
}

[[nodiscard]] constexpr inline ivec4 operator - ( const ivec4 &lhs, const ivec4 &rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

[[nodiscard]] constexpr inline ivec4 operator - ( const ivec4 &lhs, ivec2 rhs )
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.x, lhs.w - rhs.y };
}

[[nodiscard]] constexpr inline ivec4 operator - ( const ivec4 &lhs, i32 rhs )
{
	return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs };
}

[[nodiscard]] constexpr inline ivec4 operator * ( const ivec4 &lhs, const ivec4 &rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
}

[[nodiscard]] constexpr inline ivec4 operator * ( const ivec4 &lhs, i32 rhs )
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
}

[[nodiscard]] constexpr inline ivec4 operator * ( const ivec4 &lhs, ivec2 rhs )
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.x, lhs.w * rhs.y };
}

[[nodiscard]] constexpr inline ivec4 operator / ( const ivec4 &lhs, const ivec4 &rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w };
}

[[nodiscard]] constexpr inline vec4 operator / ( const ivec4 &lhs, const vec4 &rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w };
}

[[nodiscard]] constexpr inline vec4 operator / ( const ivec4 &lhs, vec2 rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.x, lhs.w / rhs.y };
}

[[nodiscard]] constexpr inline ivec4 operator / ( const ivec4 &lhs, i32 rhs )
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
}

[[nodiscard]] constexpr inline ivec4 operator / ( const ivec4 &lhs, ivec2 rhs )
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.x, lhs.w / rhs.y };
}

[[nodiscard]] constexpr inline ivec4 divide( const ivec4 &lhs, ivec2 rhs )
{
	return { divide( lhs.x, rhs.x ), divide( lhs.y, rhs.y ), divide( lhs.z, rhs.x ), divide( lhs.w, rhs.y ) };
}

[[nodiscard]] constexpr inline ivec4 divide( const ivec4 &lhs, const ivec4 &rhs )
{
	return { divide( lhs.x, rhs.x ), divide( lhs.y, rhs.y ), divide( lhs.z, rhs.z ), divide( lhs.w, rhs.w ) };
}

[[nodiscard]] constexpr inline ivec4 operator % ( ivec4 lhs, i32 rhs )
{
	return { lhs.x % rhs, lhs.y % rhs, lhs.z % rhs, lhs.w % rhs };
}

inline ivec4 &operator += ( ivec4 &lhs, const ivec4 &rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline ivec4 &operator += ( ivec4 &lhs, ivec2 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline ivec4 &operator += ( ivec4 &lhs, i32 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline ivec4 &operator -= ( ivec4 &lhs, const ivec4 &rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline ivec4 &operator -= ( ivec4 &lhs, ivec2 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline ivec4 &operator -= ( ivec4 &lhs, i32 rhs )
{
	lhs = lhs - rhs;
	return lhs;
}

inline ivec4 &operator *= ( ivec4 &lhs, const ivec4 &rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline ivec4 &operator *= ( ivec4 &lhs, i32 rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

inline ivec4 &operator /= ( ivec4 &lhs, const ivec4 &rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline ivec4 &operator /= ( ivec4 &lhs, i32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline ivec4 &operator /= ( ivec4 &lhs, ivec2 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

inline ivec4 &operator %= ( ivec4 &lhs, i32 rhs )
{
	lhs = lhs / rhs;
	return lhs;
}

[[nodiscard]] constexpr inline ivec4 operator - ( const ivec4 &vec )
{
	return { -vec.x, -vec.y, -vec.z, -vec.w };
}

[[nodiscard]] constexpr inline i32 dot( const ivec4 &lhs, const ivec4 &rhs )
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

[[nodiscard]] inline i32 magnitude( const ivec4 &vec )
{
	return (i32)sqrtf( (f32)vec.x * (f32)vec.x + (f32)vec.y * (f32)vec.y + (f32)vec.z * (f32)vec.z + (f32)vec.w * (f32)vec.w );
}

[[nodiscard]] inline i32 length( const ivec4 &vec )
{
	return (i32)sqrtf( (f32)vec.x * (f32)vec.x + (f32)vec.y * (f32)vec.y + (f32)vec.z * (f32)vec.z + (f32)vec.w * (f32)vec.w );
}

[[nodiscard]] constexpr inline i32 length_squared( const ivec4 &vec )
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}

[[nodiscard]] inline ivec4 normal( const ivec4 &vec )
{
	return vec / magnitude( vec );
}

inline void normalise( ivec4 &vec )
{
	vec /= magnitude( vec );
}

[[nodiscard]] inline i32 distance( const ivec4 &lhs, const ivec4 &rhs )
{
	return length( lhs - rhs );
}

[[nodiscard]] inline i32 distance_sq( const ivec4 &lhs, const ivec4 &rhs )
{
	return length_squared( lhs - rhs );
}

[[nodiscard]] inline ivec4 direction( const ivec4 &lhs, const ivec4 &rhs )
{
	return normal( lhs - rhs );
}

// Quaternion ///////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline f32 *ptr( Quaternion &quat )
{
	return &quat.x;
}

[[nodiscard]] constexpr inline const f32 *ptr( const Quaternion &quat )
{
	return &quat.x;
}

[[nodiscard]] Quaternion operator * ( const Quaternion &lhs, const Quaternion &rhs );

inline Quaternion &operator *= ( Quaternion &lhs, const Quaternion &rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

constexpr inline void identity( Quaternion &quat )
{
	quat.x = quat.y = quat.z = 0.0f;
	quat.w = 1.0f;
}

[[nodiscard]] constexpr inline Quaternion quaternion_identity()
{
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

[[nodiscard]] constexpr inline f32 dot( const Quaternion &lhs, const Quaternion &rhs )
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

[[nodiscard]] inline f32 magnitude( const Quaternion &quat )
{
	return sqrtf( quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w );
}

[[nodiscard]] inline f32 length( const Quaternion &quat )
{
	return sqrtf( quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w );
}

[[nodiscard]] constexpr inline f32 length_squared( const Quaternion &quat )
{
	return quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
}

void normalise( Quaternion &quat );

[[nodiscard]] inline Quaternion normal( const Quaternion &quat )
{
	f32 m = magnitude( quat );
	return { quat.x / m, quat.y / m, quat.z / m, quat.w / m };
}

[[nodiscard]] constexpr inline Quaternion conjugated( const Quaternion &quat )
{
	return { -quat.x, -quat.y, -quat.z, quat.w };
}

inline void conjugate( Quaternion &quat )
{
	quat.x = -quat.x;
	quat.y = -quat.y;
	quat.z = -quat.z;
}

[[nodiscard]] inline Quaternion inversed( const Quaternion &quat )
{
	return normal( conjugated( quat ) );
}

inline void inverse( Quaternion &quat )
{
	conjugate( quat );
	normalise( quat );
}

[[nodiscard]] Quaternion quaternion_axis_angle( const vec3 &axis, f32 angle );

void slerp( Quaternion &quat, Quaternion other, f32 percentage );

[[nodiscard]] inline Quaternion slerped( Quaternion quat, const Quaternion &other, f32 percentage )
{
	slerp( quat, other, percentage );
	return quat;
}

// mat2 /////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline f32 *ptr( mat2 &mat )
{
	return mat.data;
}

[[nodiscard]] constexpr inline const f32 *ptr( const mat2 &mat )
{
	return mat.data;
}

#ifdef MATH_COLUMN_MAJOR

[[nodiscard]] vec2 operator * ( const mat2 &lhs, vec2 rhs );

#else // !MATH_COLUMN_MAJOR

[[nodiscard]] vec2 operator * ( const vec2 &lhs, const mat2 &rhs );

#endif // !MATH_COLUMN_MAJOR

inline void identity( mat2 &mat )
{
	f32 *data = ptr( mat );

	data[ 0 ] = 1.0f;
	data[ 1 ] = 0.0f;
	data[ 2 ] = 0.0f;
	data[ 3 ] = 1.0f;
}

[[nodiscard]] constexpr inline mat2 mat2_identity()
{
	return { 1.0f, 0.0f, 0.0f, 1.0f };
}

// mat3 /////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline f32 *ptr( mat3 &mat )
{
	return mat.data;
}

[[nodiscard]] constexpr inline const f32 *ptr( const mat3 &mat )
{
	return mat.data;
}

#ifdef MATH_COLUMN_MAJOR

[[nodiscard]] vec3 operator * ( const mat3 &lhs, const vec3 &rhs );

#else // !MATH_COLUMN_MAJOR

[[nodiscard]] vec3 operator * ( const vec3 &lhs, const mat3 &rhs );

#endif // !MATH_COLUMN_MAJOR

void identity( mat3 &mat );

[[nodiscard]] constexpr inline mat3 mat3_identity()
{
	return { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
}

// mat4 /////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr inline f32 *ptr( mat4 &mat )
{
	return mat.data;
}

[[nodiscard]] constexpr inline const f32 *ptr( const mat4 &mat )
{
	return mat.data;
}

#ifdef MATH_COLUMN_MAJOR

[[nodiscard]] vec4 operator * ( const mat4 &lhs, const vec4 &rhs );

[[nodiscard]] mat4 operator * ( const mat4 &lhs, const mat4 &rhs );

#else // !MATH_COLUMN_MAJOR

[[nodiscard]] vec4 operator * ( const vec4 &lhs, const mat4 &rhs );

[[nodiscard]] mat4 operator * ( const mat4 &lhs, const mat4 &rhs );

#endif // !MATH_COLUMN_MAJOR

inline mat4 &operator *= ( mat4 &lhs, const mat4 &rhs )
{
	lhs = lhs * rhs;
	return lhs;
}

void identity( mat4 &mat );

[[nodiscard]] constexpr inline mat4 mat4_identity()
{
	return { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
}

void transpose( mat4 &mat );

void inverse( mat4 &mat );

void mat4_set_orthographic( mat4 &mat, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far );

void mat4_set_lookat( mat4 &mat, const vec3 &position, const vec3 &target, const vec3 &up );

void mat4_set_as_translation( mat4 &mat, const vec3 &position );

void mat4_set_as_scale( mat4 &mat, const vec3 &scale );

void mat4_set_as_euler_x( mat4 &mat, f32 angleRadian );

void mat4_set_as_euler_y( mat4 &mat, f32 angleRadian );

void mat4_set_as_euler_z( mat4 &mat, f32 angleRadian );

void mat4_set_as_euler_xyz( mat4 &mat, vec3 vec );

void mat4_set_as_euler_xyz( mat4 &mat, f32 xRadian, f32 yRadian, f32 zRadian );

void mat4_set_as_quaternion( mat4 &mat, const Quaternion &q );

void mat4_set_as_quaternion( mat4 &mat, const Quaternion &q, const vec3 &centre );

[[nodiscard]] constexpr inline vec3 mat4_left( const mat4 &mat )
{
	return normal( vec3{ -mat.data[ 0 ], -mat.data[ 4 ], -mat.data[ 8 ] } );
}

[[nodiscard]] constexpr inline vec3 mat4_right( const mat4 &mat )
{
	return normal( vec3{ mat.data[ 0 ], mat.data[ 4 ], mat.data[ 8 ] } );
}

[[nodiscard]] constexpr inline vec3 mat4_forward( const mat4 &mat )
{
	return normal( vec3{ -mat.data[ 2 ], -mat.data[ 6 ], -mat.data[ 10 ] } );
}

[[nodiscard]] constexpr inline vec3 mat4_backward( const mat4 &mat )
{
	return normal( vec3{ mat.data[ 2 ], mat.data[ 6 ], mat.data[ 10 ] });
}

[[nodiscard]] constexpr inline vec3 mat4_up( const mat4 &mat )
{
	return normal( vec3{ mat.data[ 1 ], mat.data[ 5 ], mat.data[ 9 ] } );
}

[[nodiscard]] constexpr inline vec3 mat4_down( const mat4 &mat )
{
	return normal( vec3{ -mat.data[ 1 ], -mat.data[ 5 ], -mat.data[ 9 ] } );
}

// Cube /////////////////////////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Cube operator + ( const Cube &lhs, ivec2 rhs )
{
	return
	{
		lhs.l + rhs.x,
		lhs.t + rhs.y,
		lhs.r + rhs.x,
		lhs.b + rhs.y,
		lhs.zB,
		lhs.zT,
	};
}

[[nodiscard]] constexpr Cube operator + ( const Cube &lhs, ivec3 rhs )
{
	return
	{
		lhs.l + rhs.x,
		lhs.t + rhs.y,
		lhs.r + rhs.x,
		lhs.b + rhs.y,
		lhs.zB + rhs.z,
		lhs.zT + rhs.z,
	};
}

inline Cube &operator += ( Cube &lhs, ivec2 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

inline Cube &operator += ( Cube &lhs, ivec3 rhs )
{
	lhs = lhs + rhs;
	return lhs;
}

[[nodiscard]] constexpr inline ivec3 cube_centre_offset( const Cube &cube )
{
	return -ivec3{ cube.r - cube.l, cube.b - cube.t, cube.zT - cube.zB } / 2;
}

[[nodiscard]] constexpr inline ivec3 cube_centre( const Cube &cube )
{
	return ivec3{ cube.l, cube.t, cube.zB } + cube_centre_offset( cube );
}

#endif // _HG_MATH_FUNCTIONS

// --------------------------------------------------------------------------------------

#if defined( MATH_FUNCTIONS_IMPLEMENTATION )

// utility //////////////////////////////////////////////////////////////////////////////
void calculate_view( vec3 position, vec3 rotation, mat4 *view )
{
	mat4 translation, rot;

	mat4_set_as_euler_xyz( rot, rotation );
	mat4_set_as_translation( translation, position );

	#ifdef MATH_COLUMN_MAJOR
		inverse( *view = ( translation * rot ) );		// T*R*S
	#else
		inverse( *view = ( rot * translation ) );		// S*R*T
	#endif
}

inline void calculate_view( ivec3 position, ivec3 rotation, mat4 *view )
{
	calculate_view( to_vec3( position ), to_vec3( rotation ), view );
}

// vec2 /////////////////////////////////////////////////////////////////////////////////
// vec3 /////////////////////////////////////////////////////////////////////////////////
// vec4 /////////////////////////////////////////////////////////////////////////////////
// ivec2 ////////////////////////////////////////////////////////////////////////////////
// ivec3 ////////////////////////////////////////////////////////////////////////////////
// ivec4 ////////////////////////////////////////////////////////////////////////////////

// Quaternion ///////////////////////////////////////////////////////////////////////////
[[nodiscard]] Quaternion operator * ( const Quaternion &lhs, const Quaternion &rhs )
{
	return
	{
		.x = lhs.x * rhs.w +
			lhs.y * rhs.z -
			lhs.z * rhs.y +
			lhs.w * rhs.x,

		.y = -lhs.x * rhs.z +
			lhs.y * rhs.w +
			lhs.z * rhs.x +
			lhs.w * rhs.y,

		.z = lhs.x * rhs.y -
			lhs.y * rhs.x +
			lhs.z * rhs.w +
			lhs.w * rhs.z,

		.w = -lhs.x * rhs.x -
			lhs.y * rhs.y -
			lhs.z * rhs.z +
			lhs.w * rhs.w,
	};
}

void normalise( Quaternion &quat )
{
	f32 m = magnitude( quat );
	quat.x /= m;
	quat.y /= m;
	quat.z /= m;
	quat.w /= m;
}

[[nodiscard]] Quaternion quaternion_axis_angle( const vec3 &axis, f32 angle )
{
	f32 halfAngle = 0.5f * angle;
	f32 s = sinf( halfAngle );
	f32 c = cosf( halfAngle );
	return { s * axis.x, s * axis.y, s * axis.z, c };
}

void slerp( Quaternion &quat, Quaternion other, f32 percentage )
{
	normalise( quat );
	normalise( other );

	f32 dt = dot( quat, other );

	if ( dt < 0.0f )
	{
		other.x = -other.x;
		other.y = -other.y;
		other.z = -other.z;
		other.w = -other.w;
		dt = -dt;
	}

	constexpr f32 threshhold = 0.9995f;

	if ( dt > threshhold )
	{
		quat.x = ( ( other.x - quat.x ) * percentage );
		quat.y = ( ( other.y - quat.y ) * percentage );
		quat.z = ( ( other.z - quat.z ) * percentage );
		quat.w = ( ( other.w - quat.w ) * percentage );

		normalise( quat );

		return;
	}

	f32 t = acosf( dt );
	f32 theta = t * percentage;
	f32 sinTheta = sinf( theta );
	f32 sinTheta0 = sinf( t );

	f32 s0 = cosf( theta ) - dt * sinTheta / sinTheta0;
	f32 s1 = sinTheta / sinTheta0;

	quat.x = ( quat.x * s0 ) + ( other.x * s1 );
	quat.y = ( quat.y * s0 ) + ( other.y * s1 );
	quat.z = ( quat.z * s0 ) + ( other.z * s1 );
	quat.w = ( quat.w * s0 ) + ( other.w * s1 );
}

// mat2 /////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_COLUMN_MAJOR

[[nodiscard]] vec2 operator * ( const mat2 &lhs, vec2 rhs )
{
	vec2 out;
	const f32 *m = ptr( lhs );

	out.x = ( m[ 0 ] * rhs.x ) + ( m[ 2 ] * rhs.y );
	out.y = ( m[ 1 ] * rhs.x ) + ( m[ 3 ] * rhs.y );

	return out;
}

#else // !MATH_COLUMN_MAJOR

[[nodiscard]] vec2 operator * ( const vec2 &lhs, const mat2 &rhs )
{
	vec2 out;
	const f32 *m = ptr( rhs );

	out.x = ( m[ 0 ] * lhs.x ) + ( m[ 1 ] * lhs.y );
	out.y = ( m[ 2 ] * lhs.x ) + ( m[ 3 ] * lhs.y );

	return out;
}

#endif // !MATH_COLUMN_MAJOR

// mat3 /////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_COLUMN_MAJOR

[[nodiscard]] vec3 operator * ( const mat3 &lhs, const vec3 &rhs )
{
	vec3 out;
	const f32 *m = ptr( lhs );

	out.x = ( m[ 0 ] * rhs.x ) + ( m[ 3 ] * rhs.y ) + ( m[ 6 ] * rhs.z );
	out.y = ( m[ 1 ] * rhs.x ) + ( m[ 4 ] * rhs.y ) + ( m[ 7 ] * rhs.z );
	out.z = ( m[ 2 ] * rhs.x ) + ( m[ 5 ] * rhs.y ) + ( m[ 8 ] * rhs.z );

	return out;
}

#else // !MATH_COLUMN_MAJOR

[[nodiscard]] vec3 operator * ( const vec3 &lhs, const mat3 &rhs )
{
	vec3 out;
	const f32 *m = ptr( rhs );

	out.x = ( m[ 0 ] * lhs.x ) + ( m[ 1 ] * lhs.y ) + ( m[ 2 ] * lhs.z );
	out.y = ( m[ 3 ] * lhs.x ) + ( m[ 4 ] * lhs.y ) + ( m[ 5 ] * lhs.z );
	out.z = ( m[ 6 ] * lhs.x ) + ( m[ 7 ] * lhs.y ) + ( m[ 8 ] * lhs.z );

	return out;
}

#endif // !MATH_COLUMN_MAJOR

void identity( mat3 &mat )
{
	f32 *data = ptr( mat );

	data[ 0 ] = 1.0f;
	data[ 1 ] = 0.0f;
	data[ 2 ] = 0.0f;
	data[ 3 ] = 0.0f;
	data[ 4 ] = 1.0f;
	data[ 5 ] = 0.0f;
	data[ 6 ] = 0.0f;
	data[ 7 ] = 0.0f;
	data[ 8 ] = 1.0f;
}

// mat4 /////////////////////////////////////////////////////////////////////////////////
#ifdef MATH_COLUMN_MAJOR

[[nodiscard]] vec4 operator * ( const mat4 &lhs, const vec4 &rhs )
{
	vec4 out;
	const f32 *m = ptr( lhs );

	out.x = ( m[ 0 ] * rhs.x ) + ( m[ 4 ] * rhs.y ) + ( m[  8 ] * rhs.z ) + ( m[ 12 ] * rhs.w );
	out.y = ( m[ 1 ] * rhs.x ) + ( m[ 5 ] * rhs.y ) + ( m[  9 ] * rhs.z ) + ( m[ 13 ] * rhs.w );
	out.z = ( m[ 2 ] * rhs.x ) + ( m[ 6 ] * rhs.y ) + ( m[ 10 ] * rhs.z ) + ( m[ 14 ] * rhs.w );
	out.w = ( m[ 3 ] * rhs.x ) + ( m[ 7 ] * rhs.y ) + ( m[ 11 ] * rhs.z ) + ( m[ 15 ] * rhs.w );

	return out;
}

[[nodiscard]] mat4 operator * ( const mat4 &lhs, const mat4 &rhs )
{
	mat4 out;
	f32 *C = ptr( out );
	const f32 *A = ptr( lhs );
	const f32 *B = ptr( rhs );

	// C[  0 ] = A[ 0 ] * B[  0 ] + A[ 4 ] * B[  1 ] + A[  8 ] * B[  2 ] + A[ 12 ] * B[  3 ];
	// C[  1 ] = A[ 1 ] * B[  0 ] + A[ 5 ] * B[  1 ] + A[  9 ] * B[  2 ] + A[ 13 ] * B[  3 ];
	// C[  2 ] = A[ 2 ] * B[  0 ] + A[ 6 ] * B[  1 ] + A[ 10 ] * B[  2 ] + A[ 14 ] * B[  3 ];
	// C[  3 ] = A[ 3 ] * B[  0 ] + A[ 7 ] * B[  1 ] + A[ 11 ] * B[  2 ] + A[ 15 ] * B[  3 ];
	// C[  4 ] = A[ 0 ] * B[  4 ] + A[ 4 ] * B[  5 ] + A[  8 ] * B[  6 ] + A[ 12 ] * B[  7 ];
	// C[  5 ] = A[ 1 ] * B[  4 ] + A[ 5 ] * B[  5 ] + A[  9 ] * B[  6 ] + A[ 13 ] * B[  7 ];
	// C[  6 ] = A[ 2 ] * B[  4 ] + A[ 6 ] * B[  5 ] + A[ 10 ] * B[  6 ] + A[ 14 ] * B[  7 ];
	// C[  7 ] = A[ 3 ] * B[  4 ] + A[ 7 ] * B[  5 ] + A[ 11 ] * B[  6 ] + A[ 15 ] * B[  7 ];
	// C[  8 ] = A[ 0 ] * B[  8 ] + A[ 4 ] * B[  9 ] + A[  8 ] * B[ 10 ] + A[ 12 ] * B[ 11 ];
	// C[  9 ] = A[ 1 ] * B[  8 ] + A[ 5 ] * B[  9 ] + A[  9 ] * B[ 10 ] + A[ 13 ] * B[ 11 ];
	// C[ 10 ] = A[ 2 ] * B[  8 ] + A[ 6 ] * B[  9 ] + A[ 10 ] * B[ 10 ] + A[ 14 ] * B[ 11 ];
	// C[ 11 ] = A[ 3 ] * B[  8 ] + A[ 7 ] * B[  9 ] + A[ 11 ] * B[ 10 ] + A[ 15 ] * B[ 11 ];
	// C[ 12 ] = A[ 0 ] * B[ 12 ] + A[ 4 ] * B[ 13 ] + A[  8 ] * B[ 14 ] + A[ 12 ] * B[ 15 ];
	// C[ 13 ] = A[ 1 ] * B[ 12 ] + A[ 5 ] * B[ 13 ] + A[  9 ] * B[ 14 ] + A[ 13 ] * B[ 15 ];
	// C[ 14 ] = A[ 2 ] * B[ 12 ] + A[ 6 ] * B[ 13 ] + A[ 10 ] * B[ 14 ] + A[ 14 ] * B[ 15 ];
	// C[ 15 ] = A[ 3 ] * B[ 12 ] + A[ 7 ] * B[ 13 ] + A[ 11 ] * B[ 14 ] + A[ 15 ] * B[ 15 ];

#ifdef MATH_SIMD

	__m128 col1 = _mm_load_ps( &A[  0 ] );
	__m128 col2 = _mm_load_ps( &A[  4 ] );
	__m128 col3 = _mm_load_ps( &A[  8 ] );
	__m128 col4 = _mm_load_ps( &A[ 12 ] );

	for ( i32 i = 0; i < 4; i++ )
	{
		__m128 brod1 = _mm_set1_ps( B[ 4 * i + 0 ] );
		__m128 brod2 = _mm_set1_ps( B[ 4 * i + 1 ] );
		__m128 brod3 = _mm_set1_ps( B[ 4 * i + 2 ] );
		__m128 brod4 = _mm_set1_ps( B[ 4 * i + 3 ] );

		__m128 row = _mm_add_ps
			(
				_mm_add_ps( _mm_mul_ps( brod1, col1 ), _mm_mul_ps( brod2, col2 ) ),
				_mm_add_ps( _mm_mul_ps( brod3, col3 ), _mm_mul_ps( brod4, col4 ) )
			);

		_mm_store_ps( &C[ 4 * i ], row );
	}

#else // !MATH_SIMD

	for ( i32 i = 0; i < 4; ++i )
	{
		for ( i32 j = 0; j < 4; ++j )
		{
			*C++ =	A[  0 + j ] * B[ 0 ] +
					A[  4 + j ] * B[ 1 ] +
					A[  8 + j ] * B[ 2 ] +
					A[ 12 + j ] * B[ 3 ];
		}
		B += 4;
	}

#endif // !MATH_SIMD

	return out;
}

#else // !MATH_COLUMN_MAJOR

[[nodiscard]] vec4 operator * ( const vec4 &lhs, const mat4 &rhs )
{
	vec4 out;
	const f32 *m = ptr( rhs );

	out.x = ( m[  0 ] * lhs.x ) + ( m[  1 ] * lhs.y ) + ( m[  2 ] * lhs.z ) + ( m[  3 ] * lhs.w );
	out.y = ( m[  4 ] * lhs.x ) + ( m[  5 ] * lhs.y ) + ( m[  6 ] * lhs.z ) + ( m[  7 ] * lhs.w );
	out.z = ( m[  8 ] * lhs.x ) + ( m[  9 ] * lhs.y ) + ( m[ 10 ] * lhs.z ) + ( m[ 11 ] * lhs.w );
	out.w = ( m[ 12 ] * lhs.x ) + ( m[ 13 ] * lhs.y ) + ( m[ 14 ] * lhs.z ) + ( m[ 15 ] * lhs.w );

	return out;
}

[[nodiscard]] mat4 operator * ( const mat4 &lhs, const mat4 &rhs )
{
	mat4 out;
	f32 *C = ptr( out );
	const f32 *A = ptr( lhs );
	const f32 *B = ptr( rhs );

#ifdef MATH_SIMD

	__m128 row1 = _mm_load_ps( &B[  0 ] );
	__m128 row2 = _mm_load_ps( &B[  4 ] );
	__m128 row3 = _mm_load_ps( &B[  8 ] );
	__m128 row4 = _mm_load_ps( &B[ 12 ] );

	for ( i32 i = 0; i < 4; i++ )
	{
		__m128 brod1 = _mm_set1_ps( A[ 4 * i + 0 ] );
		__m128 brod2 = _mm_set1_ps( A[ 4 * i + 1 ] );
		__m128 brod3 = _mm_set1_ps( A[ 4 * i + 2 ] );
		__m128 brod4 = _mm_set1_ps( A[ 4 * i + 3 ] );

		__m128 row = _mm_add_ps
			(
				_mm_add_ps( _mm_mul_ps( brod1, row1 ), _mm_mul_ps( brod2, row2 ) ),
				_mm_add_ps( _mm_mul_ps( brod3, row3 ), _mm_mul_ps( brod4, row4 ) )
			);

		_mm_store_ps( &C[ 4 * i ], row );
	}

#else // !MATH_SIMD

	for ( i32 i = 0; i < 4; ++i )
	{
		for ( i32 j = 0; j < 4; ++j )
		{
			*C++ =	A[ 0 ] * B[  0 + j ] +
					A[ 1 ] * B[  4 + j ] +
					A[ 2 ] * B[  8 + j ] +
					A[ 3 ] * B[ 12 + j ];
		}
		A += 4;
	}

#endif // !MATH_SIMD

	return out;
}

#endif // !MATH_COLUMN_MAJOR

void identity( mat4 &mat )
{
	f32 *data = ptr( mat );

	data[  0 ] = 1.0f;
	data[  1 ] = 0.0f;
	data[  2 ] = 0.0f;
	data[  3 ] = 0.0f;
	data[  4 ] = 0.0f;
	data[  5 ] = 1.0f;
	data[  6 ] = 0.0f;
	data[  7 ] = 0.0f;
	data[  8 ] = 0.0f;
	data[  9 ] = 0.0f;
	data[ 10 ] = 1.0f;
	data[ 11 ] = 0.0f;
	data[ 12 ] = 0.0f;
	data[ 13 ] = 0.0f;
	data[ 14 ] = 0.0f;
	data[ 15 ] = 1.0f;
}

void transpose( mat4 &mat )
{
	mat4 temp = mat;
	f32 *data = ptr( mat );
	const f32 *tmp = ptr( temp );

	//data[  0 ] = tmp[  0 ];
	data[  1 ] = tmp[  4 ];
	data[  2 ] = tmp[  8 ];
	data[  3 ] = tmp[ 12 ];
	data[  4 ] = tmp[  1 ];
	//data[  5 ] = tmp[  5 ];
	data[  6 ] = tmp[  9 ];
	data[  7 ] = tmp[ 13 ];
	data[  8 ] = tmp[  2 ];
	data[  9 ] = tmp[  6 ];
	//data[ 10 ] = tmp[ 10 ];
	data[ 11 ] = tmp[ 14 ];
	data[ 12 ] = tmp[  3 ];
	data[ 13 ] = tmp[  7 ];
	data[ 14 ] = tmp[ 11 ];
	//data[ 15 ] = tmp[ 15 ];
}

void inverse( mat4 &mat )
{
	mat4 temp = mat;
	const f32 *tmp = ptr( temp );
	f32 *data = ptr( mat );

	f32  t0 = tmp[ 10 ] * tmp[ 15 ];
	f32  t1 = tmp[ 14 ] * tmp[ 11 ];
	f32  t2 = tmp[  6 ] * tmp[ 15 ];
	f32  t3 = tmp[ 14 ] * tmp[  7 ];
	f32  t4 = tmp[  6 ] * tmp[ 11 ];
	f32  t5 = tmp[ 10 ] * tmp[  7 ];
	f32  t6 = tmp[  2 ] * tmp[ 15 ];
	f32  t7 = tmp[ 14 ] * tmp[  3 ];
	f32  t8 = tmp[  2 ] * tmp[ 11 ];
	f32  t9 = tmp[ 10 ] * tmp[  3 ];
	f32 t10 = tmp[  2 ] * tmp[  7 ];
	f32 t11 = tmp[  6 ] * tmp[  3 ];
	f32 t12 = tmp[  8 ] * tmp[ 13 ];
	f32 t13 = tmp[ 12 ] * tmp[  9 ];
	f32 t14 = tmp[  4 ] * tmp[ 13 ];
	f32 t15 = tmp[ 12 ] * tmp[  5 ];
	f32 t16 = tmp[  4 ] * tmp[  9 ];
	f32 t17 = tmp[  8 ] * tmp[  5 ];
	f32 t18 = tmp[  0 ] * tmp[ 13 ];
	f32 t19 = tmp[ 12 ] * tmp[  1 ];
	f32 t20 = tmp[  0 ] * tmp[  9 ];
	f32 t21 = tmp[  8 ] * tmp[  1 ];
	f32 t22 = tmp[  0 ] * tmp[  5 ];
	f32 t23 = tmp[  4 ] * tmp[  1 ];

	data[ 0 ] = ( t0 * tmp[ 5 ] + t3 * tmp[ 9 ] +  t4 * tmp[ 13 ] ) - ( t1 * tmp[ 5 ] + t2 * tmp[ 9 ] +  t5 * tmp[ 13 ] );
	data[ 1 ] = ( t1 * tmp[ 1 ] + t6 * tmp[ 9 ] +  t9 * tmp[ 13 ] ) - ( t0 * tmp[ 1 ] + t7 * tmp[ 9 ] +  t8 * tmp[ 13 ] );
	data[ 2 ] = ( t2 * tmp[ 1 ] + t7 * tmp[ 5 ] + t10 * tmp[ 13 ] ) - ( t3 * tmp[ 1 ] + t6 * tmp[ 5 ] + t11 * tmp[ 13 ] );
	data[ 3 ] = ( t5 * tmp[ 1 ] + t8 * tmp[ 5 ] + t11 * tmp[  9 ] ) - ( t4 * tmp[ 1 ] + t9 * tmp[ 5 ] + t10 * tmp[  9 ] );

	f32 d = 1.0f / ( tmp[ 0 ] * data[ 0 ] + tmp[ 4 ] * data[ 1 ] + tmp[ 8 ] * data[ 2 ] + tmp[ 12 ] * data[ 3 ] );

	data[  0 ] = d * data[ 0 ];
	data[  1 ] = d * data[ 1 ];
	data[  2 ] = d * data[ 2 ];
	data[  3 ] = d * data[ 3 ];
	data[  4 ] = d * ( (  t1 * tmp[  4 ] +  t2 * tmp[  8 ] +  t5 * tmp[ 12 ] ) - (  t0 * tmp[  4 ] +  t3 * tmp[  8 ] +  t4 * tmp[ 12 ] ) );
	data[  5 ] = d * ( (  t0 * tmp[  0 ] +  t7 * tmp[  8 ] +  t8 * tmp[ 12 ] ) - (  t1 * tmp[  0 ] +  t6 * tmp[  8 ] +  t9 * tmp[ 12 ] ) );
	data[  6 ] = d * ( (  t3 * tmp[  0 ] +  t6 * tmp[  4 ] + t11 * tmp[ 12 ] ) - (  t2 * tmp[  0 ] +  t7 * tmp[  4 ] + t10 * tmp[ 12 ] ) );
	data[  7 ] = d * ( (  t4 * tmp[  0 ] +  t9 * tmp[  4 ] + t10 * tmp[  8 ] ) - (  t5 * tmp[  0 ] +  t8 * tmp[  4 ] + t11 * tmp[  8 ] ) );
	data[  8 ] = d * ( ( t12 * tmp[  7 ] + t15 * tmp[ 11 ] + t16 * tmp[ 15 ] ) - ( t13 * tmp[  7 ] + t14 * tmp[ 11 ] + t17 * tmp[ 15 ] ) );
	data[  9 ] = d * ( ( t13 * tmp[  3 ] + t18 * tmp[ 11 ] + t21 * tmp[ 15 ] ) - ( t12 * tmp[  3 ] + t19 * tmp[ 11 ] + t20 * tmp[ 15 ] ) );
	data[ 10 ] = d * ( ( t14 * tmp[  3 ] + t19 * tmp[  7 ] + t22 * tmp[ 15 ] ) - ( t15 * tmp[  3 ] + t18 * tmp[  7 ] + t23 * tmp[ 15 ] ) );
	data[ 11 ] = d * ( ( t17 * tmp[  3 ] + t20 * tmp[  7 ] + t23 * tmp[ 11 ] ) - ( t16 * tmp[  3 ] + t21 * tmp[  7 ] + t22 * tmp[ 11 ] ) );
	data[ 12 ] = d * ( ( t14 * tmp[ 10 ] + t17 * tmp[ 14 ] + t13 * tmp[  6 ] ) - ( t16 * tmp[ 14 ] + t12 * tmp[  6 ] + t15 * tmp[ 10 ] ) );
	data[ 13 ] = d * ( ( t20 * tmp[ 14 ] + t12 * tmp[  2 ] + t19 * tmp[ 10 ] ) - ( t18 * tmp[ 10 ] + t21 * tmp[ 14 ] + t13 * tmp[  2 ] ) );
	data[ 14 ] = d * ( ( t18 * tmp[  6 ] + t23 * tmp[ 14 ] + t15 * tmp[  2 ] ) - ( t22 * tmp[ 14 ] + t14 * tmp[  2 ] + t19 * tmp[  6 ] ) );
	data[ 15 ] = d * ( ( t22 * tmp[ 10 ] + t16 * tmp[  2 ] + t21 * tmp[  6 ] ) - ( t20 * tmp[  6 ] + t23 * tmp[ 10 ] + t17 * tmp[  2 ] ) );
}

#if defined( MATH_RIGHTHANDED ) && defined( MATH_Z_NEG_1_TO_1 )
/// @desc Right Handed, z=-1 to 1
void mat4_set_orthographic( mat4 &mat, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far )
{
	f32 *data = ptr( mat );

	data[  0 ] =  2.0f / ( right - left );
	data[  1 ] =  0.0f;
	data[  2 ] =  0.0f;
	data[  3 ] =  0.0f;
	data[  4 ] =  0.0f;
	data[  5 ] =  2.0f / ( top - bottom );
	data[  6 ] =  0.0f;
	data[  7 ] =  0.0f;
	data[  8 ] =  0.0f;
	data[  9 ] =  0.0f;
	data[ 10 ] = -2.0f / ( far - near );
	data[ 11 ] =  0.0f;
	data[ 12 ] = -( right + left ) / ( right - left );
	data[ 13 ] = -( top + bottom ) / ( top - bottom );
	data[ 14 ] = -( far + near ) / ( far - near );
	data[ 15 ] =  1.0f;
}
#endif

#if defined( MATH_RIGHTHANDED ) && !defined( MATH_Z_NEG_1_TO_1 )
/// @desc Right Handed, z=0 to 1
void mat4_set_orthographic( mat4 &mat, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far )
{
	f32 *data = ptr( mat );

	data[  0 ] =  2.0f / ( right - left );
	data[  1 ] =  0.0f;
	data[  2 ] =  0.0f;
	data[  3 ] =  0.0f;
	data[  4 ] =  0.0f;
	data[  5 ] =  2.0f / ( top - bottom );
	data[  6 ] =  0.0f;
	data[  7 ] =  0.0f;
	data[  8 ] =  0.0f;
	data[  9 ] =  0.0f;
	data[ 10 ] = -1.0f / ( far - near );
	data[ 11 ] =  0.0f;
	data[ 12 ] = -( right + left ) / ( right - left );
	data[ 13 ] = -( top + bottom ) / ( top - bottom );
	data[ 14 ] = -( near ) / ( far - near );
	data[ 15 ] =  1.0f;
}
#endif

#if !defined( MATH_RIGHTHANDED ) && defined( MATH_Z_NEG_1_TO_1 )
// Left Handed, z=-1 to 1
void mat4_set_orthographic( mat4 &mat, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far )
{
	f32 *data = ptr( mat );

	data[  0 ] =  2.0f / ( right - left );
	data[  1 ] =  0.0f;
	data[  2 ] =  0.0f;
	data[  3 ] =  0.0f;
	data[  4 ] =  0.0f;
	data[  5 ] =  2.0f / ( top - bottom );
	data[  6 ] =  0.0f;
	data[  7 ] =  0.0f;
	data[  8 ] =  0.0f;
	data[  9 ] =  0.0f;
	data[ 10 ] =  2.0f / ( far - near );
	data[ 11 ] =  0.0f;
	data[ 12 ] = -( right + left ) / ( right - left );
	data[ 13 ] = -( top + bottom ) / ( top - bottom );
	data[ 14 ] = -( far + near ) / ( far - near );
	data[ 15 ] =  1.0f;
}
#endif

#if !defined( MATH_RIGHTHANDED ) && !defined( MATH_Z_NEG_1_TO_1 )
// Left Handed, z=0 to 1
void mat4_set_orthographic( mat4 &mat, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far )
{
	f32 *data = ptr( mat );

	data[  0 ] =  2.0f / ( right - left );
	data[  1 ] =  0.0f;
	data[  2 ] =  0.0f;
	data[  3 ] =  0.0f;
	data[  4 ] =  0.0f;
	data[  5 ] =  2.0f / ( top - bottom );
	data[  6 ] =  0.0f;
	data[  7 ] =  0.0f;
	data[  8 ] =  0.0f;
	data[  9 ] =  0.0f;
	data[ 10 ] =  1.0f / ( far - near );
	data[ 11 ] =  0.0f;
	data[ 12 ] = -( right + left ) / ( right - left );
	data[ 13 ] = -( top + bottom ) / ( top - bottom );
	data[ 14 ] = -( near ) / ( far - near );
	data[ 15 ] =  1.0f;
}
#endif

#ifdef MATH_RIGHTHANDED

/// @desc Right handed
void mat4_set_lookat( mat4 &mat, const vec3 &position, const vec3 &target, const vec3 &up )
{
	f32 *data = ptr( mat );

	vec3 zAxis = normal( target - position );
	vec3 xAxis = normal( cross_product( zAxis, up ) );
	vec3 yAxis = cross_product( xAxis, zAxis );

	data[  0 ] =  xAxis.x;
	data[  1 ] =  yAxis.x;
	data[  2 ] = -zAxis.x;
	data[  3 ] =  0.0f;
	data[  4 ] =  xAxis.y;
	data[  5 ] =  yAxis.y;
	data[  6 ] = -zAxis.y;
	data[  7 ] =  0.0f;
	data[  8 ] =  xAxis.z;
	data[  9 ] =  yAxis.z;
	data[ 10 ] = -zAxis.z;
	data[ 11 ] =  0.0f;
	data[ 12 ] = -dot( xAxis, position );
	data[ 13 ] = -dot( yAxis, position );
	data[ 14 ] =  dot( zAxis, position );
	data[ 15 ] =  1.0f;
}

#else // !MATH_RIGHTHANDED

// @desc Left handed
void mat4_set_lookat( mat4 &mat, const vec3 &position, const vec3 &target, const vec3 &up )
{
	f32 *data = ptr( mat );

	vec3 zAxis = normal( target - position );
	vec3 xAxis = normal( cross_product( up, zAxis ) );
	vec3 yAxis = cross_product( xAxis, zAxis );

	data[  0 ] =  xAxis.x;
	data[  1 ] =  yAxis.x;
	data[  2 ] =  zAxis.x;
	data[  3 ] =  0.0f;
	data[  4 ] =  xAxis.y;
	data[  5 ] =  yAxis.y;
	data[  6 ] =  zAxis.y;
	data[  7 ] =  0.0f;
	data[  8 ] =  xAxis.z;
	data[  9 ] =  yAxis.z;
	data[ 10 ] =  zAxis.z;
	data[ 11 ] =  0.0f;
	data[ 12 ] = -dot( xAxis, position );
	data[ 13 ] = -dot( yAxis, position );
	data[ 14 ] = -dot( zAxis, position );
	data[ 15 ] =  1.0f;
}

#endif // !MATH_RIGHTHANDED

void mat4_set_as_translation( mat4 &mat, const vec3 &position )
{
	f32 *data = ptr( mat );

	data[  0 ] = 1.0f;
	data[  1 ] = 0.0f;
	data[  2 ] = 0.0f;
	data[  3 ] = 0.0f;
	data[  4 ] = 0.0f;
	data[  5 ] = 1.0f;
	data[  6 ] = 0.0f;
	data[  7 ] = 0.0f;
	data[  8 ] = 0.0f;
	data[  9 ] = 0.0f;
	data[ 10 ] = 1.0f;
	data[ 11 ] = 0.0f;
	data[ 12 ] = position.x;
	data[ 13 ] = position.y;
	data[ 14 ] = position.z;
	data[ 15 ] = 1.0f;
}

void mat4_set_as_scale( mat4 &mat, const vec3 &scale )
{
	f32 *data = ptr( mat );

	data[  0 ] = scale.x;
	data[  1 ] = 0.0f;
	data[  2 ] = 0.0f;
	data[  3 ] = 0.0f;
	data[  4 ] = 0.0f;
	data[  5 ] = scale.y;
	data[  6 ] = 0.0f;
	data[  7 ] = 0.0f;
	data[  8 ] = 0.0f;
	data[  9 ] = 0.0f;
	data[ 10 ] = scale.z;
	data[ 11 ] = 0.0f;
	data[ 12 ] = 0.0f;
	data[ 13 ] = 0.0f;
	data[ 14 ] = 0.0f;
	data[ 15 ] = 1.0f;
}

void mat4_set_as_euler_x( mat4 &mat, f32 angleRadian )
{
	f32 *data = ptr( mat );

	f32 c = cosf( angleRadian );
	f32 s = sinf( angleRadian );

	data[  0 ] =  1.0f;
	data[  1 ] =  0.0f;
	data[  2 ] =  0.0f;
	data[  3 ] =  0.0f;
	data[  4 ] =  0.0f;
	data[  5 ] =  c;
	data[  6 ] =  s;
	data[  7 ] =  0.0f;
	data[  8 ] =  0.0f;
	data[  9 ] = -s;
	data[ 10 ] =  c;
	data[ 11 ] =  0.0f;
	data[ 12 ] =  0.0f;
	data[ 13 ] =  0.0f;
	data[ 14 ] =  0.0f;
	data[ 15 ] =  1.0f;
}

void mat4_set_as_euler_y( mat4 &mat, f32 angleRadian )
{
	f32 *data = ptr( mat );

	f32 c = cosf( angleRadian );
	f32 s = sinf( angleRadian );

	data[  0 ] =  c;
	data[  1 ] =  0.0f;
	data[  2 ] = -s;
	data[  3 ] =  0.0f;
	data[  4 ] =  0.0f;
	data[  5 ] =  1.0f;
	data[  6 ] =  0.0f;
	data[  7 ] =  0.0f;
	data[  8 ] =  s;
	data[  9 ] =  0.0f;
	data[ 10 ] =  c;
	data[ 11 ] =  0.0f;
	data[ 12 ] =  0.0f;
	data[ 13 ] =  0.0f;
	data[ 14 ] =  0.0f;
	data[ 15 ] =  1.0f;
}

void mat4_set_as_euler_z( mat4 &mat, f32 angleRadian )
{
	f32 *data = ptr( mat );

	f32 c = cosf( angleRadian );
	f32 s = sinf( angleRadian );

	data[  0 ] =  c;
	data[  1 ] =  s;
	data[  2 ] =  0.0f;
	data[  3 ] =  0.0f;
	data[  4 ] = -s;
	data[  5 ] =  c;
	data[  6 ] =  0.0f;
	data[  7 ] =  0.0f;
	data[  8 ] =  0.0f;
	data[  9 ] =  0.0f;
	data[ 10 ] =  1.0f;
	data[ 11 ] =  0.0f;
	data[ 12 ] =  0.0f;
	data[ 13 ] =  0.0f;
	data[ 14 ] =  0.0f;
	data[ 15 ] =  1.0f;
}

void mat4_set_as_euler_xyz( mat4 &mat, vec3 vec )
{
	mat4 rx, ry, rz;
	mat4_set_as_euler_x( rx, vec.x );
	mat4_set_as_euler_y( ry, vec.y );
	mat4_set_as_euler_z( rz, vec.z );
	mat = rx * ry * rz;
}

void mat4_set_as_euler_xyz( mat4 &mat, f32 xRadian, f32 yRadian, f32 zRadian )
{
	mat4 rx, ry, rz;
	mat4_set_as_euler_x( rx, xRadian );
	mat4_set_as_euler_y( ry, yRadian );
	mat4_set_as_euler_z( rz, zRadian );
	mat = rx * ry * rz;
}

void mat4_set_as_quaternion( mat4 &mat, const Quaternion &q )
{
	f32 *data = ptr( mat );
	const Quaternion qn = normal( q );

	data[  0 ] = 1.0f - 2.0f * qn.y * qn.y - 2.0f * qn.z * qn.z;
	data[  1 ] = 2.0f * qn.x * qn.y - 2.0f * qn.z * qn.w;
	data[  2 ] = 2.0f * qn.x * qn.z + 2.0f * qn.y * qn.w;
	data[  3 ] = 0.0f;
	data[  4 ] = 2.0f * qn.x * qn.y + 2.0f * qn.z * qn.w;
	data[  5 ] = 1.0f - 2.0f * qn.x * qn.x - 2.0f * qn.z * qn.z;
	data[  6 ] = 2.0f * qn.y * qn.z - 2.0f * qn.x * qn.w;
	data[  7 ] = 0.0f;
	data[  8 ] = 2.0f * qn.x * qn.z - 2.0f * qn.y * qn.w;
	data[  9 ] = 2.0f * qn.y * qn.z + 2.0f * qn.x * qn.w;
	data[ 10 ] = 1.0f - 2.0f * qn.x * qn.x - 2.0f * qn.y * qn.y;
	data[ 11 ] = 0.0f;
	data[ 12 ] = 0.0f;
	data[ 13 ] = 0.0f;
	data[ 14 ] = 0.0f;
	data[ 15 ] = 1.0f;
}

void mat4_set_as_quaternion( mat4 &mat, const Quaternion &q, const vec3 &centre )
{
	f32 *data = ptr( mat );

	data[  0 ] = ( q.x * q.x ) - ( q.y * q.y ) - ( q.z * q.z ) + ( q.w * q.w );
	data[  1 ] = 2.0f * ( ( q.x * q.y ) + ( q.z * q.w ) );
	data[  2 ] = 2.0f * ( ( q.x * q.z ) - ( q.y * q.w ) );
	data[  3 ] = centre.x - centre.x * data[ 0 ] - centre.y * data[ 1 ] - centre.z * data[ 2 ];
	data[  4 ] = 2.0f * ( ( q.x * q.y ) - ( q.z * q.w ) );
	data[  5 ] = -( q.x * q.x ) + ( q.y * q.y ) - ( q.z * q.z ) + ( q.w * q.w );
	data[  6 ] = 2.0f * ( ( q.y * q.z ) + ( q.x * q.w ) );
	data[  7 ] = centre.y - centre.x * data[ 4 ] - centre.y * data[ 5 ] - centre.z * data[ 6 ];
	data[  8 ] = 2.0f * ( ( q.x * q.z ) + ( q.y * q.w ) );
	data[  9 ] = 2.0f * ( ( q.y * q.z ) - ( q.x * q.w ) );
	data[ 10 ] = -( q.x * q.x ) - ( q.y * q.y ) + ( q.z * q.z ) + ( q.w * q.w );
	data[ 11 ] = centre.z - centre.x * data[ 8 ] - centre.y * data[ 9 ] - centre.z * data[ 10 ];
	data[ 12 ] = 0.0f;
	data[ 13 ] = 0.0f;
	data[ 14 ] = 0.0f;
	data[ 15 ] = 1.0f;
}

#endif