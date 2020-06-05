
/*!****************************************************************************
\file     MemoryBlock.h
\author   Kenny Mecham
\par      Email: kennethmecham\@comcast.net
\par      Project: Memory Manager
\date     05-02-2020

\brief
  Declares the MemoryBlock class and all of its data

******************************************************************************/

#pragma once

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

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

//! Keeps track of all data for a given chunck of memory
class MemoryBlock
{
  public:
    
    MemoryBlock(void);
    MemoryBlock(void* location, unsigned int size);
    MemoryBlock(const MemoryBlock& block);
    ~MemoryBlock(void) = default;

    void* const MemoryLocation(void) const;

    unsigned int Size(void) const;

    friend bool operator<(const MemoryBlock& left, const MemoryBlock& right);

    friend bool operator==(const MemoryBlock& left, const MemoryBlock& right);

    MemoryBlock& operator=(const MemoryBlock& right);

  private:

    void* mMemoryLocation;  //!< the address of the memory
    unsigned int mSize;     //!< the number of bytes allocated for the block
};
