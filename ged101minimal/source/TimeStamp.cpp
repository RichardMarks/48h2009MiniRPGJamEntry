
// CODESTYLE: v2.0

// TimeStamp.cpp
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for getting the system time as a formatted string (wraps strftime)

/**
 * \file TimeStamp.cpp
 * \brief System Time - Implementation
 * \author Richard Marks <ccpsceo@gmail.com>
 */

// include the common headers
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

// include the complementing header
#include "TimeStamp.h"

namespace UTILITY
{
	namespace TIME
	{
		std::string TimeStamp::Execute(const char* timeFormat)
		{
			char buffer[0x1000];
			time_t rawTime;
			time(&rawTime);
			strftime(buffer, 0x1000, timeFormat, localtime(&rawTime));
			return static_cast<std::string>(buffer);
		}
	} // end namespace
} // end namespace

