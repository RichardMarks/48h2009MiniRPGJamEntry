
// CODESTYLE: v2.0

// StringTokenizer.cpp
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for splitting a string into a vector of strings by any number of delimiters

/**
 * \file StringTokenizer.cpp
 * \brief String Tokenization - Implementation
 * \author Richard Marks <ccpsceo@gmail.com>
 */

// include the common headers
#include <cstdio>
#include <cstdlib>
#include <cstring>

// include the complementing header
#include "StringTokenizer.h"

// include the error reporting header
#include "DebugReport.h"

namespace UTILITY
{
	namespace PARSING
	{
		std::vector<std::string> StringTokenizer::Execute(const std::string& source, const std::string& delimiters)
		{
			std::vector<std::string> tokens;

			size_t p0 = 0;
			size_t p1 = std::string::npos;

			while (p0 != std::string::npos)
			{
				p1 = source.find_first_of(delimiters, p0);
				if (p1 != p0)
				{
					std::string token = source.substr(p0, p1 - p0);
					tokens.push_back(token);
				}
				p0 = source.find_first_not_of(delimiters, p1);
			}

			return tokens;
		}
	} // end namespace
} // end namespace

