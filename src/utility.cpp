
[[nodiscard]] i32 compare_value( const char *lhs, const char *rhs )
{
	return string_utf8_compare_value( lhs, rhs );
}

[[nodiscard]] i32 value_in_multiples( i32 value, i32 multiple )
{
	if ( multiple == 0 )
		return value;

	i32 remainder = value % multiple;
	if ( remainder == 0 )
		return value;

	return value + multiple - remainder;
}

[[nodiscard]] vec3 slerp( vec3 a, vec3 b, f32 t )
{
	f32 dt = clamp( dot( a, b ), -1.f, 1.f );
	f32 theta = acosf( dt ) * t;
	vec3 relative = normal( b - a * dt );

	return a * cosf( theta ) + relative * sinf( theta );
}

[[nodiscard]] vec4 bilinear_interpolation( vec4 tl, vec4 tr, vec4 br, vec4 bl, f32 x, f32 y )
{
	f32 ld = 1.f - x;
	f32 td = 1.f - y;

	return
	(
		tl * ld * td +
		tr *  x * td +
		br *  x *  y +
		bl * ld *  y
	);
}

[[nodiscard]] vec4 bilinear_interpolation( const vec4 v[ 4 ], f32 x, f32 y )
{
	f32 ld = 1.f - x;
	f32 td = 1.f - y;

	return
	(
		v[ 0 ] * ld * td +
		v[ 1 ] *  x * td +
		v[ 2 ] *  x *  y +
		v[ 3 ] * ld *  y
	);
}

i64 wrap( i64 value, i64 minValue, i64 maxValue, i64 mod )
{
	value -= minValue;
	maxValue -= minValue;

	if ( mod > 0 )
	{
		return minValue + ( ( value + mod ) % ( maxValue + 1 ) );
	}
	else if ( mod < 0 )
	{
		mod = -( -mod % ( maxValue + 1 ) );
		value = ( maxValue + 1 ) + value;
		return minValue + ( ( value + mod ) % ( maxValue + 1 ) );
	}

	return value;
}

[[nodiscard]] i32 random_range_wrapped( i32 min, i32 max, i32 wrap )
{
	assert( min >= 0 && min <= wrap );
	assert( max >= 0 && max <= wrap );

	if ( min <= max )
		return irandom_range( min, max );

	const i32 range = ( wrap - min ) + max;
	const i32 value = irandom( range );

	return ( min + value ) % wrap;
}

[[nodiscard]] f32 random_range_wrapped( f32 min, f32 max, f32 wrap )
{
	assert( min >= 0 && min <= wrap );
	assert( max >= 0 && max <= wrap );

	if ( min <= max )
		return random_range( min, max );

	const f32 range = ( wrap - min ) + max;
	const f32 value = random( range );

	return fmodf( min + value, wrap );
}

[[nodiscard]] f32 increment_real( f32 value )
{
	i32 exponent;
	f32 mantissa = frexpf( value, &exponent );
	if ( mantissa == 0 )
		return FLT_MIN;
	mantissa += FLT_EPSILON / 2.0f;
	value = ldexpf( mantissa, exponent );
	return value;
}

[[nodiscard]] f32 decrement_real( f32 value )
{
	i32 exponent;
	f32 mantissa = frexpf( value, &exponent );
	if ( mantissa == 0 )
		return FLT_MIN;
	mantissa -= FLT_EPSILON / 2.0f;
	value = ldexpf( mantissa, exponent );
	return value;
}

[[nodiscard]] f64 increment_real( f64 value )
{
	i32 exponent;
	f64 mantissa = frexp( value, &exponent );
	if ( mantissa == 0 )
		return DBL_MIN;
	mantissa += DBL_EPSILON / 2.0;
	value = ldexp( mantissa, exponent );
	return value;
}

[[nodiscard]] f64 decrement_real( f64 value )
{
	i32 exponent;
	f64 mantissa = frexp( value, &exponent );
	if ( mantissa == 0 )
		return DBL_MIN;
	mantissa -= DBL_EPSILON / 2.0;
	value = ldexp( mantissa, exponent );
	return value;
}

[[nodiscard]] vec3 rgb_to_hsv( vec3 colour )
{
	vec3 hsv;
	f32 min, max, delta;

	min = colour.r < colour.g ? colour.r : colour.g;
	min = min < colour.b ? min : colour.b;

	max = colour.r > colour.g ? colour.r : colour.g;
	max = max > colour.b ? max : colour.b;

	delta = max - min;

	hsv.v = max;												// value

	if ( delta < 0.00001f )
	{
		hsv.s = 0;
		hsv.h = 0;
		return hsv;
	}

	if ( max > 0.0f )
	{
		hsv.s = ( delta / max );								// saturation
	}
	else
	{
		hsv.s = 0.0;
		hsv.h = NAN;
		return hsv;
	}

	if ( colour.r >= max )
	{
		hsv.h = ( colour.g - colour.b ) / delta;				// between yellow & magenta
	}
	else
	{
		if ( colour.g >= max )
			hsv.h = 2.0f + ( colour.b - colour.r ) / delta;		// between cyan & yellow
		else
			hsv.h = 4.0f + ( colour.r - colour.g ) / delta;		// between magenta & cyan
	}

	hsv.h *= 60.0f;												// degrees

	if ( hsv.h < 0.0f )
		hsv.h += 360.0f;

	hsv.h /= 360.0f;

	return hsv;
}

[[nodiscard]] vec3 hsv_to_rgb( vec3 colour )
{
	vec3 rgb;

	if ( colour.s <= 0.0f )
	{
		rgb.r = colour.v;
		rgb.g = colour.v;
		rgb.b = colour.v;
		return rgb;
	}

	colour.h *= 360.0f;
	if ( colour.h < 0.0f || colour.h > 360.0f )
		colour.h = 0.0f;
	colour.h /= 60.0f;

	u32 i = static_cast<u32>( colour.h );
	f32 ff = colour.h - i;
	f32 p = colour.v * ( 1.0f - colour.s );
	f32 q = colour.v * ( 1.0f - ( colour.s * ff ) );
	f32 t = colour.v * ( 1.0f - ( colour.s * ( 1.0f - ff ) ) );

	switch ( i )
	{
	case 0:
		rgb.r = colour.v;
		rgb.g = t;
		rgb.b = p;
		break;

	case 1:
		rgb.r = q;
		rgb.g = colour.v;
		rgb.b = p;
		break;

	case 2:
		rgb.r = p;
		rgb.g = colour.v;
		rgb.b = t;
		break;

	case 3:
		rgb.r = p;
		rgb.g = q;
		rgb.b = colour.v;
		break;

	case 4:
		rgb.r = t;
		rgb.g = p;
		rgb.b = colour.v;
		break;

	default:
		rgb.r = colour.v;
		rgb.g = p;
		rgb.b = q;
		break;
	}

	return rgb;
}

[[nodiscard]] u64 convert_to_u64( const char *input, const char **output )
{
	const char *start = input;

	// remove leading whitespace
	while ( *input == ' ' )
		input += 1;

	// check it's a number or invalid input
	if ( ( *input < '0' || *input > '9' ) && *input != '+' )
	{
		log_warning( "Invalid data. Not an u64." );
		if ( output )
			*output = start;
		return 0;
	}

	u64 base = 0;

	bool overflow = false;

	while ( ( *input >= '0' && *input <= '9' ) )
	{
		overflow = overflow || base > ( SIZE_MAX / 10 ) || ( base == ( SIZE_MAX / 10 ) && ( *input - '0' ) > 7 );

		base = 10 * base + ( *input++ - '0' );
	}

	if ( output )
		*output = input;

	return !overflow ? base : SIZE_MAX;
}

[[nodiscard]] i64 convert_to_i64( const char *input, const char **output )
{
	const char *start = input;

	// remove leading whitespace
	while ( *input == ' ' )
		input += 1;

	// check it's a number or invalid input
	if ( ( *input < '0' || *input > '9' ) && *input != '-' && *input != '+' )
	{
		log_warning( "Invalid data. Not an i64." );
		if ( output )
			*output = start;
		return 0;
	}

	i64 sign = 1;
	i64 base = 0;

	// sign
	if ( *input == '-' || *input == '+' )
		sign = 1 - 2 * ( *input++ == '-' );

	bool overflow = false;

	while ( ( *input >= '0' && *input <= '9' ) )
	{
		overflow = overflow || base > ( INT64_MAX / 10 ) || ( base == ( INT64_MAX / 10 ) && ( *input - '0' ) > 7 );

		base = 10 * base + ( *input++ - '0' );
	}

	if ( output )
		*output = input;

	return !overflow ? ( base * sign ) : ( sign >= 0 ? INT64_MAX : INT64_MIN );
}

[[nodiscard]] f32 convert_to_float( const char *input, const char **output )
{
	const char *start = input;

	// remove leading whitespace
	while ( *input == ' ' )
		input += 1;

	f32 sign = 1.0f;
	f32 base = 0.0f;
	bool overflow = false;
	bool foundPoint = false;
	char c = *input;

	// check it's a number or invalid input
	if ( ( c < '0' || c > '9' ) && c != '.' && c != '-' && c != '+' )
	{
		log_warning( "Invalid data. Not a f32: %s", start );
		if ( output )
			*output = start;
		return 0.f;
	}

	// sign
	if ( c == '-' || c == '+' )
		sign = 1.0f - 2.0f * ( *input++ == '-' );

	// starting . :eg: .5
	if ( *input == '.' )
	{
		input += 1;
		foundPoint = true;
	}

	c = *input++;

	while ( ( c >= '0' && c <= '9' ) || c == '.' )
	{
		if ( c == '.' )
		{
			c = *input++;

			if ( foundPoint )
			{
				log_warning( "Invalid data. Not a f32: %s", start );
				return 0.f;
			}
			foundPoint = true;
			continue;
		}

		if ( foundPoint )
			sign /= 10.0f;

		overflow = overflow || base > ( FLT_MAX / 10.0f ) || ( base == ( FLT_MAX / 10.0f ) && ( c - '0' ) > 7 );

		base = 10.0f * base + ( c - '0' );

		c = *input++;
	}

	if ( output )
		*output = input - 1;

	return !overflow ? ( base * sign ) : ( sign >= 0 ? FLT_MAX : -FLT_MAX );
}

[[nodiscard]] bool convert_to_bool( const char *input, const char **output )
{
	const char *start = input;

	// remove leading whitespace
	while ( *input == ' ' )
		input += 1;

	const char *trueStr = "true";
	const char *falseStr = "false";
	bool value = ( ascii_char_lower( *input ) == *trueStr );
	const char *checkStr = value ? trueStr : falseStr;

	while ( *checkStr )
	{
		// If a character is different, return false, but don't advance output
		if ( ascii_char_lower( *input++ ) != *checkStr++ )
		{
			if ( output )
				*output = start;
			return false;
		}
	}

	// Advance the output if required
	if ( output )
		*output = input;

	return value;
}

[[nodiscard]] bool is_number_floating_point( const char *input )
{
	// remove leading whitespace
	while ( *input == ' ' )
		input += 1;

	// ignore leading +/-
	if ( *input == '-' || *input == '+' )
		input += 1;

	// search for non number
	while ( *input >= '0' && *input <= '9' )
		input += 1;

	// if it ended with a . , its a floating point number
	return *input == '.';
}

[[nodiscard]] vec3 convert_to_vec3( const char *input, const char **output )
{
	// remove leading whitespace
	while ( *input == ' ' )
		input += 1;

	bool isFloatingPointNumber = is_number_floating_point( input );
	f32 f0, f1, f2;

	if ( isFloatingPointNumber )
	{
		f0 = convert_to_float( input, &input );
		f1 = convert_to_float( input, &input );
		f2 = convert_to_float( input, &input );

		while ( *input == ' ' )
			input += 1;
	}
	else
	{
		i32 c0 = convert_to_int( input, &input );

		assert( c0 >= 0 );

		// remove leading whitespace
		while ( *input == ' ' )
			input += 1;

		// check if that was the end of the command
		if ( *input == ']' || *input == '\0' )
		{
			// since it was the end, there are no more reads
			// just a single u32 that represents 8bits per channel ARGB
			u32 col = static_cast<u32>( c0 );

			f0 = static_cast<f32>( ( col >> 16 ) & 0xff ) / 255.f;
			f1 = static_cast<f32>( ( col >>  8 ) & 0xff ) / 255.f;
			f2 = static_cast<f32>( ( col       ) & 0xff ) / 255.f;
		}
		else
		{
			// since it is not the end, should be 3 ints
			f0 = static_cast<f32>( c0 ) / 255.0f;
			f1 = static_cast<f32>( convert_to_int( input, &input ) ) / 255.0f;
			f2 = static_cast<f32>( convert_to_int( input, &input ) ) / 255.0f;

			while ( *input == ' ' )
				input += 1;
		}
	}

	if ( output )
		*output = input;

	return { f0, f1, f2 };
}

[[nodiscard]] vec4 convert_to_vec4( const char *input, const char **output )
{
	// remove leading whitespace
	while ( *input == ' ' )
		input += 1;

	bool isFloatingPointNumber = is_number_floating_point( input );
	f32 f0, f1, f2, f3 = 1.0f;

	if ( isFloatingPointNumber )
	{
		f0 = convert_to_float( input, &input );
		f1 = convert_to_float( input, &input );
		f2 = convert_to_float( input, &input );

		while ( *input == ' ' )
			input += 1;

		if ( *input != ']' )
			f3 = convert_to_float( input, &input );
	}
	else
	{
		i32 c0 = convert_to_int( input, &input );

		assert( c0 >= 0 );

		// remove leading whitespace
		while ( *input == ' ' )
			input += 1;

		// check if that was the end of the command
		if ( *input == ']' || *input == '\0' )
		{
			// since it was the end, there are no mroe reads
			// just a single i32 that represents 8bits per channel ARGB
			u32 col = static_cast<u32>( c0 );

			f3 = static_cast<f32>( ( col >> 24 ) & 0xff ) / 255.f;
			f0 = static_cast<f32>( ( col >> 16 ) & 0xff ) / 255.f;
			f1 = static_cast<f32>( ( col >>  8 ) & 0xff ) / 255.f;
			f2 = static_cast<f32>( ( col       ) & 0xff ) / 255.f;
		}
		else
		{
			// since it is not the end, can be either 3 or 4 ints
			f0 = static_cast<f32>( c0 ) / 255.0f;
			f1 = static_cast<f32>( convert_to_int( input, &input ) ) / 255.0f;
			f2 = static_cast<f32>( convert_to_int( input, &input ) ) / 255.0f;

			while ( *input == ' ' )
				input += 1;

			if ( *input != ']' )
				f3 = static_cast<f32>( convert_to_int( input, &input ) ) / 255.0f;
		}
	}

	if ( output )
		*output = input;

	return { f0, f1, f2, f3 };
}

u8 ascii_to_hex( char c )
{
	if ( c >= '0' && c <= '9' ) return c - '0';
	else if ( c >= 'a' && c <= 'f' ) return 10 + c - 'a';
	else if ( c >= 'A' && c <= 'F' ) return 10 + c - 'A';
	else return 0;
}

/// @desc Convert RGBA or RGB hexstring into RGBA u32
[[nodiscard]] u32 convert_hex_string_to_u32( const char *input, const char **output )
{
	const char *start = input;

	if ( !input )
	{
		log_warning( "Invalid data. Not an hex string." );
		if ( output )
			*output = start;
		return 0;
	}

	// remove leading whitespace
	while ( *input == ' ' )
		input += 1;

	u64 bytes = string_utf8_bytes( input ) - 1;
	if ( bytes < 6 )
	{
		log_warning( "Invalid data. Not an hex string." );
		if ( output )
			*output = start;
		return 0;
	}

	char data[ 8 ] = { 0, 0, 0, 0, 0, 0, 'F', 'F' };
	i32 components;

	for ( components = 0; components < 8; ++components )
	{
		char c = *input++;

		if ( c == '\0' )
			break;

		if ( ( c < '0' || c > '9' ) && ( c < 'a' || c > 'f' ) && ( c < 'A' || c > 'F' ) )
		{
			log_warning( "Invalid data. Not an hex string." );
			if ( output )
				*output = start;
			return 0;
		}

		data[ components ] = c;
	}

	components /= 2;

	if ( components != 3 && components != 4	)
	{
		log_warning( "Invalid data. Not an hex string." );
		if ( output )
			*output = start;
		return 0;
	}

	u32 value = 0;

	for ( i32 i = 0; i < 8; i += 2 )
	{
		value <<= 8;
		value |= ( ascii_to_hex( data[ i ] ) << 4 ) | ascii_to_hex( data[ i + 1 ] );
	}

	if ( output )
		*output = input;

	return value;
}

u64 convert_to_string( char *dest, u64 destSize, u64 value, i32 radix, i32 trailing )
{
	assert( radix <= 256 );
	assert( trailing <= 32 );

	char tmp[ 64 ];
	char *tp = tmp;

	char i;

	while ( value || tp == tmp )
	{
		i = static_cast<char>( value % radix );
		value /= radix;

		if ( i < 10 )
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}

	u64 len = tp - tmp;

	// Check if trailing 0's are needed
	while ( len < trailing )
	{
		*tp++ = '0';
		len += 1;
	}

	assert( destSize + 1 >= len );

	while ( tp > tmp )
		*dest++ = *--tp;

	*dest = '\0';

	// Digits of string
	return len;
}

u64 convert_to_string( char *dest, u64 destSize, i64 value, i32 radix, i32 trailing )
{
	assert( radix <= 256 );
	assert( trailing <= 32 );

	char tmp[ 64 ];
	char *tp = tmp;

	char i;
	u64 v;

	i64 sign = ( radix == 10 && value < 0 );
	if ( sign )
		v = -value;
	else
		v = static_cast<u64>( value );

	while ( v || tp == tmp )
	{
		i = static_cast<char>( v % radix );
		v /= radix;

		if ( i < 10 )
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}

	u64 len = tp - tmp;

	// Check if trailing 0's are needed
	while ( len < trailing )
	{
		*tp++ = '0';
		len += 1;
	}

	if ( sign )
	{
		*dest++ = '-';
		len += 1;
	}

	assert( destSize + 1 >= len );

	while ( tp > tmp )
		*dest++ = *--tp;

	*dest = '\0';

	// Digits of string
	return len;
}

u64 convert_to_string( char *dest, u64 destSize, f32 value, i32 fracDigits, bool clipZeroFrac )
{
	char *start = dest;
	char *point = nullptr;
	i32 integer = static_cast<i32>( value );
	f32 fraction = value - static_cast<f32>( integer );

	u64 size = convert_to_string( dest, destSize, integer, 10 );

	dest += size;

	if ( fracDigits != 0 )
	{
		point = dest;
		*dest++ = '.';

		i32 fd = abs( fracDigits );

		u64 fracSize = convert_to_string( dest, destSize - ( size + 1 ), static_cast<i32>( fraction * pow( 10, fd ) ), 10, fd );
		dest += fracSize;

		// Negative fracDigits means to cut trailing zeros (2.0000000 > 2.0)
		if ( fracDigits < 0 )
		{
			dest -= 1;
			while ( *dest == '0' )
				dest -= 1;
			if ( *dest == '.' )
				dest += 1;
			*++dest = '\0';
		}
	}

	if ( clipZeroFrac && point )
	{
		char *ptr = dest - 1;

		if ( *ptr == '0' )
		{
			// starting at the right, move backwards removing any zeros.
			// if the decimal point is reached, remove that too.
			while ( *ptr == '0' )
				ptr -= 1;

			if ( *ptr == '.' )
				ptr -= 1;

			ptr[ 1 ] = '\0';

			dest = ptr + 1;
		}
	}

	return dest - start;
}

u64 convert_to_string( char *dest, u64 destSize, bool value )
{
	return string_utf8_copy( dest, destSize, value ? "true" : "false" );
}

u64 convert_to_string( char *dest, u64 destSize, ivec3 value )
{
	u64 size;
	size = convert_to_string( dest, destSize, value.x );
	assert( destSize > size );
	dest[ size ] = ' ';
	size += 1;
	size += convert_to_string( dest + size, destSize - size, value.y );
	assert( destSize > size );
	dest[ size ] = ' ';
	size += 1;
	size += convert_to_string( dest + size, destSize - size, value.z );
	return size;
}

u64 convert_to_hex_string( char *dest, u64 destSize, vec3 colour, bool uppercase )
{
	assert( destSize >= 7 );

	constexpr const char digitsUppercase[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	constexpr const char digitsLowercase[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	u8 r = static_cast<u8>( colour.r * 255.0f );
	u8 g = static_cast<u8>( colour.g * 255.0f );
	u8 b = static_cast<u8>( colour.b * 255.0f );

	if ( uppercase )
	{
		dest[ 0 ] = digitsUppercase[ r >> 4 ];
		dest[ 1 ] = digitsUppercase[ r & 0xF ];
		dest[ 2 ] = digitsUppercase[ g >> 4 ];
		dest[ 3 ] = digitsUppercase[ g & 0xF ];
		dest[ 4 ] = digitsUppercase[ b >> 4 ];
		dest[ 5 ] = digitsUppercase[ b & 0xF ];
		dest[ 6 ] = '\0';
		return 6;
	}

	dest[ 0 ] = digitsLowercase[ r >> 4 ];
	dest[ 1 ] = digitsLowercase[ r & 0xF ];
	dest[ 2 ] = digitsLowercase[ g >> 4 ];
	dest[ 3 ] = digitsLowercase[ g & 0xF ];
	dest[ 4 ] = digitsLowercase[ b >> 4 ];
	dest[ 5 ] = digitsLowercase[ b & 0xF ];
	dest[ 6 ] = '\0';
	return 6;
}

u64 convert_to_hex_string( char *dest, u64 destSize, vec4 colour, bool uppercase )
{
	assert( destSize >= 9 );
	convert_to_hex_string( dest, destSize, to_vec3( colour ) );

	constexpr const char digitsUppercase[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	constexpr const char digitsLowercase[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	u8 a = static_cast<u8>( colour.a * 255.0f );

	if ( uppercase )
	{
		dest[ 6 ] = digitsUppercase[ a >> 4 ];
		dest[ 7 ] = digitsUppercase[ a & 0xF ];
		dest[ 8 ] = '\0';
		return 8;
	}

	dest[ 6 ] = digitsLowercase[ a >> 4 ];
	dest[ 7 ] = digitsLowercase[ a & 0xF ];
	dest[ 8 ] = '\0';
	return 8;
}

[[nodiscard]] const char *convert_to_string( u8 value, i32 radix, i32 trailing )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, radix, trailing );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( u16 value, i32 radix, i32 trailing )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, radix, trailing );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( u32 value, i32 radix, i32 trailing )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, radix, trailing );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( u64 value, i32 radix, i32 trailing )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, radix, trailing );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( i8 value, i32 radix, i32 trailing )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, radix, trailing );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( i16 value, i32 radix, i32 trailing )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, radix, trailing );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( i32 value, i32 radix, i32 trailing )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, radix, trailing );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( i64 value, i32 radix, i32 trailing )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, radix, trailing );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( f32 value, i32 fracDigits, bool clipZeroFrac )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, fracDigits, clipZeroFrac );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_string( bool value )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_string( text, MAX_CONVERT_TO_STRING_DIGITS, value );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_hex_string( vec3 value, bool uppercase )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_hex_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, uppercase );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

[[nodiscard]] const char *convert_to_hex_string( vec4 value, bool uppercase )
{
	Allocator *allocator = &platform->memory.transient;
	char *text = allocator->allocate<char>( MAX_CONVERT_TO_STRING_DIGITS );
	convert_to_hex_string( text, MAX_CONVERT_TO_STRING_DIGITS, value, uppercase );
	allocator->shrink( text, string_utf8_bytes( text ) );
	return text;
}

#ifdef INCLUDE_ZLIB

namespace zlib
{
	i32 compress( u8 *dest, u64 *destSize, const u8 *source, u64 sourceSize, const char **error )
	{
		if ( !dest )
		{
			if ( error )
				*error = "zlib::compress : [dest is nullptr]";
			return ZLIB_INPUT_ERROR;
		}

		if ( !destSize )
		{
			if ( error )
				*error = "zlib::compress : [destSize is nullptr]";
			return ZLIB_INPUT_ERROR;
		}

		if ( !source )
		{
			if ( error )
				*error = "zlib::compress : [source is nullptr]";
			return ZLIB_INPUT_ERROR;
		}

		if ( !sourceSize )
		{
			if ( error )
				*error = "zlib::compress : [sourceSize is 0]";
			return ZLIB_INPUT_ERROR;
		}

		uLongf _destSize = static_cast<uLong>( *destSize );

		i32 result = ::compress( dest, &_destSize, source, static_cast<uLong>( sourceSize ) );

		*destSize = _destSize;

		if ( error )
		{
			switch ( result )
			{
			case Z_OK:             *error = ""; result = ZLIB_OK;           break;
			case Z_STREAM_END:     *error = ""; result = ZLIB_STREAM_END;   break;
			case Z_NEED_DICT:      *error = ""; result = ZLIB_NEED_DICT;    break;

				// errors
			case Z_STREAM_ERROR:   *error = "zlib::compress : [ stream error]"; result = ZLIB_STREAM_ERROR;     break;
			case Z_DATA_ERROR:     *error = "zlib::compress : [   data error]"; result = ZLIB_DATA_ERROR;       break;
			case Z_MEM_ERROR:      *error = "zlib::compress : [ memory error]"; result = ZLIB_MEM_ERROR;        break;
			case Z_BUF_ERROR:      *error = "zlib::compress : [ buffer error]"; result = ZLIB_BUF_ERROR;        break;
			case Z_VERSION_ERROR:  *error = "zlib::compress : [version error]"; result = ZLIB_VERSION_ERROR;    break;
			default:               *error = "zlib::compress : [unknown error]"; result = ZLIB_UNKNOWN;          break;
			}
		}

		return result;
	}

	i32 uncompress( u8 *dest, u64 *destSize, const u8 *source, u64 sourceSize, const char **error )
	{
		if ( !dest )
		{
			if ( error )
				*error = "zlib::uncompress : [dest is nullptr]";
			return ZLIB_INPUT_ERROR;
		}

		if ( !destSize )
		{
			if ( error )
				*error = "zlib::uncompress : [destSize is nullptr]";
			return ZLIB_INPUT_ERROR;
		}

		if ( !source )
		{
			if ( error )
				*error = "zlib::uncompress : [source is nullptr]";
			return ZLIB_INPUT_ERROR;
		}

		if ( !sourceSize )
		{
			if ( error )
				*error = "zlib::uncompress : [sourceSize is 0]";
			return ZLIB_INPUT_ERROR;
		}

		uLongf _destSize = static_cast<uLong>( *destSize );

		i32 result = ::uncompress( dest, &_destSize, source, static_cast<uLong>( sourceSize ) );

		*destSize = _destSize;

		if ( error )
		{
			switch ( result )
			{
			case Z_OK:             *error = ""; result = ZLIB_OK;           break;
			case Z_STREAM_END:     *error = ""; result = ZLIB_STREAM_END;   break;
			case Z_NEED_DICT:      *error = ""; result = ZLIB_NEED_DICT;    break;

				// errors
			case Z_STREAM_ERROR:   *error = "zlib::uncompress : [ stream error]"; result = ZLIB_STREAM_ERROR;   break;
			case Z_DATA_ERROR:     *error = "zlib::uncompress : [   data error]"; result = ZLIB_DATA_ERROR;     break;
			case Z_MEM_ERROR:      *error = "zlib::uncompress : [ memory error]"; result = ZLIB_MEM_ERROR;      break;
			case Z_BUF_ERROR:      *error = "zlib::uncompress : [ buffer error]"; result = ZLIB_BUF_ERROR;      break;
			case Z_VERSION_ERROR:  *error = "zlib::uncompress : [version error]"; result = ZLIB_VERSION_ERROR;  break;
			default:               *error = "zlib::uncompress : [unknown error]"; result = ZLIB_UNKNOWN;        break;
			}
		}

		return result;
	}

	[[nodiscard]] u32 compress_bound( u64 size )
	{
		return static_cast<u32>( ::compressBound( static_cast<uLong>( size ) ) );
	}
}

#endif // INCLUDE_ZLIB

namespace base64
{
	bool encode( u8 *dest, u64 *destSize, const u8 *source, u64 sourceSize, const char **error )
	{
		if ( !dest )
		{
			if ( error )
				*error = "base64::encode : [dest is nullptr]";
			return false;
		}

		if ( !destSize )
		{
			if ( error )
				*error = "base64::encode : [destSize is nullptr]";
			return false;
		}

		if ( !source )
		{
			if ( error )
				*error = "base64::encode : [source is nullptr]";
			return false;
		}

		if ( !sourceSize )
		{
			if ( error )
				*error = "base64::encode : [sourceSize is 0]";
			return false;
		}

		static constexpr const char *toBase64 =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789+/";

		u64 missing = 0;
		u64 reqSize = sourceSize;

		while ( ( reqSize % 3 ) != 0 )
		{
			reqSize += 1;
			missing += 1;
		}

		reqSize = 4 * reqSize / 3;

		// make sure there is enough room in the dest buffer
		if ( *destSize < reqSize )
		{
			if ( error )
				*error = "base64::encode : [Required memory size is greater than the supplied buffer size.]";
			return false;
		}

		*destSize = reqSize;

		for ( u64 i = 0, count = ( reqSize / 4 ) * 3; i < count; i += 3 )
		{
			u8 byte0 = ( i + 0 < sourceSize ) ? source[ i + 0 ] : 0;
			u8 byte1 = ( i + 1 < sourceSize ) ? source[ i + 1 ] : 0;
			u8 byte2 = ( i + 2 < sourceSize ) ? source[ i + 2 ] : 0;

			*dest++ = toBase64[ ( ( byte0 & 0xfc ) >> 2 ) ];
			*dest++ = toBase64[ ( ( byte0 & 0x03 ) << 4 ) + ( ( byte1 & 0xf0 ) >> 4 ) ];
			*dest++ = toBase64[ ( ( byte1 & 0x0f ) << 2 ) + ( ( byte2 & 0xc0 ) >> 6 ) ];
			*dest++ = toBase64[ ( ( byte2 & 0x3f ) << 0 ) ];
		}

		for ( u64 i = 0; i < missing; ++i )
			*dest++ = '=';

		return true;
	}

	bool decode( u8 *dest, u64 *destSize, const u8 *source, u64 sourceSize, const char **error )
	{
		if ( !dest )
		{
			if ( error )
				*error = "base64::decode : [dest is nullptr]";
			return false;
		}

		if ( !destSize )
		{
			if ( error )
				*error = "base64::decode : [destSize is nullptr]";
			return false;
		}

		if ( !source )
		{
			if ( error )
				*error = "base64::decode : [source is nullptr]";
			return false;
		}

		if ( !sourceSize )
		{
			if ( error )
				*error = "base64::decode : [sourceSize is 0]";
			return false;
		}

		static constexpr const u8 fromBase64[] =
		{
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255,  62, 255,  63,
			 52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,   0, 255, 255, 255,
			255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
			 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255,  63,
			255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
			 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255
		};

		if ( sourceSize % 4 != 0 )
		{
			if ( error )
				*error = "base64::decode : [sourceSize is not divisble by 4]";
			return false;
		}

		// make sure there is enough room in the dest buffer
		if ( *destSize < ( 3 * sourceSize / 4 ) )
		{
			if ( error )
				*error = "base64::decode : [Required memory size is greater than the supplied buffer size.]";
			return false;
		}

		u8 *start = dest;

		for ( u64 i = 0; i < sourceSize; i += 4 )
		{
			u8 byte0 = ( source[ i + 0 ] <= 'z' ) ? fromBase64[ source[ i + 0 ] ] : 0xff;
			u8 byte1 = ( source[ i + 1 ] <= 'z' ) ? fromBase64[ source[ i + 1 ] ] : 0xff;
			u8 byte2 = ( source[ i + 2 ] <= 'z' ) ? fromBase64[ source[ i + 2 ] ] : 0xff;
			u8 byte3 = ( source[ i + 3 ] <= 'z' ) ? fromBase64[ source[ i + 3 ] ] : 0xff;

			if ( byte1 != 0xff )
				*dest++ = static_cast<u8>( ( ( byte0 & 0x3f ) << 2 ) + ( ( byte1 & 0x30 ) >> 4 ) );

			if ( byte2 != 0xff )
				*dest++ = static_cast<u8>( ( ( byte1 & 0x0f ) << 4 ) + ( ( byte2 & 0x3c ) >> 2 ) );

			if ( byte3 != 0xff )
				*dest++ = static_cast<u8>( ( ( byte2 & 0x03 ) << 6 ) + ( ( byte3 & 0x3f ) >> 0 ) );
		}

		*destSize = ( dest - start );

		return true;
	}

	[[nodiscard]] u64 encode_bound( u64 size )
	{
		u64 reqSize = size;
		while ( ( reqSize % 3 ) != 0 )
			reqSize += 1;
		return 4 * reqSize / 3;
	}
}

void delay( i32 msWait )
{
	platform_delay( msWait );
}