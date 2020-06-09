/*!****************************************************************************
\file     MemoryAllocator.h
\author   Kenny Mecham
\par      Email: kennethmecham\@comcast.net
\par      Project: Memory Manager
\date     06-06-2020

\brief
  A custom Memory Allocator used in the Memory Manager so that operator new
  can be overloaded while simultaneously using the STL

******************************************************************************/

#pragma once

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stddef.h>
#include <limits>
#include <utility>
#include <new>

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Consts
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Classes
//-----------------------------------------------------------------------------

template <class T>
class MemoryAllocator
{
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using const_reference = const T&;
    using void_pointer = void*;
    using const_void_pointer = void const*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    MemoryAllocator(void) = default;
    ~MemoryAllocator(void) = default;

    template <class U>
    struct rebind
    {
      using other = MemoryAllocator<U>;
    };

    template <class U>
    MemoryAllocator(const MemoryAllocator<U>& other){}

    pointer allocate(const size_type numObjects) const
    {
      if ((numObjects * sizeof(T)) > static_cast<size_type>(-1))
      {
        throw std::bad_alloc();
      }
      else if (numObjects == 0)
      {
        return NULL;
      }

      pointer temp = static_cast<pointer>(malloc(sizeof(T) * numObjects));

      if (temp == NULL)
      {
        throw std::bad_alloc();
      }

      return temp;
    }

    template <class U>
    pointer allocate(const size_type numObjects, const_void_pointer hint) const
    {
      hint;
      return allocate(numObjects);
    }

    void deallocate(pointer ptr, size_type numObjects = 0) const
    {
      free(ptr);
    }
    
    size_type max_size() const
    {
      return (static_cast<size_type>(-1) / sizeof(size_type));
    }

    template <class... Args>
    void construct(T* const ptr, Args&& ...args) const
    {
      void* const newPtr = static_cast<void*>(ptr);

      new(newPtr)T(std::forward<Args>(args)...);
    }

    void destroy(T* const ptr) const
    {
      ptr->~T();
    }

    bool operator==(const MemoryAllocator& rhs) const
    {
      return true;
    }

};
