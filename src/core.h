
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <intrin.h>

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

static_assert( sizeof( i8 ) == 1 );
static_assert( sizeof( i16 ) == 2 );
static_assert( sizeof( i32 ) == 4 );
static_assert( sizeof( i64 ) == 8 );
static_assert( sizeof( u8 ) == 1 );
static_assert( sizeof( u16 ) == 2 );
static_assert( sizeof( u32 ) == 4 );
static_assert( sizeof( u64 ) == 8 );
static_assert( sizeof( f32 ) == 4 );
static_assert( sizeof( f64 ) == 8 );

void handle_assert( const char *file, i32 line, const char *condition );
void message_log( const char *message, ... );
void message_warning( const char *file, i32 line, const char *message, ... );
void message_error( const char *file, i32 line, const char *message, ... );

void handle_assert( const char *file, i32 line, const char *condition )
{
	fprintf( stderr, "FAILED ASSERT: %s\n%s (%i)\n", condition, file, line );
	__debugbreak();
}

void message_log( const char *message, ... )
{
	va_list args;
	va_start( args, message );
	vprintf( message, args );
	va_end( args );
	printf( "\n" );
}

void message_warning( const char *file, i32 line, const char *message, ... )
{
	printf( "[WARN]: @ %s (%d)\n\t", file, line );
	va_list args;
	va_start( args, message );
	vprintf( message, args );
	va_end( args );
	printf( "\n" );
}

void message_error( const char *file, i32 line, const char *message, ... )
{
	fprintf( stderr, "\n[ ERR]: @ %s (%d)\n\t", file, line );
	va_list args;
	va_start( args, message );
	vfprintf( stderr, message, args );
	va_end( args );
	fprintf( stderr, "\n" );
	__debugbreak();
}

#ifdef DEBUG
#define log( msg, ... ) message_log( msg __VA_OPT__(,) __VA_ARGS__ )
#define log_warning( msg, ... ) message_warning( __FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__ )
#define log_error( msg, ... ) message_error( __FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__ )
#else
#define log( msg, ... ) do {} while ( 0 )
#define log_warning( msg, ... ) do {} while ( 0 )
#define log_error( msg, ... ) do {} while ( 0 )
#endif

#define assert( condition ) \
	do \
	{ \
		if ( !( condition ) ) [[unlikely]] \
			handle_assert( __FILE__, __LINE__, #condition ); \
	} while ( 0 )

#define KB( m )		( ( m ) * 1024 )
#define MB( m )		( KB( m ) * 1024 )
#define GB( m )		( MB( m ) * 1024 )
#define TB( m )		( GB( m ) * 1024 )

#define BIT( b )	( 1u << ( b ) )

#define ARRAY_LENGTH( arr )		( sizeof( arr ) / sizeof( arr[ 0 ] ) )

constexpr u64 MAX_FILEPATH = 260;

struct Platform *platform = nullptr;
struct MemoryArena *memory = nullptr;

#ifdef INCLUDE_ZLIB
#include "zlib.h"
#endif

#include "core.h"
#include "math_functions.h"
#include "random_functions.h"
#include "memory_functions.h"
#include "array.h"
#include "string_functions.h"
#include "map.h"
#include "utility.h"
#include "platform.h"
#include "file_functions.h"