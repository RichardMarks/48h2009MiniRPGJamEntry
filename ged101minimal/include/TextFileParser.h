
// CODESTYLE: v2.0

// TextFileParser.h
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for reading the contents of a text file into a vector of strings -- accounting for comments

/**
 * \file TextFileParser.h
 * \brief File Parser - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __TEXTFILEPARSER_H__
#define __TEXTFILEPARSER_H__

#include <string>
#include <vector>

/// Contains Utility Classes
namespace UTILITY
{
	/// Contains classes for file parsing
	namespace PARSING
	{
		/// A class for reading the contents of a text file into a vector of strings -- accounting for comments
		class TextFileParser
		{
		public:
			/// perform the action that this class is designed for
			static std::vector<std::string> Execute(const char* pathName, const std::string& ignore, bool quiet = true);

		private:
			/// hidden constructor
			TextFileParser();

			/// hidden copy constructor
			TextFileParser(const TextFileParser& rhs);

			/// hidden assignment operator
			const TextFileParser& operator=(const TextFileParser& rhs);
		}; // end class

	} // end namespace

} // end namespace

#endif
