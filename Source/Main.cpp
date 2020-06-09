/*!****************************************************************************
\file     Stub.cpp
\author   Kenny Mecham
\par      Email: kennethmecham\@comcast.net
\par      Project: Memory Manager
\date     05-02-2020

\brief
  The Stub file, used to create later files in the project

******************************************************************************/

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include "MemoryManager.h"
#include <chrono>
#include <iostream>
#include <string>

//-----------------------------------------------------------------------------
// Private Consts
//-----------------------------------------------------------------------------

class Temp
{
  public:
    Temp(std::string&& name);

  private:
    std::string mName;
};

Temp::Temp(std::string&& string) : mName(string)
{
}

Temp temp("Test");

//-----------------------------------------------------------------------------
// Private Classes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Functions
//-----------------------------------------------------------------------------

std::chrono::system_clock::time_point GetTime(void);

void PrintTimeDiff(const std::chrono::system_clock::time_point& startTime, const std::string& testName);

//-----------------------------------------------------------------------------
// Public Functions
//-----------------------------------------------------------------------------

int main(void)
{
  MemoryManagerInit();

  char* buffer[2000];

  int amount;

  auto startTime = GetTime();

  for (int j = 0; j < 300; ++j)
  {
    for (int i = 0; i < 500; ++i)
    {
      amount = (i % 40 + 1) * 40;
      //amount = i;
      buffer[i] = new char[amount];
    }

    for (int i = 0; i < 500; ++i)
    {
      delete [] buffer[i];
    }
  }
  
  PrintTimeDiff(startTime, "MemoryManager (1000 int test)");


  startTime = GetTime();

  for (int j = 0; j < 300; ++j)
  {
    for (int i = 0; i < 500; ++i)
    {
      amount = (i % 40 + 1) * 40;
      //amount = i;
      buffer[i] = new char[amount];
    }

    for (int i = 0; i < 500; ++i)
    {
      delete [] buffer[i];
    }
  }

  PrintTimeDiff(startTime, "new (1000 int test)");

  MemoryManagerShutdown();

  return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Class: Name
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Class Functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Private Class Functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Private Functions
//-----------------------------------------------------------------------------

/*!****************************************************************************
\brief
  A stub file header for use in later files

\param stub
  parameter use in the function

\return
  return of the function
******************************************************************************/
std::chrono::system_clock::time_point GetTime(void)
{
  return std::chrono::system_clock::now();
}

void PrintTimeDiff(const std::chrono::system_clock::time_point& startTime, const std::string& testName)
{
  std::chrono::duration<double> diff = GetTime() - startTime;

  std::cout << "It took " << diff.count() << " time to complete " << testName << std::endl;
}