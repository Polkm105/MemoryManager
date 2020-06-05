/*!****************************************************************************
\file     MemoryBlock.cpp
\author   Kenny Mecham
\par      Email: kennethmecham\@comcast.net
\par      Project: Memory Manager
\date     05-02-2020

\brief
  Holds the implementation of all public and private functions of the MemoryBlock
  class

******************************************************************************/

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "MemoryBlock.h"
#include "MemoryAllocated.h"
#include <stdint.h>
#include <cstddef>
#include <iostream>

//-----------------------------------------------------------------------------
// Private Consts
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Private Classes
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Public Functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Class: MemoryBlock
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Class Functions
//-----------------------------------------------------------------------------

/*!****************************************************************************
\brief
  initializes and new memoryblock with the given information

\param location
  the memory address which the stats belong to

\param size
  the size of the block starting at the given address

\param prevSize
  the size of the block the comes directly before this block in memory
******************************************************************************/
MemoryBlock::MemoryBlock(void* location,
                         unsigned int size) : 
                         mMemoryLocation(location),
                         mSize(size)
{
  MemoryAllocated* temp = (MemoryAllocated*)((uintptr_t)(location) - sizeof(MemoryAllocated));
  temp->size = mSize;
}

MemoryBlock::MemoryBlock(const MemoryBlock& block) : 
                         mMemoryLocation(block.MemoryLocation()),
                         mSize(block.Size())
{
}

MemoryBlock::MemoryBlock(void) :
                         mMemoryLocation(NULL),
                         mSize(0)
{

}

/*!****************************************************************************
\brief
  gets the memorylocation held by the block

\return
  a void* to the memory location in the block
******************************************************************************/
void* const MemoryBlock::MemoryLocation(void) const
{
  return mMemoryLocation;
}

/*!****************************************************************************
\brief
  Gets the size of the memory held by the block

\return
  the size of the memory being held by the block
******************************************************************************/
unsigned int MemoryBlock::Size(void) const
{
  return mSize;
}

/*!****************************************************************************
\brief
  Checks if the left MemoryBlock's mMemoryLocation is before the right 
  MemoryBlock's mMemoryLocation in memory

\return
  true if the left memory block is before the right memory block, else false
******************************************************************************/
bool operator<(const MemoryBlock& left, const MemoryBlock& right)
{
  return (uintptr_t)left.mMemoryLocation < (uintptr_t)right.mMemoryLocation;
}

/*!****************************************************************************
\brief
  Checks if the left MemoryBlock's mMemoryLocation is the same as the right
  MemoryBlock's mMemoryLocation

\return
  true if the left and right have the same location, else false
******************************************************************************/
bool operator==(const MemoryBlock& left, const MemoryBlock& right)
{
  if (left.MemoryLocation() == right.MemoryLocation())
  {
    return true;
  }

  return false;
}

MemoryBlock& MemoryBlock::operator=(const MemoryBlock& right)
{
  //std::cout << "MemoryBlock Copy assignement" << std::endl;

  mMemoryLocation = right.MemoryLocation();
  mSize = right.Size();
  return *this;
}

//-----------------------------------------------------------------------------
// Private Class Functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
