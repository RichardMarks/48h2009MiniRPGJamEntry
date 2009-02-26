
// CODESTYLE: v2.0

// TimeStamp.h
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for getting the system time as a formatted string (wraps strftime)

/**
 * \file TimeStamp.h
 * \brief System Time - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__

#include <string>

/// Contains Utility Classes
namespace UTILITY
{
	/// Contains classes for dealing with time
	namespace TIME
	{
		/// A class for getting the system time as a formatted string (wraps strftime)
		class TimeStamp
		{
		public:
			/// perform the action that this class is designed for
			static std::string Execute(const char* timeFormat = "%I:%M:%S %p %Z on %m-%d-%Y");

		private:
			/// hidden constructor
			TimeStamp();

			/// hidden copy constructor
			TimeStamp(const TimeStamp& rhs);

			/// hidden assignment operator
			const TimeStamp& operator=(const TimeStamp& rhs);
		}; // end class

	} // end namespace

} // end namespace

#endif
