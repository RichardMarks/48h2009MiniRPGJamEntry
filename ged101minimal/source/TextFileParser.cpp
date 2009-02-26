
// CODESTYLE: v2.0

// TextFileParser.cpp
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for reading the contents of a text file into a vector of strings -- accounting for comments

/**
 * \file TextFileParser.cpp
 * \brief File Parser - Implementation
 * \author Richard Marks <ccpsceo@gmail.com>
 */

// include the common headers
#include <cstdio>
#include <cstdlib>
#include <cstring>

// include the complementing header
#include "TextFileParser.h"

// include the error reporting header
#include "DebugReport.h"

namespace UTILITY
{
	namespace PARSING
	{
		std::vector<std::string> TextFileParser::Execute(const char* pathName, const std::string& ignore, bool quiet)
		{
			std::vector<std::string> fileLines;

			FILE* fp = fopen(pathName, "r");

			if (!fp)
			{
				if (!quiet)
				{
					// do we have the ability to use the ged101 error logger?
					#ifdef LogError
					LogError("Error: Cannot Open %s for Reading!\n", pathName);
					#else
					fprintf(stderr, "Error: Cannot Open %s for Reading!\n", pathName);
					#endif
				}
			}
			else
			{
				char buffer[0x1000];
				while(fgets(buffer, sizeof(buffer), fp))
				{
					char ignoreChar = ignore.c_str()[0];
					// ignore comments, and empty lines
					if (ignoreChar == buffer[0])
					{
						// ignored
						if (!quiet)
						{
							// do we have the ability to use the ged101 message logger?
							#ifdef LogMessage
							LogMessage("Ignoring: %s\n", buffer);
							#else
							fprintf(stderr, "Ignoring: %s\n", buffer);
							#endif
						}
					}
					else
					{
						unsigned int length = strlen(buffer);
						bool lineEmpty = true;
						for (unsigned int p = 0; p < length; p++)
						{
							if (
								buffer[p] == ignoreChar ||
								buffer[p] == ' ' ||
								buffer[p] == '\t' ||
								buffer[p] == '\r' ||
								buffer[p] == '\n'
								)
							{
								p = length;
							}
							else
							{
								lineEmpty = false;
								p = length;
							}
						}

						if (!lineEmpty)
						{
							// strip ending newline if its there
							if ('\n' == buffer[length - 1])
							{
								buffer[length - 1] = '\0';
							}
							fileLines.push_back(buffer);
						}
					}
				}

				// close the file
				fclose(fp);
			}

			return fileLines;
		}
	} // end namespace
} // end namespace

