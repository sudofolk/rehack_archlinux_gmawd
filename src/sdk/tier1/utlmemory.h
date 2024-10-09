#include "tier0/memalloc.h"
#include "tier0/memdbgon.h"

#pragma warning (disable:4100)
#pragma warning (disable:4514)


//-----------------------------------------------------------------------------


#ifdef UTLMEMORY_TRACK
#define UTLMEMORY_TRACK_ALLOC()		MemAlloc_RegisterAllocation( "Sum of all UtlMemory", 0, m_nAllocationCount * sizeof(T), m_nAllocationCount * sizeof(T), 0 )
#define UTLMEMORY_TRACK_FREE()		if ( !m_pMemory ) ; else MemAlloc_RegisterDeallocation( "Sum of all UtlMemory", 0, m_nAllocationCount * sizeof(T), m_nAllocationCount * sizeof(T), 0 )
#else
#define UTLMEMORY_TRACK_ALLOC()		((void)0)
#define UTLMEMORY_TRACK_FREE()		((void)0)
#endif


//-----------------------------------------------------------------------------
// The CUtlMemory class:
// A growable memory class which doubles in size by default.
//-----------------------------------------------------------------------------
template< class T, class I = int >
class CUtlMemory
{
public:
	// constructor, destructor
	CUtlMemory( int nGrowSize = 0, int nInitSize = 0 );
	CUtlMemory( T* pMemory, int numElements );
	CUtlMemory( const T* pMemory, int numElements );
	~CUtlMemory();

	// Set the size by which the memory grows
	void Init( int nGrowSize = 0, int nInitSize = 0 );

	class Iterator_t
	{
	public:
		Iterator_t( I i ) : index( i ) {}
		I index;

		bool operator==( const Iterator_t it ) const	{ return index == it.index; }
		bool operator!=( const Iterator_t it ) const	{ return index != it.index; }
	};
	Iterator_t First() const							{ return Iterator_t( IsIdxValid( 0 ) ? 0 : InvalidIndex() ); }
	Iterator_t Next( const Iterator_t &it ) const		{ return Iterator_t( IsIdxValid( it.index + 1 ) ? it.index + 1 : InvalidIndex() ); }
	I GetIndex( const Iterator_t &it ) const			{ return it.index; }
	bool IsIdxAfter( I i, const Iterator_t &it ) const	{ return i > it.index; }
	bool IsValidIterator( const Iterator_t &it ) const	{ return IsIdxValid( it.index ); }
	Iterator_t InvalidIterator() const					{ return Iterator_t( InvalidIndex() ); }

	// element access
	T& operator[]( I i );
	const T& operator[]( I i ) const;
	T& Element( I i );
	const T& Element( I i ) const;

	// Can we use this index?
	bool IsIdxValid( I i ) const;

	// Specify the invalid ('null') index that we'll only return on failure
	static const I INVALID_INDEX = ( I )-1; // For use with COMPILE_TIME_ASSERT
	static I InvalidIndex() { return INVALID_INDEX; }

	// Gets the base address (can change when adding elements!)
	T* Base();
	const T* Base() const;

	// Attaches the buffer to external memory....
	void SetExternalBuffer( T* pMemory, int numElements );
	void SetExternalBuffer( const T* pMemory, int numElements );
	// Takes ownership of the passed memory, including freeing it when this buffer is destroyed.
	void AssumeMemory( T *pMemory, int nSize );

	// Fast swap
	void Swap( CUtlMemory< T, I > &mem );

	// Switches the buffer from an external memory buffer to a reallocatable buffer
	// Will copy the current contents of the external buffer to the reallocatable buffer
	void ConvertToGrowableMemory( int nGrowSize );

	// Size
	int NumAllocated() const;
	int Count() const;

	// Grows the memory, so that at least allocated + num elements are allocated
	void Grow( int num = 1 );

	// Makes sure we've got at least this much memory
	void EnsureCapacity( int num );

	// Memory deallocation
	void Purge();

	// Purge all but the given number of elements
	void Purge( int numElements );

	// is the memory externally allocated?
	bool IsExternallyAllocated() const;

	// is the memory read only?
	bool IsReadOnly() const;

	// Set the size by which the memory grows
	void SetGrowSize( int size );

protected:
	void ValidateGrowSize()
	{
#ifdef _X360
		if ( m_nGrowSize && m_nGrowSize != EXTERNAL_BUFFER_MARKER )
		{
			// Max grow size at 128 bytes on XBOX
			const int MAX_GROW = 128;
			if ( m_nGrowSize * sizeof(T) > MAX_GROW )
			{
				m_nGrowSize = max( 1, MAX_GROW / sizeof(T) );
			}
		}
#endif
	}

	enum
	{
		EXTERNAL_BUFFER_MARKER = -1,
		EXTERNAL_CONST_BUFFER_MARKER = -2,
	};

	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};


//-----------------------------------------------------------------------------
// The CUtlMemory class:
// A growable memory class which doubles in size by default.
//-----------------------------------------------------------------------------
template< class T, size_t SIZE, class I = int >
class CUtlMemoryFixedGrowable : public CUtlMemory< T, I >
{
	typedef CUtlMemory< T, I > BaseClass;

public:
	CUtlMemoryFixedGrowable( int nGrowSize = 0, int nInitSize = SIZE ) : BaseClass( m_pFixedMemory, SIZE ) 
	{
		Assert( nInitSize == 0 || nInitSize == SIZE );
		m_nMallocGrowSize = nGrowSize;
	}

	void Grow( int nCount = 1 )
	{
		if ( this->IsExternallyAllocated() )
		{
			this->ConvertToGrowableMemory( m_nMallocGrowSize );
		}
		BaseClass::Grow( nCount );
	}

	void EnsureCapacity( int num )
	{
		if ( CUtlMemory<T>::m_nAllocationCount >= num )
			return;

		if ( this->IsExternallyAllocated() )
		{
			// Can't grow a buffer whose memory was externally allocated 
			this->ConvertToGrowableMemory( m_nMallocGrowSize );
		}

		BaseClass::EnsureCapacity( num );
	}

private:
	int m_nMallocGrowSize;
	T m_pFixedMemory[ SIZE ];
};

//-----------------------------------------------------------------------------
// The CUtlMemoryFixed class:
// A fixed memory class
//-----------------------------------------------------------------------------
template< typename T, size_t SIZE, int nAlignment = 0 >
class CUtlMemoryFixed
{
public:
	// constructor, destructor
	CUtlMemoryFixed( int nGrowSize = 0, int nInitSize = 0 )	{ Assert( nInitSize == 0 || nInitSize == SIZE ); 	}
	CUtlMemoryFixed( T* pMemory, int numElements )			{ Assert( 0 ); 										}

	// Can we use this index?
	// Use unsigned math to improve performance
	bool IsIdxValid( int i ) const							{ return (size_t)i < SIZE; }

	// Specify the invalid ('null') index that we'll only return on failure
	static const int INVALID_INDEX = -1; // For use with COMPILE_TIME_ASSERT
	static int InvalidIndex() { return INVALID_INDEX; }

	// Gets the base address
	T* Base()												{ if ( nAlignment == 0 ) return (T*)(&m_Memory[0]); else return (T*)AlignValue( &m_Memory[0], nAlignment ); }
	const T* Base() const									{ if ( nAlignment == 0 ) return (T*)(&m_Memory[0]); else return (T*)AlignValue( &m_Memory[0], nAlignment ); }

	// element access
	// Use unsigned math and inlined checks to improve performance.
	T& operator[]( int i )									{ Assert( (size_t)i < SIZE ); return Base()[i];	}
	const T& operator[]( int i ) const						{ Assert( (size_t)i < SIZE ); return Base()[i];	}
	T& Element( int i )										{ Assert( (size_t)i < SIZE ); return Base()[i];	}
	const T& Element( int i ) const							{ Assert( (size_t)i < SIZE ); return Base()[i];	}

	// Attaches the buffer to external memory....
	void SetExternalBuffer( T* pMemory, int numElements )	{ Assert( 0 ); }

	// Size
	int NumAllocated() const								{ return SIZE; }
	int Count() const										{ return SIZE; }

	// Grows the memory, so that at least allocated + num elements are allocated
	void Grow( int num = 1 )