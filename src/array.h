
#pragma once

template <typename Type, u64 Capacity>
struct Array
{
	u64 count = 0;
	Type data[ Capacity ];

	inline void add( Type &t )
	{
		assert( count < Capacity );
		data[ count++ ] = t;
	}

	inline void add( const Type &t )
	{
		assert( count < Capacity );
		data[ count++ ] = t;
	}

	inline void add( Type &t, i32 times )
	{
		assert( count + times <= Capacity );
		for ( i32 i = 0; i < times; ++i )
			data[ count++ ] = t;
	}

	inline void add( const Type &t, i32 times )
	{
		assert( count + times <= Capacity );
		for ( i32 i = 0; i < times; ++i )
			data[ count++ ] = t;
	}

	inline void add_no_bounds_check( const Type &t )
	{
		data[ count++ ] = t;
	}

	template <typename ...Args>
	inline void add_no_bounds_check( const Type &t, Args&&... args )
	{
		data[ count++ ] = t;
		add_no_bounds_check( args... );
	}

	template <typename ...Args>
	void add( const Type &t, Args&&... args )
	{
		assert( count + sizeof...( Args ) <= Capacity );
		data[ count++ ] = t;
		add_no_bounds_check( args... );
	}

	bool add_unique( Type &t )
	{
		if ( has_value( t ) )
			return false;
		add( t );
		return true;
	}

	void append( const Type *t, u64 appentCount )
	{
		assert( count + appentCount <= Capacity );

		Type *p = &data[ count ];

		memcpy( &data[ count ], t, appentCount * sizeof( Type ) );

		count += appentCount;
	}

	void append_and_offset( const Type *t, const Type &offset, u64 appentCount )
	{
		assert( count + appentCount <= Capacity );

		Type *p = &data[ count ];

		for ( u64 i = 0; i < appentCount; ++i )
			*p++ = ( *t++ ) + offset;

		count += appentCount;
	}

	template <u64 otherArraySize>
	inline void append( const Array<Type, otherArraySize> &arr )
	{
		append( arr.data, arr.count );
	}

	template <u64 otherArraySize>
	inline void append_and_offset( const Array<Type, otherArraySize> &arr, const Type &offset )
	{
		append_and_offset( arr.data, offset, arr.count );
	}

	template <typename ...Args>
	inline void set( u64 idx, Type &t )
	{
		assert( idx < count );
		data[ idx ] = t;
	}

	template <typename ...Args>
	inline void set_no_bounds_check( u64 idx, Type &t )
	{
		data[ idx ] = t;
	}

	template <typename ...Args>
	inline void set( u64 idx, Type &t, Args&&... args )
	{
		assert( idx + sizeof...( Args ) <= count );
		data[ idx ] = t;
		set_no_bounds_check( idx + 1, args... );
	}

	template <typename ...Args>
	inline void set( u64 idx, const Type &t )
	{
		assert( idx < count );
		data[ idx ] = t;
	}

	template <typename ...Args>
	inline void set_no_bounds_check( u64 idx, const Type &t )
	{
		data[ idx ] = t;
	}

	template <typename ...Args>
	inline void set( u64 idx, const Type &t, const Args&&... args )
	{
		assert( idx + sizeof...( Args ) <= count );
		data[ idx ] = t;
		set_no_bounds_check( idx + 1, args... );
	}

	void set_blank( bool totalCapacity = false )
	{
		memset( this, 0, sizeof( *this ) );

		if ( totalCapacity )
			count = Capacity;
	}

	void set_all_bits_1( bool totalCapacity = false )
	{
		memset( this, 0xFF, sizeof( *this ) );

		if ( totalCapacity )
			count = Capacity;
	}

	void set_all( Type &value, bool totalCapacity )
	{
		if ( totalCapacity )
			count = Capacity;

		for ( u64 i = 0; i < count; ++i )
			data[ i ] = value;
	}

	void set_all( const Type &value, bool totalCapacity )
	{
		if ( totalCapacity )
			count = Capacity;

		for ( u64 i = 0; i < count; ++i )
			data[ i ] = value;
	}

	inline void set_full()
	{
		count = Capacity;
	}

	inline void resize( u64 size )
	{
		assert( size <= Capacity );
		count = size;
	}

	inline void clear()
	{
		count = 0;
	}

	inline void swap_and_remove( u64 idx )
	{
		assert( idx < count );
		data[ idx ] = data[ --count ];
	}

	void remove( u64 idx )
	{
		assert( idx < count );
		// Maintains order by shuffling everything down 1
		memmove( &data[ idx ], &data[ idx + 1 ], ( count - idx - 1 ) * sizeof( Type ) );
		count -= 1;
	}

	void hflip( ivec2 size )
	{
		for ( i32 y = 0; y < size.y; ++y )
		{
			i32 j = y * size.x;
			i32 k = j + size.x - 1;

			for ( i32 x = 0, xCount = size.x / 2; x < xCount; ++x )
			{
				Type temp = data[ j ];
				data[ j++ ] = data[ k ];
				data[ k-- ] = temp;
			}
		}
	}

	void vflip( ivec2 size )
	{
		i32 j = 0;
		i32 k = ( size.x * ( size.y - 1 ) );

		for ( i32 y = 0, yCount = size.y / 2; y < yCount; ++y )
		{
			for ( i32 x = 0; x < size.x; ++x )
			{
				Type temp = data[ j ];
				data[ j++ ] = data[ k ];
				data[ k++ ] = temp;
			}

			k -= size.x * 2;
		}
	}

	[[nodiscard]] inline Type &at( u64 idx )
	{
		assert( idx < count );
		return data[ idx ];
	}

	[[nodiscard]] inline Type &at_no_bounds_check( u64 idx )
	{
		return data[ idx ];
	}

	[[nodiscard]] inline Type & operator[] ( u64 idx )
	{
		assert( idx < count );
		return data[ idx ];
	}

	[[nodiscard]] inline const Type & operator[] ( u64 idx ) const
	{
		assert( idx < count );
		return data[ idx ];
	}

	[[nodiscard]] inline Type *ptr()
	{
		assert( count > 0 );
		return &data[ 0 ];
	}

	[[nodiscard]] inline const Type *ptr() const
	{
		assert( count > 0 );
		return &data[ 0 ];
	}

	[[nodiscard]] inline Type &first()
	{
		assert( count > 0 );
		return data[ 0 ];
	}

	[[nodiscard]] inline const Type &first() const
	{
		assert( count > 0 );
		return data[ 0 ];
	}

	[[nodiscard]] inline Type &last()
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline const Type &last() const
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline Type &top()
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline const Type &top() const
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline Type &back()
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline const Type &back() const
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	inline Type &push()
	{
		assert( count < Capacity );
		return data[ count++ ];
	}

	inline Type &pop()
	{
		assert( count > 0 );
		return data[ --count ];
	}

	inline void pop_back()
	{
		assert( count > 0 );
		count -= 1;
	}

	inline Type &pop_index( u64 idx )
	{
		assert( idx < count );
		Type t = data[ idx ];
		data[ idx ] = data[ --count ];
		data[ count ] = t;
		return data[ count ];
	}

	inline Type &pop_random()
	{
		assert( count > 0 );
		return pop_index( irandom( count - 1 ) );
	}

	[[nodiscard]] inline Type *pop_random_ptr()
	{
		if ( count == 0 )
			return nullptr;
		return &pop_index( irandom( count - 1 ) );
	}

	void shuffle( Allocator *allocator )
	{
		Array<Type, Capacity> *copy = allocator->allocate<Array<Type, Capacity>>( count );
		Array<Type, Capacity> &other = *copy;
		other = *this;
		for ( u64 i = 0; i < count; ++i )
			data[ i ] = other.pop_random();
		allocator->free( copy );
	}

	[[nodiscard]] inline bool has_value( const Type &t ) const
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
			if ( *p++ == t )
				return true;
		return false;
	}

	bool find_and_remove_value_keep_order( const Type &t )
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
		{
			if ( *p++ == t )
			{
				remove( i );
				return true;
			}
		}
		return false;
	}

	bool find_and_remove_value( const Type &t )
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
		{
			if ( *p++ == t )
			{
				swap_and_remove( i );
				return true;
			}
		}
		return false;
	}

	u64 find_and_remove_all_values( const Type &t )
	{
		const Type *p = data;
		u64 startCount = count;
		for ( u64 i = 0; i < count; ++i )
			if ( *p++ == t )
				swap_and_remove( i-- );
		return startCount - count;
	}

	bool find_and_remove_address_keep_order( const Type *t )
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
		{
			if ( p++ == t )
			{
				remove( i );
				return true;
			}
		}
		return false;
	}

	bool find_and_remove_address( const Type *t )
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
		{
			if ( p++ == t )
			{
				swap_and_remove( i );
				return true;
			}
		}
		return false;
	}

	u64 find_and_remove_all_addresses( const Type *t )
	{
		const Type *p = data;
		u64 startCount = count;
		for ( u64 i = 0; i < count; ++i )
			if ( p++ == t )
				swap_and_remove( i-- );
		return startCount - count;
	}

	[[nodiscard]] inline bool empty() const
	{
		return count == 0;
	}

	[[nodiscard]] inline bool full() const
	{
		return count == Capacity;
	}

	[[nodiscard]] constexpr inline u64 capacity() const
	{
		return Capacity;
	}

	[[nodiscard]] constexpr inline u64 available() const
	{
		return Capacity - count;
	}

	[[nodiscard]] inline u64 bytes() const
	{
		return sizeof( Type ) * count;
	}

	inline void swap( u64 entry1, u64 entry2 )
	{
		if ( entry1 == entry2 )
			return;

		assert( entry1 < count );
		assert( entry2 < count );

		Type temp = data[ entry1 ];
		data[ entry1 ] = data[ entry2 ];
		data[ entry2 ] = temp;
	}

	inline void sort()
	{
		if ( count > 0 )
			sort( 0, count - 1 );
	}

	void sort( u64 left, u64 right )
	{
		if ( left >= right )
			return;

		u64 last = left;

		// Pivot goes into the first position (so it isnt moved while going through all the values)
		swap( left, ( left + right ) / 2 );

		// Go through entries left+1 to right, put lower values
		// on the left of the pivot. last is the end of that left array
		// left + 1 because the pivot is stored at left
		for ( u64 i = left + 1; i <= right; ++i )
			if ( compare_value( data[ left ], data[ i ] ) > 0 )
				swap( i, ++last );

		// Put the pivot value to the end of the left-side (all the values are lower than it)
		swap( last, left );

		if ( last > 0 )
			sort( left, last - 1 );
		sort( last + 1, right );
	}
};

template <typename Type>
struct DynamicArray
{
	u64 count = 0;
	u64 currentCapacity = 0;
	Type *data = nullptr;
	Allocator *allocator = nullptr;

	void grow( u64 size )
	{
		data = allocator->reallocate<Type>( data, size );
		if ( !data )
		{
			log_warning( "Failed to grow dynamic array." );
			currentCapacity = 0;
			return;
		}
		currentCapacity = size;
	}

	bool has_volatile_memory() const
	{
		return !allocator || ( allocator->flags & ALLOCATOR_FLAG_VOLATILE_MEMORY );
	}

	void reset()
	{
		count = 0;
		if ( has_volatile_memory() )
		{
			currentCapacity = 0;
			data = nullptr;
		}
	}

	void insert( u64 index, Type *t, u64 insertCount )
	{
		if ( count + insertCount >= currentCapacity )
			grow( insertCount + currentCapacity * 2 );

		assert( count + insertCount <= currentCapacity );
		memmove( &data[ index + insertCount ], &data[ index ], ( count - index ) * sizeof( Type ) );
		memcpy( &data[ index ], t, insertCount * sizeof( Type ) );
		count += insertCount;
	}

	void insert( u64 index, u64 insertCount )
	{
		if ( count + insertCount >= currentCapacity )
			grow( insertCount + currentCapacity * 2 );

		assert( count + insertCount <= currentCapacity );
		memmove( &data[ index + insertCount ], &data[ index ], ( count - index ) * sizeof( Type ) );
		count += insertCount;
	}

	// Note: Care when pasting text and it would be over the current limit
	// as you would need to make sure there is a null terminator. If pasting
	// in the middle, you probably don't want the null terminator
	void paste( u64 index, Type *t, u64 insertCount )
	{
		u64 added = 0;
		if ( count < ( index + insertCount ) )
			added = ( index + insertCount ) - count;

		if ( index + insertCount > currentCapacity )
			grow( insertCount + currentCapacity * 2 );

		assert( index + insertCount <= currentCapacity );
		memcpy( &data[ index ], t, insertCount * sizeof( Type ) );
		count += added;
	}

	void free()
	{
		count = 0;
		currentCapacity = 0;
		allocator->free( data );
		data = nullptr;
	}

	void add( Type &t )
	{
		if ( count >= currentCapacity )
			grow( 1 + currentCapacity * 2 );
		assert( count < currentCapacity );
		data[ count++ ] = t;
	}

	void add( const Type &t )
	{
		if ( count >= currentCapacity )
			grow( 1 + currentCapacity * 2 );
		assert( count < currentCapacity );
		data[ count++ ] = t;
	}

	inline void add_no_bounds_check( const Type &t )
	{
		data[ count++ ] = t;
	}

	template <typename ...Args>
	inline void add_no_bounds_check( const Type &t, Args&&... args )
	{
		data[ count++ ] = t;
		add_no_bounds_check( args... );
	}

	template <typename ...Args>
	void add( const Type &t, Args&&... args )
	{
		if ( count + sizeof...( Args ) >= currentCapacity )
			grow( sizeof...( Args ) + currentCapacity * 2 );
		assert( count + sizeof...( Args ) <= currentCapacity );
		data[ count++ ] = t;
		add_no_bounds_check( args... );
	}

	bool add_unique( Type &t )
	{
		if ( has_value( t ) )
			return false;
		add( t );
		return true;
	}

	void append( const Type *t, u64 appentCount )
	{
		if ( count + appentCount >= currentCapacity )
			grow( appentCount + currentCapacity * 2 );

		assert( count + appentCount <= currentCapacity );

		memcpy( &data[ count ], t, appentCount * sizeof( Type ) );

		count += appentCount;
	}

	void append_and_offset( const Type *t, const Type &offset, u64 appentCount )
	{
		if ( count + appentCount >= currentCapacity )
			grow( appentCount + currentCapacity * 2 );

		assert( count + appentCount <= currentCapacity );

		Type *p = &data[ count ];

		for ( u64 i = 0; i < appentCount; ++i )
			*p++ = ( *t++ ) + offset;

		count += appentCount;
	}

	template <u64 otherArraySize>
	inline void append( const Array<Type, otherArraySize> &arr )
	{
		append( arr.data, arr.count );
	}

	template <u64 otherArraySize>
	inline void append_and_offset( const Array<Type, otherArraySize> &arr, const Type &offset )
	{
		append_and_offset( arr.data, offset, arr.count );
	}

	template <typename ...Args>
	inline void set( u64 idx, Type &t )
	{
		assert( idx < count );
		data[ idx ] = t;
	}

	template <typename ...Args>
	inline void set_no_bounds_check( u64 idx, Type &t )
	{
		data[ idx ] = t;
	}

	template <typename ...Args>
	inline void set( u64 idx, Type &t, Args&&... args )
	{
		assert( idx + sizeof...( Args ) <= count );
		data[ idx ] = t;
		set_no_bounds_check( idx + 1, args... );
	}

	template <typename ...Args>
	inline void set( u64 idx, const Type &t )
	{
		assert( idx < count );
		data[ idx ] = t;
	}

	template <typename ...Args>
	inline void set_no_bounds_check( u64 idx, const Type &t )
	{
		data[ idx ] = t;
	}

	template <typename ...Args>
	inline void set( u64 idx, const Type &t, const Args&&... args )
	{
		assert( idx + sizeof...( Args ) <= count );
		data[ idx ] = t;
		set_no_bounds_check( idx + 1, args... );
	}

	void set_blank( bool totalCapacity = false )
	{
		memset( this, 0, sizeof( *this ) );

		if ( totalCapacity )
			count = currentCapacity;
	}

	void set_all_bits_1( bool totalCapacity = false )
	{
		memset( this, 0xFF, sizeof( *this ) );

		if ( totalCapacity )
			count = currentCapacity;
	}

	void set_all( Type &value, bool totalCapacity )
	{
		if ( totalCapacity )
			count = currentCapacity;

		for ( u64 i = 0; i < count; ++i )
			data[ i ] = value;
	}

	void set_all( const Type &value, bool totalCapacity )
	{
		if ( totalCapacity )
			count = currentCapacity;

		for ( u64 i = 0; i < count; ++i )
			data[ i ] = value;
	}

	inline void set_full()
	{
		count = currentCapacity;
	}

	inline void resize( u64 size )
	{
		assert( size <= currentCapacity );
		count = size;
	}

	void clear()
	{
		count = 0;
	}

	inline void swap_and_remove( u64 idx )
	{
		assert( idx < count );
		data[ idx ] = data[ --count ];
	}

	void remove( u64 idx )
	{
		assert( idx < count );
		// Maintains order by shuffling everything down 1
		memmove( &data[ idx ], &data[ idx + 1 ], count - idx - 1 );
	}

	void hflip( ivec2 size )
	{
		for ( i32 y = 0; y < size.y; ++y )
		{
			i32 j = y * size.x;
			i32 k = j + size.x - 1;

			for ( i32 x = 0, xCount = size.x / 2; x < xCount; ++x )
			{
				Type temp = data[ j ];
				data[ j++ ] = data[ k ];
				data[ k-- ] = temp;
			}
		}
	}

	void vflip( ivec2 size )
	{
		i32 j = 0;
		i32 k = ( size.x * ( size.y - 1 ) );

		for ( i32 y = 0, yCount = size.y / 2; y < yCount; ++y )
		{
			for ( i32 x = 0; x < size.x; ++x )
			{
				Type temp = data[ j ];
				data[ j++ ] = data[ k ];
				data[ k++ ] = temp;
			}

			k -= size.x * 2;
		}
	}

	[[nodiscard]] inline Type &at( u64 idx )
	{
		assert( idx < count );
		return data[ idx ];
	}

	[[nodiscard]] inline Type &at_no_bounds_check( u64 idx )
	{
		return data[ idx ];
	}

	[[nodiscard]] inline Type & operator[] ( u64 idx )
	{
		assert( idx < count );
		return data[ idx ];
	}

	[[nodiscard]] inline const Type & operator[] ( u64 idx ) const
	{
		assert( idx < count );
		return data[ idx ];
	}

	[[nodiscard]] inline Type *ptr()
	{
		assert( count > 0 );
		return &data[ 0 ];
	}

	[[nodiscard]] inline const Type *ptr() const
	{
		assert( count > 0 );
		return &data[ 0 ];
	}

	[[nodiscard]] inline Type &first()
	{
		assert( count > 0 );
		return data[ 0 ];
	}

	[[nodiscard]] inline const Type &first() const
	{
		assert( count > 0 );
		return data[ 0 ];
	}

	[[nodiscard]] inline Type &last()
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline const Type &last() const
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline Type &top()
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline const Type &top() const
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline Type &back()
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	[[nodiscard]] inline const Type &back() const
	{
		assert( count > 0 );
		return data[ count - 1 ];
	}

	inline Type &push()
	{
		assert( count < currentCapacity );
		return data[ count++ ];
	}

	inline Type &pop()
	{
		assert( count > 0 );
		return data[ --count ];
	}

	inline void pop_back()
	{
		assert( count > 0 );
		count -= 1;
	}

	inline Type &pop_index( u64 idx )
	{
		assert( idx < count );
		Type t = data[ idx ];
		data[ idx ] = data[ --count ];
		data[ count ] = t;
		return data[ count ];
	}

	[[nodiscard]] inline Type &pop_random()
	{
		assert( count > 0 );
		return pop_index( irandom( count - 1 ) );
	}

	[[nodiscard]] inline Type *pop_random_ptr()
	{
		if ( count == 0 )
			return nullptr;
		return &pop_index( irandom( count - 1 ) );
	}

	[[nodiscard]] inline bool has_value( const Type &t ) const
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
			if ( *p++ == t )
				return true;
		return false;
	}

	bool find_and_remove_value_keep_order( const Type &t )
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
		{
			if ( *p++ == t )
			{
				remove( i );
				return true;
			}
		}
		return false;
	}

	bool find_and_remove_value( const Type &t )
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
		{
			if ( *p++ == t )
			{
				swap_and_remove( i );
				return true;
			}
		}
		return false;
	}

	u64 find_and_remove_all_values( const Type &t )
	{
		const Type *p = data;
		u64 startCount = count;
		for ( u64 i = 0; i < count; ++i )
			if ( *p++ == t )
				swap_and_remove( i-- );
		return startCount - count;
	}

	bool find_and_remove_address_keep_order( const Type *t )
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
		{
			if ( p++ == t )
			{
				remove( i );
				return true;
			}
		}
		return false;
	}

	bool find_and_remove_address( const Type *t )
	{
		const Type *p = data;
		for ( u64 i = 0; i < count; ++i )
		{
			if ( p++ == t )
			{
				swap_and_remove( i );
				return true;
			}
		}
		return false;
	}

	u64 find_and_remove_all_addresses( const Type *t )
	{
		const Type *p = data;
		u64 startCount = count;
		for ( u64 i = 0; i < count; ++i )
			if ( p++ == t )
				swap_and_remove( i-- );
		return startCount - count;
	}

	[[nodiscard]] inline bool empty() const
	{
		return count == 0;
	}

	[[nodiscard]] inline bool full() const
	{
		return count == currentCapacity;
	}

	[[nodiscard]] constexpr inline u64 capacity() const
	{
		return currentCapacity;
	}

	[[nodiscard]] constexpr inline u64 available() const
	{
		return currentCapacity - count;
	}

	[[nodiscard]] inline u64 bytes() const
	{
		return sizeof( Type ) * count;
	}

	inline void swap( u64 entry1, u64 entry2 )
	{
		if ( entry1 == entry2 )
			return;

		assert( entry1 < count );
		assert( entry2 < count );

		Type temp = data[ entry1 ];
		data[ entry1 ] = data[ entry2 ];
		data[ entry2 ] = temp;
	}

	inline void sort()
	{
		if ( count > 0 )
			sort( 0, count - 1 );
	}

	void sort( u64 left, u64 right )
	{
		if ( left >= right )
			return;

		u64 last = left;

		// Pivot goes into the first position (so it isnt moved while going through all the values)
		swap( left, ( left + right ) / 2 );

		// Go through entries left+1 to right, put lower values
		// on the left of the pivot. last is the end of that left array
		// left + 1 because the pivot is stored at left
		for ( u64 i = left + 1; i <= right; ++i )
			if ( compare_value( data[ left ], data[ i ] ) > 0 )
				swap( i, ++last );

		// Put the pivot value to the end of the left-side (all the values are lower than it)
		swap( last, left );

		if ( last > 0 )
			sort( left, last - 1 );
		sort( last + 1, right );
	}
};