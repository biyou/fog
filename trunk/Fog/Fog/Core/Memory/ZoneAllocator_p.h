// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_MEMORY_ZONEALLOCATOR_P_H
#define _FOG_CORE_MEMORY_ZONEALLOCATOR_P_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Memory/MemMgr.h>
#include <Fog/Core/Threading/Atomic.h>

namespace Fog {

//! @addtogroup Fog_Core_Memory
//! @{

// ============================================================================
// [Fog::ZoneAllocator]
// ============================================================================

//! @internal
//!
//! @brief Memory allocator designed to fast alloc memory that will be freed
//! in one step (used to alloc temporary objects).
//!
//! This is hackery for performance. Concept is that objects created by
//! @c ZoneAllocator are freed all at once. This means that lifetime of
//! these objects are the same as the lifetime of the @c ZoneAllocator
//! itself. Optionally it's possible to call @c record() and @c revert() methods
//! that can be used to record current allocation position and to revert
//! it back. This is used by clip-span engine to reuse memory used by the
//! clip state that was restored (data not needed anymore).
struct FOG_NO_EXPORT ZoneAllocator
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! @brief Create a new instance of zone allocator.
  //! @param chunkSize Default size for one zone chunk.
  ZoneAllocator(size_t chunkSize);

  //! @brief Destroy the zone allocator instance.
  ~ZoneAllocator();

  // --------------------------------------------------------------------------
  // [Chunk]
  // --------------------------------------------------------------------------

  //! @internal
  //!
  //! @brief One allocated chunk of memory.
  struct Chunk
  {
    //! @brief Link to previous chunk.
    Chunk* prev;
    //! @brief Link to next chunk (optional, used by clip-span allocator).
    Chunk* next;

    //! @brief Current position in this chunk (data + offset).
    uint8_t* pos;
    //! @brief End position (first invalid byte position) of this chunk.
    uint8_t* end;

    //! @brief Data.
    uint8_t data[sizeof(void*)];
  };

  // --------------------------------------------------------------------------
  // [Record]
  // --------------------------------------------------------------------------

  //! @internal
  //!
  //! @brief Zone allocator record, returned by @c record() call.
  struct Record
  {
    //! @brief Current chunk.
    Chunk* current;

    //! @brief Current chunk position pointer (saved).
    uint8_t* pos;
  };

  // --------------------------------------------------------------------------
  // [Methods]
  // --------------------------------------------------------------------------

  //! @brief Allocate @c size bytes of memory and return pointer to it.
  //!
  //! Pointer allocated by this way will be valid until @c ZoneAllocator object
  //! is destroyed. To create class by this way use placement @c new and
  //! @c delete operators:
  //!
  //! @code
  //! // Example of allocating simple class
  //!
  //! // Your class
  //! class Object
  //! {
  //!   // members...
  //! };
  //!
  //! // Your function
  //! void f()
  //! {
  //!   // Create zone object with chunk size of 65536 bytes.
  //!   ZoneAllocator zone(8096);
  //!
  //!   // Create your objects using zone object allocating, for example:
  //!   Object* obj = new(zone.alloc(sizeof(YourClass))) Object();
  //!
  //!   // ... lifetime of your objects ...
  //!
  //!   // Destroy your objects:
  //!   obj->~Object();
  //!
  //!   // ZoneAllocator destructor will free all memory allocated through it,
  //!   // alternative is to call @c zone.free().
  //! }
  //! @endcode
  FOG_INLINE void* alloc(size_t size)
  {
    // Chunks must be valid pointer if we are here.
    FOG_ASSERT(_current != NULL);

    // This allocator wasn't designed to alloc huge amount of memory (larger
    // than the chunk size), so never do it!
    FOG_ASSERT(size < _chunkSize);

    uint8_t* p = _current->pos;
    _current->pos += size;

    if (FOG_UNLIKELY(_current->pos > _current->end))
      return _alloc(size);

    return (void*)p;
  }

  //! @brief Internal alloc method (called by @c alloc() inline).
  //!
  //! There is no reason to inline this method, because the @c alloc() is
  //! sufficient (if we need to call libc malloc then the cost of calling
  //! @c _alloc() is zero).
  void* _alloc(size_t size);

  //! @brief Record current state.
  Record* record();

  //! @brief Revert to state previously recorded by @c record() method.
  void revert(Record* record, bool keepRecord = false);

  //! @brief Invalidate all allocated memory, but do not free allocated memory
  //! chunks.
  //!
  //! This method should be used when one task which needed zone memory ended,
  //! but another needs to be run.
  void reuse();

  //! @brief Free allocated memory.
  void reset();

  // --------------------------------------------------------------------------
  // [Private]
  // --------------------------------------------------------------------------

private:
  FOG_INLINE Chunk* _allocChunk()
  {
    return reinterpret_cast<Chunk*>(
      MemMgr::alloc(sizeof(Chunk) - sizeof(void*) + _chunkSize));
  }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

protected:
  //! @brief Current allocated chunk of memory.
  Chunk* _current;

  //! @brief One chunk size.
  size_t _chunkSize;

  //! @brief First allocated chunk of memory.
  //!
  //! @note This chunk is statically allocated and its length is zero. It
  //! prevents some checks in @c alloc() method and it also prevents from
  //! allocation memory in case that the instance has not been used.
  Chunk _first;

private:
  _FOG_NO_COPY(ZoneAllocator)
};

template<size_t N>
struct FOG_NO_EXPORT ZoneAllocatorT : public ZoneAllocator
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  FOG_INLINE ZoneAllocatorT(size_t chunkSize) : ZoneAllocator(chunkSize)
  {
    _first.end = _first.data + N;
  }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  char _buffer[N];
};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_CORE_MEMORY_ZONEALLOCATOR_P_H