
// CODESTYLE: v2.0

// StringTokenizer.h
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for splitting a string into a vector of strings by any number of delimiters

/**
 * \file StringTokenizer.h
 * \brief String Tokenization - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __STRINGTOKENIZER_H__
#define __STRINGTOKENIZER_H__

#include <string>
#include <vector>

/// Contains Utility Classes
namespace UTILITY
{
	/// Contains classes for file parsing
	namespace PARSING
	{
		/// A class for splitting a string into a vector of strings by any number of delimiters
		class StringTokenizer
		{
		public:
			/// perform the action that this class is designed for
			static std::vector<std::string> Execute(const std::string& source, const std::string& delimiters);

		private:
			/// hidden constructor
			StringTokenizer();

			/// hidden copy constructor
			StringTokenizer(const StringTokenizer& rhs);

			/// hidden assignment operator
			const StringTokenizer& operator=(const StringTokenizer& rhs);
		}; // end class

	} // end namespace

} // end namespace

#endif
