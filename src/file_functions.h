
#ifndef _HG_FILE_FUNCTIONS
#define _HG_FILE_FUNCTIONS

constexpr const u64 MAX_FIND_FILES = 4096;
constexpr const u64 INVALID_FILE_INDEX = UINT64_MAX;
constexpr const u64 INVALID_DIR_INDEX = UINT64_MAX;

using FilePermissions = u8;
enum FILE_PERMISSION : FilePermissions
{
	FILE_PERMISSION_READ	= BIT( 0 ),
	FILE_PERMISSION_WRITE	= BIT( 1 ),
};

using FileOptions = u32;
enum FILE_OPTION : FileOptions
{
	FILE_OPTION_READ					= BIT( 0 ),			// open the file for reading
	FILE_OPTION_WRITE					= BIT( 1 ),			// open the file for writing
	FILE_OPTION_CREATE					= BIT( 2 ),			// create the file if it doesnt exist
	FILE_OPTION_APPEND					= BIT( 3 ),			// start at the end of the file
	FILE_OPTION_CLEAR					= BIT( 4 ),			// the file is cleared
};

using FileSeek = u32;
enum FILE_SEEK : FileSeek
{
	FILE_SEEK_START,
	FILE_SEEK_CURRENT,
	FILE_SEEK_END,
};

using FileMove = u32;
enum FILE_MOVE : FileMove
{
	FILE_MOVE_REPLACE					= BIT( 0 ),			// if the target file already exists replace it
	FILE_MOVE_ERROR_LOG					= BIT( 1 ),			// if an error occurs it logs it
};

using FileCopy = u32;
enum FILE_COPY : FileCopy
{
	FILE_COPY_ERROR_LOG					= BIT( 0 ),			// if an error occurs it logs it
};

enum FILE_HEADER_FLAG : u32
{
	FILE_HEADER_FLAG_BASE64_ENCODED		= BIT( 0 ),
	FILE_HEADER_FLAG_COMPRESSED			= BIT( 1 ),
};
using FileHeaderFlags = u32;

struct FileHeader
{
	u32 version;
	char id[ 4 ];
	u64 decodedSize;
	u64 uncompressedSize;
	FileHeaderFlags flags;
};

struct DirEntry
{
	char path[ MAX_FILEPATH ];
	u64 creationDate;
};

struct FileInDir
{
	const char *path;
	u64 creationDate;
};

// Utility
char *handle_file_includes( const char *root, char *code, u64 codeSize, Allocator *allocator );

// Directories & Paths
u64 open_directory( const char *path );
bool directory_next_entry( u64 dirID, DirEntry *entry );
bool directory_next_file_entry( u64 dirID, DirEntry *entry );
void close_directory( u64 dirID );
bool make_directory( const char *directory );
bool delete_directory( const char *directory );
bool change_directory( const char *directory );
[[nodiscard]] char *get_directory();
[[nodiscard]] char *get_relative_filename( char *path );
[[nodiscard]] const char *get_relative_filename( const char *path );
bool get_files_in_directory( const char *path, Array<FileInDir, MAX_FIND_FILES> *files, bool fullPath, bool recursive, Allocator *allocator );
const char *abs_path( const char *path, char *abPath, u64 abPathFileSize );
[[nodiscard]] const char *abs_path( const char *path, Allocator *allocator );

// Files
[[nodiscard]] bool file_exists( const char *path );
[[nodiscard]] u8 *read_file( const char *path, u64 *fileSize, bool addNullTerminator, Allocator *allocator );
u64 write_file( const char *path, const u8 *buffer, u64 size, bool append );
bool delete_file( const char *path );
[[nodiscard]] bool file_permissions( const char *path, FilePermissions permissions );
bool can_open_file( const char *path, FileOptions options, i32 attempts );
u64 open_file( const char *path, FileOptions options );
void close_file( u64 fileID );
u64 get_file_size( u64 fileID );
bool seek_in_file( u64 fileID, FileSeek seek, u64 offset );
u8 *read_whole_file( u64 fileID, Allocator *allocator, bool addNullTerminator );
u64 read_from_file( u64 fileID, void *buffer, u64 size );
u64 write_to_file( u64 fileID, const void *buffer, u64 size );
void file_flush( u64 fileID );
[[nodiscard]] u64 file_creation_timestamp( const char *path );
[[nodiscard]] u64 file_last_edit_timestamp( const char *path );
bool move_file( const char *from, const char *to, FileMove move = FILE_MOVE_ERROR_LOG );
bool move_file_retry( const char *from, const char *to, FileMove move = FILE_MOVE_ERROR_LOG, i32 attempts = 3, i32 msWaitPerAttempt = 0 );
bool copy_file( const char *from, const char *to, FileCopy copy = FILE_COPY_ERROR_LOG );
bool copy_file_retry( const char *from, const char *to, FileCopy copy = FILE_COPY_ERROR_LOG, i32 attempts = 3, i32 msWaitPerAttempt = 0 );

// Saving/Loading
bool save_file( const char *path, void *data, u64 size, FileHeader *header, Allocator *allocator );
[[nodiscard]] u8 *load_file( const char *path, u64 *fileSize, FileHeader *header, Allocator *allocator );

#endif // _HG_FILE_FUNCTIONS

// --------------------------------------------------------------------------------

#ifdef FILE_FUNCTIONS_IMPLEMENTATION

#if defined( _WIN32 )

	#include <direct.h>
	#include "dirent/dirent.h"

	static_assert( MAX_FILEPATH >= MAX_PATH );

	#define finternal_stat_struct		struct _stat64
	#define finternal_stat				_stat64
	#define finternal_mkdir( p, m )		_mkdir( p )
	#define finternal_seek				_fseeki64
	#define finternal_ftell				_ftelli64
	#define finternal_unlink			_unlink
	#define finternal_rmdir				_rmdir
	#define finternal_chdir				_chdir
	#define finternal_getcwd			_getcwd
	#define finternal_rename			rename

#else

	#include <dirent.h>
	#include <sys/stat.h>

	#define finternal_stat_struct		struct stat64
	#define finternal_stat				stat64
	#define finternal_mkdir( p, m )		mkdir( p, m )
	#define finternal_seek				fseeko
	#define finternal_ftell				ftello
	#define finternal_unlink			unlink
	#define finternal_rmdir				rmdir
	#define finternal_chdir				chdir
	#define finternal_getcwd			getcwd
	#define finternal_rename			rename

#endif

char *handle_file_includes( const char *root, char *code, u64 codeSize, Allocator *allocator )
{
	char *codeStart = code;
	u64 defaultAllocationSize = codeSize * 2 + 8192;

	// Allocate more than needed to increase the chance of it not needing
	// to reallocate (and move) when an include is encountered
	DynamicArray<char> buffer =
	{
		.count = 0,
		.currentCapacity = defaultAllocationSize,
		.data = allocator->allocate<char>( defaultAllocationSize ),
		.allocator = allocator,
	};

	buffer.append( code, codeSize );

	bool inComment = false;
	bool blockComment = false;

	for ( u64 i = 0; i < codeSize; ++i )
	{
		char c = *code++;

		// Search for #include
		if ( c == '#' && !inComment )
		{
			char *lhs = code;
			const char *rhs = "include \"";

			bool match = true;

			while ( *rhs != '\0' )
			{
				if ( *lhs++ != *rhs++ )
				{
					match = false;
					break;
				}
			}

			if ( match )
			{
				char filepath[ MAX_FILEPATH ];
				u64 find = string_utf8_find_first( lhs, "\"" );
				if ( find != UINT64_MAX )
				{
					u64 size = string_utf8_copy( filepath, sizeof( filepath ), root );
					string_utf8_copy( filepath + size, sizeof( filepath ) - size, lhs, find );

					u64 includedFileSize = 0;
					char *includedCode = (char *)read_file( filepath, &includedFileSize, false, allocator );

					if ( includedCode )
					{
						u64 includeFilenameLength = ( lhs - code ) + 1 + size + 1;
						// Insert a gap after the #include "file.h" that the included file will go
						buffer.insert( i + includeFilenameLength, includedFileSize - includeFilenameLength );
						// Paste the included file, it will overwrite the #include instruction
						buffer.paste( i, includedCode, includedFileSize );
						code = lhs + size + 1;
					}
				}
			}
		}
		else if ( c == '/' && ( *code == '/' || *code == '*' ) && !inComment )
		{
			inComment = true;
			blockComment = ( *code == '*' );
			code += 1;
		}
		else if ( c == '\n' && !blockComment )
		{
			inComment = false;
		}
		else if ( ( *code == '*' && *code == '/' ) && blockComment )
		{
			inComment = false;
			blockComment = false;
			code += 1;
		}
	}

	// If no other allocations happen because there are no includes
	// then these can be free'd together, else it's fine since its all
	// transient memory anyway.
	allocator->attach( buffer.data, codeStart );

	return buffer.data;
}

u64 open_directory( const char *path )
{
	DIR *dir = opendir( path );

	if ( !dir )
	{
		log_warning( "Failed to open directory: \"%s\"", path );
		return INVALID_DIR_INDEX;
	}

	return reinterpret_cast<u64>( dir );
}

bool directory_next_entry( u64 dirID, DirEntry *entry )
{
	DIR *dir = reinterpret_cast<DIR *>( dirID );
	if ( !dir )
		return false;
	struct dirent *ent = readdir( dir );
	if ( !ent )
		return false;
	string_utf8_copy( entry->path, ent->d_name );
	entry->creationDate = file_creation_timestamp( ent->d_name );
	return true;
}

bool directory_next_file_entry( u64 dirID, DirEntry *entry )
{
	DIR *dir = reinterpret_cast<DIR *>( dirID );
	if ( !dir )
		return false;
	struct dirent *ent = readdir( dir );
	while ( ent )
	{
		if ( !string_utf8_compare( ent->d_name, "." ) && !string_utf8_compare( ent->d_name, ".." ) )
		{
			string_utf8_copy( entry->path, ent->d_name );
			entry->creationDate = file_creation_timestamp( ent->d_name );
			return true;
		}
		ent = readdir( dir );
	}
	return false;
}

void close_directory( u64 dirID )
{
	DIR *dir = reinterpret_cast<DIR *>( dirID );
	if ( dir )
		closedir( dir );
}

bool make_directory( const char *directory )
{
	char pathMem[ PATH_MAX ];
	if ( string_utf8_copy( pathMem, directory ) == 0 )
		return false;

	char *path = pathMem;
	const char *token;
	const char *delimiters = "./\\";
	char delim;
	char dir[ PATH_MAX ] = "";

	if ( *path == '.' )
	{
		path += 1;

		if ( *path == '/' || *path == '\\' )
		{
			// ./ (current directory)
			string_utf8_append( dir, "./" );
		}
		else if ( *path == '.' )
		{
			path += 1;

			// ../ (moving up from current directory)
			if ( *path == '/' || *path == '\\' )
			{
				// ./ (current directory)
				string_utf8_append( dir, "../" );
			}
		}
	}

	path = string_utf8_tokenise( path, delimiters, &token, &delim );

	while ( token )
	{
		if ( delim == '.' )
		{
			// file ext
			return true;
		}

		string_utf8_append( dir, token );
		string_utf8_append( dir, "/" );

		if ( finternal_mkdir( dir, 0777 ) != 0 )
			return false;

		path = string_utf8_tokenise( path, delimiters, &token, &delim );
	}

	return true;
}

bool delete_directory( const char *directory )
{
	DIR *dir = opendir( directory );
	if ( !dir )
		return false;

	struct dirent *entry;

	while ( ( entry = readdir( dir ) ) != nullptr )
	{
		if ( string_utf8_compare( entry->d_name, "." ) || string_utf8_compare( entry->d_name, ".." ) )
			continue;

		if ( entry->d_type == DT_DIR )
		{
			char subdirectoryPath[ PATH_MAX ] = "";
			string_utf8_copy( subdirectoryPath, directory );
			string_utf8_append( subdirectoryPath, "/" );
			string_utf8_append( subdirectoryPath, entry->d_name );

			delete_directory( subdirectoryPath );
		}
	}

	closedir( dir );

	dir = opendir( directory );
	if ( !dir )
		return false;

	while ( ( entry = readdir( dir ) ) != nullptr )
	{
		if ( string_utf8_compare( entry->d_name, "." ) || string_utf8_compare( entry->d_name, ".." ) )
			continue;

		char filePath[ PATH_MAX ] = "";
		string_utf8_copy( filePath, directory );
		string_utf8_append( filePath, "/" );
		string_utf8_append( filePath, entry->d_name );

		finternal_unlink( filePath );
	}

	closedir( dir );

	return finternal_rmdir( directory ) == 0;
}

bool change_directory( const char *directory )
{
	assert( directory );
	if ( finternal_chdir( directory ) != 0 )
		return false;
	string_utf8_copy( platform->workingDirectory, directory );
	return true;
}
 
[[nodiscard]] char *get_directory()
{
	Allocator *allocator = &platform->memory.transient;

	u32 bufferSize = PATH_MAX;

	char *buffer = allocator->allocate<char>( bufferSize );
	if ( !buffer )
		return nullptr;

	bool success = finternal_getcwd( buffer, bufferSize );

	if ( !success )
	{
		allocator->free( buffer );
		return nullptr;
	}

	allocator->shrink( buffer, strlen( buffer ) + 1 );
	return buffer;
}

[[nodiscard]] char *get_relative_filename( char *path )
{
	path = string_utf8_past_start_case_insensitive( path, platform->workingDirectory );
	while ( *path == '\\' || *path == '/' )
		path += 1;
	return path;
}

[[nodiscard]] const char *get_relative_filename( const char *path )
{
	path = string_utf8_past_start_case_insensitive( path, platform->workingDirectory );
	while ( *path == '\\' || *path == '/' )
		path += 1;
	return path;
}

bool get_files_in_directory__internal( const char *path, Array<FileInDir, MAX_FIND_FILES> *files, bool fullPath, bool recursive, Allocator *allocator )
{
	finternal_stat_struct st;

	if ( finternal_stat( path, &st ) != 0 )
	{
		log_warning( "Failed to get stat for path: %s", path );
		return false;
	}

	if ( !S_ISDIR( st.st_mode ) )
	{
		log_warning( "Path \"%s\" is not a directory.", path );
		return false;
	}

	DIR *dir = opendir( path );
	if ( !dir )
	{
		log_warning( "Failed to open directory: \"%s\"", path );
		return false;
	}

	const char *prevDir = get_directory();

	if ( finternal_chdir( path ) != 0 )
	{
		log_warning( "Failed to change to directory: \"%s\"", path );
		return false;
	}

	struct dirent *entry;
	while ( ( entry = readdir( dir ) ) != nullptr )
	{
		if ( entry->d_type == DT_REG )
		{
			const char *file = abs_path( entry->d_name, allocator );
			if ( !fullPath )
				file = get_relative_filename( file );

			files->add(
			{
				.path = file,
				.creationDate = file_creation_timestamp( entry->d_name ),
			} );
		}
		else if ( recursive && entry->d_type == DT_DIR && !string_utf8_compare( entry->d_name, "." ) && !string_utf8_compare( entry->d_name, ".." ) )
		{
			if ( !get_files_in_directory__internal( entry->d_name, files, fullPath, recursive, allocator ) )
			{
				closedir( dir );
				return false;
			}
		}
	}

	if ( finternal_chdir( prevDir ) != 0 )
	{
		log_warning( "Failed to restore directory: %s", prevDir );
	}

	closedir( dir );

	return true;
}

bool get_files_in_directory( const char *path, Array<FileInDir, MAX_FIND_FILES> *files, bool fullPath, bool recursive, Allocator *allocator )
{
	const char *prevDir = get_directory();

	string_utf8_copy( platform->workingDirectory, abs_path( path, allocator ) );

	bool ret = get_files_in_directory__internal( path, files, fullPath, recursive, allocator );

	change_directory( prevDir );

	return ret;
}

const char *abs_path( const char *path, char *abPath, u64 abPathFileSize )
{
	assert( abPathFileSize >= MAX_FILEPATH );

	#ifdef _WIN32
		GetFullPathNameA( path, (DWORD)abPathFileSize, abPath, nullptr );
	#else
		realpath( path, abPath );
	#endif

	return abPath;
}

const char *abs_path( const char *path, Allocator *allocator )
{
	char *buffer = allocator->allocate<char>( (u64)MAX_FILEPATH );
	abs_path( path, buffer, MAX_FILEPATH );
	allocator->shrink( buffer, string_utf8_bytes( buffer ) );
	return buffer;
}

[[nodiscard]] bool file_exists( const char *path )
{
	finternal_stat_struct st;
	bool success = finternal_stat( path, &st ) == 0;

	if ( !success )
		return false;

	return S_ISREG( st.st_mode );
}

[[nodiscard]] u8 *read_file( const char *path, u64 *fileSize, bool addNullTerminator, Allocator *allocator )
{
	FILE *file = fopen( path, "rb" );

	if ( !file )
	{
		log_warning( "Failed to open file: \"%s\"", path );
		return nullptr;
	}

	finternal_seek( file, 0, SEEK_END );
	u64 size = finternal_ftell( file );
	finternal_seek( file, 0, SEEK_SET );

	if ( size == 0 )
	{
		fclose( file );
		log_warning( "File \"%s\" totally empty ( %llu bytes )", path, size );
		if ( fileSize )
			*fileSize = 0;
		return nullptr;
	}

	u8 *buffer = allocator->allocate<u8>( size + ( addNullTerminator ? 1 : 0 ) );

	if ( !buffer )
	{
		fclose( file );
		log_warning( "Failed to allocate memory ( %llu bytes )", size );
		return nullptr;
	}

	u64 bytesRead = fread( buffer, 1, size, file );

	if ( bytesRead != size )
	{
		fclose( file );
		allocator->free( buffer );
		log_warning( "Failed to read file: %s", path );
		if ( fileSize )
			*fileSize = bytesRead;
		return nullptr;
	}

	if ( addNullTerminator )
	{
		buffer[ bytesRead ] = '\0';
		bytesRead += 1;
	}

	if ( fileSize )
		*fileSize = bytesRead;

	fclose( file );

	return buffer;
}

u64 write_file( const char *path, const u8 *buffer, u64 size, bool append )
{
	FILE *file = fopen( path, "wb" );

	if ( append )
	{
		finternal_seek( file, 0, SEEK_END );
	}

	if ( !file )
	{
		log_warning( "Failed to open file: \"%s\"", path );
		return 0;
	}

	u64 bytesWritten = fwrite( buffer, 1, size, file );

	fclose( file );

	if ( size != bytesWritten )
	{
		log_warning( "Failed to write file: \"%s\"", path );
		return 0;
	}

	return bytesWritten;
}

bool delete_file( const char *path )
{
	return finternal_unlink( path ) == 0;
}

bool file_permissions( const char *path, FilePermissions permissions )
{
	finternal_stat_struct st;
	bool success = finternal_stat( path, &st ) == 0;

	if ( !success )
		return false;

	switch ( permissions )
	{
	case FILE_PERMISSION_READ:
		return ( st.st_mode & S_IRUSR ) != 0;
		break;

	case FILE_PERMISSION_WRITE:
		return ( st.st_mode & S_IWUSR ) != 0;
		break;

	case FILE_PERMISSION_READ | FILE_PERMISSION_WRITE:
		return ( st.st_mode & ( S_IRUSR | S_IWUSR ) ) == ( S_IRUSR | S_IWUSR );
		break;

	default:
		log_warning( "Unknown permissions: %d", permissions );
	}

	return false;
}

bool can_open_file( const char *path, FileOptions options, i32 attempts )
{
	while ( attempts-- > 0 )
	{
		u64 file = open_file( path, options );

		if ( file != INVALID_FILE_INDEX )
		{
			close_file( file );
			return true;
		}

		platform_delay( 25 + attempts );
	}

	return false;
}

u64 open_file( const char *path, FileOptions options )
{
	FILE *file;

	switch ( options & ( FILE_OPTION_READ | FILE_OPTION_WRITE | FILE_OPTION_CREATE | FILE_OPTION_CLEAR ) )
	{
	case FILE_OPTION_READ:
		// Open the file only if it exists & it can be read
		if ( !file_permissions( path, FILE_PERMISSION_READ ) )
			return INVALID_FILE_INDEX;
		file = fopen( path, "rb" );
		break;

	case FILE_OPTION_WRITE:
		// Open the file only if it exists & it can be written to
		if ( !file_permissions( path, FILE_PERMISSION_WRITE ) )
			return INVALID_FILE_INDEX;
		file = fopen( path, "r+b" );
		break;

	case FILE_OPTION_READ | FILE_OPTION_WRITE:
		// Open the file only if it exists & it can be read & written to
		if ( !file_permissions( path, FILE_PERMISSION_READ | FILE_PERMISSION_WRITE ) )
			return INVALID_FILE_INDEX;
		file = fopen( path, "r+b" );
		break;

	case FILE_OPTION_READ | FILE_OPTION_CREATE:
		// Open the file for reading, create it if it doesn't already exist
		file = fopen( path, "rb" );
		if ( !file )
			file = fopen( path, "w+b" );
		break;

	case FILE_OPTION_WRITE | FILE_OPTION_CREATE:
		// Open the file for writing, create it if it doesn't already exist
		file = fopen( path, "r+b" );
		if ( !file )
			file = fopen( path, "wb" );
		break;

	case FILE_OPTION_READ | FILE_OPTION_WRITE | FILE_OPTION_CREATE:
		// Open the file for reading & writing, create it if it doesn't already exist
		file = fopen( path, "r+b" );
		if ( !file )
			file = fopen( path, "w+b" );
		break;

	case FILE_OPTION_READ | FILE_OPTION_CLEAR:
		// Open the file only if it exists & it can be read, clear it first
		if ( !file_permissions( path, FILE_PERMISSION_READ ) )
			return INVALID_FILE_INDEX;
		file = fopen( path, "w+b" );
		break;

	case FILE_OPTION_WRITE | FILE_OPTION_CLEAR:
		// Open the file only if it exists & it can be written to, clear it first
		if ( !file_permissions( path, FILE_PERMISSION_WRITE ) )
			return INVALID_FILE_INDEX;
		file = fopen( path, "w+b" );
		break;

	case FILE_OPTION_READ | FILE_OPTION_WRITE | FILE_OPTION_CLEAR:
		// Open the file only if it exists & it can be read & written to, clear it first
		if ( !file_permissions( path, FILE_PERMISSION_READ | FILE_PERMISSION_WRITE ) )
			return INVALID_FILE_INDEX;
		file = fopen( path, "w+b" );
		break;

	case FILE_OPTION_READ | FILE_OPTION_CREATE | FILE_OPTION_CLEAR:
		// Open the file for reading, create it if it doesn't already exist, clear it first
		file = fopen( path, "w+b" );
		break;

	case FILE_OPTION_WRITE | FILE_OPTION_CREATE | FILE_OPTION_CLEAR:
		// Open the file for writing, create it if it doesn't already exist, clear it first
		file = fopen( path, "wb" );
		break;

	case FILE_OPTION_READ | FILE_OPTION_WRITE | FILE_OPTION_CREATE | FILE_OPTION_CLEAR:
		// Open the file for reading & writing, create it if it doesn't already exist, clear it first
		file = fopen( path, "w+b" );
		break;

	default:
		log_warning( "Unexpected open_file options combination (%d)", options );
		return INVALID_FILE_INDEX;
	}

	if ( !file )
	{
		log_warning( "Failed to open file: \"%s\"", path );
		return INVALID_FILE_INDEX;
	}

	if ( options & FILE_OPTION_APPEND )
	{
		finternal_seek( file, 0, SEEK_END );
	}

	return reinterpret_cast<u64>( file );
}

void close_file( u64 fileID )
{
	FILE *file = reinterpret_cast<FILE *>( fileID );
	if ( file )
		fclose( file );
}

u64 get_file_size( u64 fileID )
{
	FILE *file = reinterpret_cast<FILE *>( fileID );
	u64 position = finternal_ftell( file );
	finternal_seek( file, 0, SEEK_END );
	u64 size = finternal_ftell( file );
	finternal_seek( file, position, SEEK_SET );
	return size;
}

bool seek_in_file( u64 fileID, FileSeek seek, u64 offset )
{
	FILE *file = reinterpret_cast<FILE *>( fileID );
	if ( !file )
		return false;

	i32 move;

	switch ( seek )
	{
		case FILE_SEEK_START:	move = SEEK_SET;	break;
		case FILE_SEEK_CURRENT:	move = SEEK_CUR;	break;
		case FILE_SEEK_END:		move = SEEK_END;	break;
		default:
			log_warning( "Function received unknown seek: %d", seek );
			return false;
	}

	if ( finternal_seek( file, offset, move ) != 0 )
	{
		log_warning( "Failed to seek pointer in fileID: %llu", fileID );
		return false;
	}

	return true;
}

u8 *read_whole_file( u64 fileID, Allocator *allocator, bool addNullTerminator )
{
	FILE *file = reinterpret_cast<FILE *>( fileID );

	u64 position = finternal_ftell( file );
	finternal_seek( file, 0, SEEK_END );
	u64 fileSize = finternal_ftell( file );

	if ( fileSize == 0 )
	{
		finternal_seek( file, position, SEEK_SET );
		return nullptr;
	}

	u8 *buffer = allocator->allocate<u8>( fileSize + ( addNullTerminator ? 1 : 0 ) );

	finternal_seek( file, 0, SEEK_SET );

	u64 bytesRead = fread( buffer, 1, fileSize, file );

	finternal_seek( file, position, SEEK_SET );

	if ( bytesRead != fileSize )
	{
		allocator->free( buffer );
		return nullptr;
	}

	if ( addNullTerminator )
		buffer[ fileSize ] = '\0';

	return buffer;
}

u64 read_from_file( u64 fileID, void *buffer, u64 size )
{
	FILE *file = reinterpret_cast<FILE *>( fileID );
	if ( !file )
		return 0;

	u64 bytesRead = fread( buffer, 1, size, file );

	if ( bytesRead != size )
	{
		log_warning( "Failed to read from fileID: %llu", fileID );
		return 0;
	}

	return bytesRead;
}

u64 write_to_file( u64 fileID, const void *buffer, u64 size )
{
	FILE *file = reinterpret_cast<FILE *>( fileID );
	if ( !file )
		return 0;

	u64 bytesWritten = fwrite( buffer, 1, size, file );

	if ( bytesWritten != size )
	{
		log_warning( "Failed to write to fileID: %llu", fileID );
		return 0;
	}

	return bytesWritten;
}

void file_flush( u64 fileID )
{
	FILE *file = reinterpret_cast<FILE *>( fileID );
	if ( file )
		fflush( file );
}

[[nodiscard]] u64 file_creation_timestamp( const char *path )
{
	finternal_stat_struct st;
	bool success = finternal_stat( path, &st ) == 0;

	if ( !success )
		return 0;

	return st.st_ctime;
}

[[nodiscard]] u64 file_last_edit_timestamp( const char *path )
{
	finternal_stat_struct st;
	bool success = finternal_stat( path, &st ) == 0;

	if ( !success )
		return 0;

	return st.st_mtime;
}

bool move_file( const char *from, const char *to, FileMove move )
{
	if ( file_exists( to ) )
	{
		if ( !( move & FILE_MOVE_REPLACE ) )
		{
			if ( move & FILE_MOVE_ERROR_LOG )
			{
				log_warning( "Error moving file \"%s\" to \"%s\".", from, to );
			}

			return false;
		}

		if ( !delete_file( to ) )
			return false;
	}

	return finternal_rename( from, to ) == 0;
}

bool move_file_retry( const char *from, const char *to, FileMove move, i32 attempts, i32 msWaitPerAttempt )
{
	if ( file_exists( to ) && !( move & FILE_MOVE_REPLACE ) )
		return false;

	bool errorLog = move & FILE_MOVE_ERROR_LOG;
	move &= ~FILE_MOVE_ERROR_LOG;

	while ( attempts >= 0 && !move_file( from, to, move ) )
	{
		attempts -= 1;
		delay( msWaitPerAttempt );
	}

	if ( errorLog && attempts < 0 )
	{
		log_warning( "Error moving file \"%s\" to \"%s\".", from, to );
	}

	return attempts >= 0;
}

bool copy_file( const char *from, const char *to, FileCopy copy )
{
	FILE *src = fopen( from, "rb" );
	if ( !src )
	{
		if ( copy & FILE_COPY_ERROR_LOG )
			log_warning( "Error opening source file: %s", from );
		return false;
	}

	FILE *dst = fopen( to, "wb" );
	if ( !dst )
	{
		fclose( src );
		if ( copy & FILE_COPY_ERROR_LOG )
			log_warning( "Error opening destination file: %s", to );
		return false;
	}

	char buffer[ 4096 ];
	u64 bytesRead;

	while ( ( bytesRead = fread( buffer, 1, sizeof( buffer ), src ) ) > 0 )
	{
		fwrite( buffer, 1, bytesRead, dst );
		if ( feof( src ) )
			break;
	}

	fclose( src );
	fclose( dst );

	return true;
}

bool copy_file_retry( const char *from, const char *to, FileCopy copy, i32 attempts, i32 msWaitPerAttempt )
{
	bool errorLog = copy & FILE_COPY_ERROR_LOG;
	copy &= ~FILE_COPY_ERROR_LOG;

	while ( attempts >= 0 && !copy_file( from, to, copy ) )
	{
		attempts -= 1;
		delay( msWaitPerAttempt );
	}

	if ( errorLog && attempts < 0 )
	{
		log_warning( "Error copying file \"%s\" to \"%s\".", from, to );
	}

	return attempts >= 0;
}

bool save_file( const char *path, void *data, u64 size, FileHeader *header, Allocator *allocator )
{
	bool result = true;
	u8 *compressedBuffer = nullptr;
	u8 *encodedBuffer = nullptr;
	const char *error = nullptr;

	// Check if it should be encoded
	if ( header->flags & FILE_HEADER_FLAG_BASE64_ENCODED )
	{
		// Calculate how much room will be needed for the encoded data
		u64 encodedSize = base64::encode_bound( size );

		// Create a the destination buffer to fit this encoded size
		compressedBuffer = allocator->allocate<u8>( encodedSize );

		if ( !compressedBuffer )
		{
			log_warning( "Failed to allocate to %llu bytes in memory arena", encodedSize );
			return false;
		}

		// Encode the data
		if ( !base64::encode( compressedBuffer, &encodedSize, static_cast<u8 *>( data ), size, &error ) )
		{
			log_warning( "Failed to encode data. %s", error );
			allocator->free( compressedBuffer );
			return false;
		}

		// Set up data to pass along the chain
		header->decodedSize = size;
		data = compressedBuffer;
		size = encodedSize;
	}

	#ifdef INCLUDE_ZLIB

	// Check if it should be compressed
	if ( result && header->flags & FILE_HEADER_FLAG_COMPRESSED )
	{
		// Calculate the destination buffer to fir the compressed data
		u64 compressedSize = zlib::compress_bound( size );

		// Create a new buffer to fit the
		encodedBuffer = allocator->allocate<u8>( compressedSize );

		if ( !encodedBuffer )
		{
			log_warning( "Failed to allocate to %llu bytes in memory arena", compressedSize );
			allocator->free( compressedBuffer );
			return false;
		}

		if ( zlib::compress( encodedBuffer, &compressedSize, static_cast<u8 *>( data ), size, &error ) != zlib::ZLIB_OK )
		{
			log_warning( "Failed to compress data. %s", error );
			allocator->free( encodedBuffer );
			allocator->free( compressedBuffer );
			return false;
		}

		// Set up data to pass along the chain
		header->uncompressedSize = size;
		data = encodedBuffer;
		size = compressedSize;
	}

	#else

	header->flags &= ~FILE_HEADER_FLAG_COMPRESSED;

	#endif // INCLUDE_ZLIB

	if ( result )
	{
		char tempPath[ PATH_MAX ] = "";
		string_utf8_copy( tempPath, path );
		string_utf8_append( tempPath, ".TEMP" );

		u64 fileID = open_file( tempPath, FILE_OPTION_WRITE | FILE_OPTION_CREATE | FILE_OPTION_CLEAR );

		if ( fileID != INVALID_FILE_INDEX )
		{
			if ( !write_to_file( fileID, header, sizeof( FileHeader ) ) )
			{
				log_warning( "Failed to write header to file: %s", tempPath );
				result = false;
			}

			if ( result && !write_to_file( fileID, data, size ) )
			{
				log_warning( "Failed to write data to file: %s", tempPath );
				result = false;
			}

			close_file( fileID );

			if ( !result )
			{
				// Failed to write the data to the file, delete the file
				delete_file( tempPath );
			}

			// Move the new TEMP file to being the real file
			if ( result && !move_file_retry( tempPath, path, FILE_MOVE_REPLACE ) )
			{
				log_warning( "Failed to rename \"%s\" to \"%s\".", tempPath, path );
				result = false;
			}
		}
		else
		{
			log_warning( "Failed to create/open file: %s", tempPath );
			result = false;
		}
	}

	allocator->free( encodedBuffer );
	allocator->free( compressedBuffer );

	return result;
}

[[nodiscard]] u8 *load_file( const char *path, u64 *fileSize, FileHeader *header, Allocator *allocator )
{
	if ( fileSize )
		*fileSize = 0;

	char tempPath[ PATH_MAX ] = "";
	string_utf8_copy( tempPath, path );
	string_utf8_append( tempPath, ".TEMP" );

	u64 timestampTemp = file_last_edit_timestamp( tempPath );
	u64 timestamp = file_last_edit_timestamp( path );

	u64 readFileSize = 0;
	u8 *file = nullptr;
	FileHeader *fileHeader = nullptr;
	const char *error = nullptr;

	// Check if the temp file has a more recent timestamp
	if ( timestampTemp > timestamp )
	{
		// The temp is a newer file, attempt to read it
		file = read_file( tempPath, &readFileSize, false, allocator );

		if ( file )
		{
			if ( readFileSize >= sizeof( FileHeader ) )
			{
				fileHeader = (FileHeader *)file;

				if ( fileHeader->version != header->version || !has_magic_value( fileHeader->id, header->id ) )
				{
					allocator->free( file );
					file = nullptr;
					fileHeader = nullptr;
				}
			}
			else
			{
				allocator->free( file );
				file = nullptr;
			}
		}
	}

	// Normal file is most recent, or the TEMP file failed to load
	if ( !file )
	{
		file = read_file( path, &readFileSize, false, allocator );

		if ( !file )
		{
			log_warning( "Failed to read file: %s", path );
			return nullptr;
		}

		if ( readFileSize < sizeof( FileHeader ) )
		{
			log_warning( "File read has no data." );
			allocator->free( file );
			return nullptr;
		}

		fileHeader = (FileHeader *)file;

		if ( fileHeader->version != header->version )
		{
			log_warning( "Failed header version." );
			allocator->free( file );
			return nullptr;
		}

		if ( !has_magic_value( fileHeader->id, header->id ) )
		{
			log_warning( "Failed header id." );
			allocator->free( file );
			return nullptr;
		}
	}

	*header = *fileHeader;

	bool result = true;

	u8 *data = file + sizeof( FileHeader );
	u64 size = readFileSize - sizeof( FileHeader );
	u8 *uncompressedBuffer = nullptr;
	u8 *decodedBuffer = nullptr;

	#ifdef INCLUDE_ZLIB

	// Check if the file needs uncompressing
	if ( result && header->flags & FILE_HEADER_FLAG_COMPRESSED )
	{
		u64 uncompressedSize = header->uncompressedSize;

		uncompressedBuffer = allocator->allocate<u8>( uncompressedSize );

		if ( !uncompressedBuffer )
		{
			log_warning( "Failed to allocate to %llu bytes in memory arena", uncompressedSize );
			allocator->free( file );
			return nullptr;
		}

		if ( zlib::uncompress( uncompressedBuffer, &uncompressedSize, data, size, &error ) != zlib::ZLIB_OK )
		{
			log_warning( "Failed to uncompress data. %s", error );
			allocator->free( uncompressedBuffer );
			allocator->free( file );
			return nullptr;
		}

		// Set up data to pass along the chain
		data = uncompressedBuffer;
		size = uncompressedSize;
	}

	#else

	if ( result && header->flags & FILE_HEADER_FLAG_COMPRESSED )
	{
		log_warning( "Cannot handle compressed files without zlib." );
		result = false;
	}

	#endif // INCLUDE_ZLIB

	// Check if the file needs decoding
	if ( result && header->flags & FILE_HEADER_FLAG_BASE64_ENCODED )
	{
		u64 decodedSize = base64::encode_bound( header->decodedSize );

		decodedBuffer = allocator->allocate<u8>( decodedSize );

		if ( !decodedBuffer )
		{
			log_warning( "Failed to allocate to %llu bytes in memory arena", decodedSize );
			allocator->free( uncompressedBuffer );
			allocator->free( file );
			return nullptr;
		}

		if ( !base64::decode( decodedBuffer, &decodedSize, data, size, &error ) )
		{
			log_warning( "Failed to decode data. %s", error );
			allocator->free( decodedBuffer );
			allocator->free( uncompressedBuffer );
			allocator->free( file );
			return nullptr;
		}

		// Set up data to pass along the chain
		data = decodedBuffer;
		size = header->decodedSize;	// take the literal header size, since the decode can allocate extra to align it
	}

	if ( !result )
	{
		allocator->free( decodedBuffer );
		allocator->free( uncompressedBuffer );
		allocator->free( file );
		return nullptr;
	}

	// Attach all memory from data to file together, so if data is free'd
	// everything inbetween data and file is free'd too (including the file allocation)
	allocator->attach( data, file );

	if ( fileSize )
		*fileSize = size;

	return data;
}

#undef finternal_stat_struct
#undef finternal_stat
#undef finternal_fstat
#undef finternal_mkdir
#undef finternal_seek
#undef finternal_ftell
#undef finternal_unlink
#undef finternal_rmdir
#undef finternal_chdir
#undef finternal_getcwd
#undef finternal_rename

#endif // FILE_FUNCTIONS_IMPLEMENTATION