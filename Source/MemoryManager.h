
/*!****************************************************************************
\file     Stub.h
\author   Kenny Mecham
\par      Email: kennethmecham\@comcast.net
\par      Project: Memory Manager
\date     04-21-2020

\brief
  The Stub file, used to create later files in the project

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

void* operator new(size_t size);

void* operator new[](size_t size);

void operator delete(void* ptr);

void operator delete[](void* ptr);

void MemoryManagerInit(void);

void* Alloc(size_t size);
void Delete(void* ptr);

void MemoryManagerShutdown(void);

//-----------------------------------------------------------------------------
// Classes
//-----------------------------------------------------------------------------

