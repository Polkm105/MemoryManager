/*!****************************************************************************
\file     MemoryManager.cpp
\author   Kenny Mecham
\par      Email: kennethmecham\@comcast.net
\par      Project: Memory Manager
\date     05-02-2020

\brief
  Contains the private memoryManager class and all public wrappers for accessing
  the manager itself

******************************************************************************/

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "MemoryManager.h"
#include "MemoryBlock.h"
#include "MemoryAllocated.h"
#include "MemoryPage.h"
#include "MemoryAllocator.h"
#include <set>
#include <unordered_map>
#include <vector>
#include <new>
#include <utility>
#include <cstddef>

//-----------------------------------------------------------------------------
// Private Consts
//-----------------------------------------------------------------------------

const size_t PAGE_SIZE = 16000;  //!< a page is 16000 bytes in size

#define MANAGER_SIZE_TABLE std::unordered_multimap<size_t, MemoryBlock, std::hash<size_t>, std::equal_to<size_t>, MemoryAllocator<std::pair<const size_t, MemoryBlock> > >

#define MANAGER_LOC_TABLE std::unordered_map<void*, MemoryBlock, std::hash<void*>, std::equal_to<void*>, MemoryAllocator<std::pair<const void*, MemoryBlock>>>

//-----------------------------------------------------------------------------
// Private Classes
//-----------------------------------------------------------------------------

//! The class to be used to keep track of allocations and deallocations
class MemoryManager
{
public:

  MemoryManager(void);

  void* Allocate(size_t memSize);
  void Destroy(void* ptr);

  friend void MemoryManagerInit(void);
  friend void MemoryManagerShutdown(void);

private:

  MemoryBlock mHeap;      //!< the current heap of the manager

  std::vector<MemoryPage, MemoryAllocator<MemoryPage>> mPageVec; //!< a vector of all allocated pages

  MANAGER_SIZE_TABLE mFreeSize; //!< hash table of all free blocks sorted by size, inserted by elementSize * numElements
  MANAGER_LOC_TABLE mFreeLoc;   //!< hash table of all free blocks sorted by location, inserted by MemoryBlock void*


  void* AllocatePage(size_t pageSize = PAGE_SIZE);
  void* AllocateMemoryFromHeap(size_t size);
  void AddPageToFree(void* page);
  void AddBlockToFree(MemoryBlock& block);
  void MoveBlock(MemoryBlock& block, size_t amount, bool right);
  bool GetHeapFromFreeMap(void);
  bool IsInPage(void* ptr, unsigned int pageIndex) const;
  unsigned int PageIndex(void* ptr) const;
};

MemoryManager manager;

//-----------------------------------------------------------------------------
// Private Function Declerations
//-----------------------------------------------------------------------------

MANAGER_SIZE_TABLE::iterator SearchSizeTable(void* ptr, size_t size, MANAGER_SIZE_TABLE& table);
MANAGER_LOC_TABLE::iterator SearchLocTable(void* ptr, MANAGER_LOC_TABLE& table);

//-----------------------------------------------------------------------------
// Public Functions
//-----------------------------------------------------------------------------

/*!****************************************************************************
\brief
  Allocates all pages and initializes the heap of the memory manager
******************************************************************************/
void MemoryManagerInit(void)
{
  // for all pages in the manager
  for (unsigned int i = 0; i < 20; ++i)
  {
    void* page = manager.AllocatePage();  // allocate a new page

    MemoryBlock temp(page, PAGE_SIZE);

    manager.mFreeLoc.insert(std::pair<void*, MemoryBlock>(temp.MemoryLocation(), temp));  // insert new block into free loc table
    manager.mFreeSize.insert(std::pair<size_t, MemoryBlock>(temp.Size(), temp));
  }

  manager.GetHeapFromFreeMap(); // get the heap
}

/*!****************************************************************************
\brief
  frees all dynamic MemoryManager memory
******************************************************************************/
void MemoryManagerShutdown(void)
{
  size_t size = manager.mPageVec.size();
  
  // for all allocated pages in manager
  for (size_t i = 0; i < size; ++i)
  {
    manager.mPageVec[i].Destroy();  // free current page
  }
}

/*!****************************************************************************
\brief
  Allocates a given size in bytes from the memoryManager

\param size
  the number of bytes to allocate

\return
  a pointer to the allocated memory
******************************************************************************/
void* operator new(size_t size)
{
  if (size)
  {
    return manager.Allocate(size);
  }

  return NULL;
}

void* operator new[](size_t size)
{
  if (size)
  {
    return manager.Allocate(size);
  }

  return NULL;
}

/*!****************************************************************************
\brief
  Deletes a given pointer from the memory manager

\param ptr
  the pointer to delete
******************************************************************************/
void operator delete(void* ptr)
{
  if (ptr)
  {
    manager.Destroy(ptr);
  }
}

void operator delete[](void* ptr)
{
  if (ptr)
  {
    manager.Destroy(ptr);
  }
}

//-----------------------------------------------------------------------------
// Private Functions
//-----------------------------------------------------------------------------

/*!****************************************************************************
\brief
  Searches the size table for a block that matches the given parameters

\param ptr
  the pointer of the block to look for

\param size
  the size of the block to look for

\param table
  the table to search for the given information in

\return
  an iterator to the matching block, else returns the end of the size table
******************************************************************************/
MANAGER_SIZE_TABLE::iterator SearchSizeTable(void* ptr, size_t size, MANAGER_SIZE_TABLE& table)
{
  std::pair<MANAGER_SIZE_TABLE::iterator, MANAGER_SIZE_TABLE::iterator> temp = table.equal_range(size);

  // while we are still looking through the iterator
  while (temp.first != temp.second)
  {
    // if the curreny block has the same mem location
    if (temp.first->second.MemoryLocation() == ptr)
    {
      return temp.first;
    }
    else
    {
      ++temp.first; // move to next block in iterator
    }
  }

  return table.end();
}

/*!****************************************************************************
\brief
  Searches the location table for a block that matches the given parameters

\param ptr
  the pointer of the block to look for

\param table
  the table to search for the given pointer in

\return
  an iterator to the matching block, else returns the end of the table
******************************************************************************/
MANAGER_LOC_TABLE::iterator SearchLocTable(void* ptr, MANAGER_LOC_TABLE& table)
{
  return table.find(ptr);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Class: MemoryManager
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Class Functions
//-----------------------------------------------------------------------------

/*!****************************************************************************
\brief
  A default constructor for a memoryManager, starts with 10 pages
******************************************************************************/
MemoryManager::MemoryManager(void) :
  mHeap(),
  mPageVec(0),
  mFreeSize(),
  mFreeLoc()
{
}

/*!****************************************************************************
\brief
  Allocates a given size in bytes

\param memSize
  the number of bytes to be allocated

\return
  a pointer to the memory allocated
******************************************************************************/
void* MemoryManager::Allocate(size_t memSize)
{
  void* mem = NULL;
  MANAGER_SIZE_TABLE::iterator it = mFreeSize.find(memSize);

  // if a matching block was found
  if (it != mFreeSize.end())
  {
    mem = it->second.MemoryLocation(); // hold memory location

    mFreeLoc.erase(it->second.MemoryLocation());  // remove block from loc table
    mFreeSize.erase(it);                          // remove block from size table
  }
  else
  {
    mem = AllocateMemoryFromHeap(memSize);

    // if we could not allocate from the heap
    if (mem == NULL)
    {
      AddBlockToFree(mHeap);

      // if the mem size is smaller than a page
      if (memSize + sizeof(MemoryAllocated) < PAGE_SIZE)
      {
        it = mFreeSize.find(PAGE_SIZE); // search for a free page

          // if a page was found
        if (it != mFreeSize.end())
        {
          mHeap = it->second; // hold page as new heap

          mFreeLoc.erase(mHeap.MemoryLocation()); // erase found heap from free and loc tables
          mFreeSize.erase(it);
        }
        // no new page was found
        else
        {
          mHeap = MemoryBlock(AllocatePage(), PAGE_SIZE); // allocate a new page
        }
      }
      // memSize is larger than a page
      else
      {
        mHeap = MemoryBlock(AllocatePage(memSize), memSize); // allocate a memSize sized page
      }

      mem = AllocateMemoryFromHeap(memSize);  // allocate memory
    }
  }

  return mem;
}

/*!****************************************************************************
\brief
  Destroys a given pointer in the manager

\param ptr
  the address of the MemoryBlock to destroy
******************************************************************************/
void MemoryManager::Destroy(void* ptr)
{
  unsigned int memSize = *(unsigned int*)((uintptr_t)(ptr)-sizeof(MemoryAllocated));
  MemoryBlock temp(ptr, memSize);

  mFreeLoc.insert(std::pair<void*, MemoryBlock>(ptr, temp));
  mFreeSize.insert(std::pair<size_t, MemoryBlock>(memSize, temp));
}


//-----------------------------------------------------------------------------
// Private Class Functions
//-----------------------------------------------------------------------------

bool MemoryManager::IsInPage(void* ptr, unsigned int pageIndex) const
{
  if (pageIndex < mPageVec.size())
  {
    const void* pagePtr = mPageVec[pageIndex].Ptr();

    return ((ptr >= pagePtr) && ((uintptr_t)(ptr) < ((uintptr_t)(pagePtr)+mPageVec[pageIndex].Size())));
  }

  return false;
}

unsigned int MemoryManager::PageIndex(void* ptr) const
{
  ptr;
  return 0;
}

/*!****************************************************************************
\brief
  Allocates memory for a new page and adds the page to the pageVec, if page
  is not allocated, throws a bad_alloc exception and aborts the program

\param pageSize
  the size of the page to allocate in bytes

\return
  a pointer to the user usable memory
******************************************************************************/
void* MemoryManager::AllocatePage(size_t pageSize)
{
  static unsigned int pageIndex = 0;

  void* page = malloc(pageSize + sizeof(MemoryAllocated)); // allocate a page of page_size

    // if page was allocated
  if (page)
  {
    mPageVec.push_back(MemoryPage(page, pageSize)); // add page to back of mPages

    return (void*)((uintptr_t)(page)+sizeof(MemoryAllocated));  // move to user usable memory
  }

  throw std::bad_alloc(); // could not allocate memory
}


/*!****************************************************************************
\brief
  Allocates a given size of memory from the heap, if size is to large, returns
  NULL

\param size
  the size in bytes to allocate from the heap

\return
  a pointer to the memory if heap is big enough, else NULL
******************************************************************************/
void* MemoryManager::AllocateMemoryFromHeap(size_t size)
{
  size_t heapSize = mHeap.Size();                     // the current size of the heap

    // if heap is perfect size or heap can be split after allocation
  if ((heapSize == size) || (heapSize > size + sizeof(MemoryAllocated)))
  {
    void* memory = mHeap.MemoryLocation();                                                            // get the location of the heap memory
    MemoryAllocated* memoryAlloced = (MemoryAllocated*)((uintptr_t)memory - sizeof(MemoryAllocated)); // move memory to alloced info
    *memoryAlloced = MemoryAllocated(size);                                                           // insert alloced info in front of memory

      // if we need a new heap after this allocation
    if (mHeap.Size() - size == 0)
    {
      // if we did not get a heap from the free map
      if (!GetHeapFromFreeMap())
      {
        mHeap = MemoryBlock(AllocatePage(), PAGE_SIZE); // allocate a new page and set the new heap
      }
    }
    // if heap is being split
    else
    {
      MoveBlock(mHeap, (size_t)(size + sizeof(MemoryAllocated)), true); // move the heap to the next block location
    }

    return memory;  // return allocated memory
  }

  return NULL;  // could not allocate from heap
}

/*!****************************************************************************
\brief
  Adds a MemoryBlock with the page information to the free list

\param page
  a pointer to the page memory
******************************************************************************/
void MemoryManager::AddPageToFree(void* page)
{
  page = (void*)((uintptr_t)(page)+sizeof(MemoryAllocated));  // move page pointer to the right
  unsigned int size = PAGE_SIZE - sizeof(MemoryAllocated);      // size of usable memory

  MemoryBlock temp(page, size);                                       // create a page block
  mFreeSize.insert(std::pair<size_t, MemoryBlock>(size, temp)); // add page to size table
  mFreeLoc.insert(std::pair<void*, MemoryBlock>(page, temp));         // add page to loc table
}

/*!****************************************************************************
\brief
  Adds a given MemoryBlock to the Free list
******************************************************************************/
void MemoryManager::AddBlockToFree(MemoryBlock& block)
{
  mFreeSize.insert(std::pair<size_t, MemoryBlock>(block.Size(), block));
  mFreeLoc.insert(std::pair<void*, MemoryBlock>(block.MemoryLocation(), block));
}

/*!****************************************************************************
\brief
  Moves the heap a given amount of bytes in a given direction and adjusts the
  size accordingly

\param amount
  the amount of bytes to move the heap

\param right
  a bool indicating whether to move the heap right or left in memory, if true
  moves right, else moves left
******************************************************************************/
void MemoryManager::MoveBlock(MemoryBlock& block, size_t amount, bool right)
{
  int size = (int)amount;

  // if we are moving heap left
  if (!right)
  {
    size = -size; // flip amount
  }

  block = MemoryBlock((void*)((uintptr_t)(block.MemoryLocation()) + size), block.Size() - amount);
}

/*!****************************************************************************
\brief
  Gets the largest possible block from the free Map

\param amount
  the amount of bytes to move the heap

\param right
  a bool indicating whether to move the heap right or left in memory, if true
  moves right, else moves left
******************************************************************************/
bool MemoryManager::GetHeapFromFreeMap(void)
{
  // if free map has blocks
  if (mFreeSize.size() > 0)
  {
    MANAGER_SIZE_TABLE::iterator it = mFreeSize.find(PAGE_SIZE);  // get the end of the table

    mHeap = it->second;                           // set heap to the found block
    mFreeLoc.erase(it->second.MemoryLocation());  // remove found block from location set
    mFreeSize.erase(it);                          // remove the found block from the multimap

    return true;
  }

  return false;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
