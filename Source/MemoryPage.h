
/*!****************************************************************************
\file     MemoryPage.h
\author   Kenny Mecham
\par      Email: kennethmecham\@comcast.net
\par      Project: Memory Manager
\date     05-02-2020

\brief
  Holds the MemoryPage class and all functions needed to interact with it

******************************************************************************/

#pragma once

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <set>
#include "MemoryBlock.h"

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

class MemoryPage
{
  public:
    
    MemoryPage(void);
    MemoryPage(void* ptr, size_t size);

    ~MemoryPage() = default;

    unsigned int Size(void) const;
    const void* Ptr(void) const;

    void Destroy(void);

  private:
    
    void* mPtr;   // pointer to the page memory
    size_t mSize; // size of the page in bytes
};
