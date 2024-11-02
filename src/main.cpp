
#include "core.h"

enum ERROR_CODE
{
	ERROR_CODE_NO_INPUT_FILES = -1,
	ERROR_CODE_FAILED_TO_INITIALISE_MEMORY_ARENA = -2,
	ERROR_CODE_FAILED_TO_INITIALISE_PLATFORM = -3,
};

// -------------------------------------------------------
// ENTRY
// -------------------------------------------------------

int main( int argc, char *argv[] )
{
	if ( argc <= 1 )
	{
		log_warning( "No input files." );
		return ERROR_CODE_NO_INPUT_FILES;
	}

	{
		// -- memory ---------------------------------------------
		MemoryArena memoryArena = memory_default();

		if ( !memoryArena.init( KB( 1 ), MB( 32 ), KB( 0 ), true ) )
		{
			log_warning( "Failed to initialise memory arena." );
			return ERROR_CODE_FAILED_TO_INITIALISE_MEMORY_ARENA;
		}

		log( "Memory initialised." );

		// -- platform ---------------------------------------------
		if ( !platform_init( &memoryArena ) )
		{
			log_warning( "Failed to initialise platform." );
			return ERROR_CODE_FAILED_TO_INITIALISE_PLATFORM;
		}

		platform->frame = 0;

		log( "Platform initialised." );

		// -- random seed ---------------------------------------------
		#if 0
			u64 seed = platform_get_seed();

			random_set_seed( seed );

			u64 state[ 9 ];
			random_get_state( state );

			log( "Random Seed:\n\t%llu, %llu, %llu\n\t%llu, %llu, %llu\n\t%llu, %llu, %llu",
				state[ 0 ], state[ 1 ], state[ 2 ], state[ 3 ], state[ 4 ], state[ 5 ], state[ 6 ], state[ 7 ], state[ 8 ] );
		#endif
	}

	for ( i32 argEntry = 0, count = argc - 1; argEntry < count; ++argEntry )
	{
		const char *filepath = argv[ argEntry + 1 ];

		log( "Processing: %s", filepath );

		u64 fileSize;
		u8 *file = read_file( filepath, &fileSize, true, &memory->transient );
		u8 *newFile = memory->transient.allocate<u8>( fileSize, true );
		u8 *src = file;
		u8 *dst = newFile;
		char l = '\0';
		char c = *src++;
		bool inComment = false;
		bool blockComment = false;
		bool inStringLiteral = false;
		char stringLiteralOpener = '\0';

		while ( c != '\0' )
		{
			if ( c == '/' && ( *src == '/' || *src == '*' ) && !inComment && !inStringLiteral )
			{
				inComment = true;
				blockComment = ( *src == '*' );
				src += 1;
			}
			else if ( c == '\n' && !blockComment )
			{
				inComment = false;
			}
			else if ( ( c == '*' && *src == '/' ) && blockComment )
			{
				inComment = false;
				blockComment = false;
				src += 1;
			}
			else if ( !inComment )
			{
				if ( l != '\\' )
				{
					if ( c == '"' && stringLiteralOpener == '"' )
					{
						inStringLiteral = !inStringLiteral;
						stringLiteralOpener = '"';
					}
					else if ( c == '\'' && stringLiteralOpener == '\'' )
					{
						inStringLiteral = !inStringLiteral;
						stringLiteralOpener = '\'';
					}
				}

				*dst++ = c;
			}

			l = c;
			c = *src++;
		}

		memory->transient.free( newFile );
		memory->transient.free( file );

		log( "Writing file: %s", filepath );

		if ( write_file( filepath, newFile, dst - newFile, false ) == 0 )
		{
			log_warning( "Failed to write file: %s", filepath );
		}
	}

	return 0;
}

// -- UNITY BUILD --
#if defined( __PLATFORM_WINDOWS__ )
#include "platform_windows.cpp"
#else
#warning No Platform Selected.
#endif

#include "utility.cpp"

#define FILE_FUNCTIONS_IMPLEMENTATION
#include "file_functions.h"

#define MATH_FUNCTIONS_IMPLEMENTATION
#include "math_functions.h"

#define RANDOM_FUNCTIONS_IMPLEMENTATION
#include "random_functions.h"

#define STRING_FUNCTIONS_IMPLEMENTATION
#include "string_functions.h"

#define MEMORY_FUNCTIONS_IMPLEMENTATION
#include "memory_functions.h"
