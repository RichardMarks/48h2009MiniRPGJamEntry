
// CODESTYLE: v2.0

// FileUtilities.cpp
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: classes for file system utilities

/**
 * \file FileUtilities.cpp
 * \brief File System Utilities - Implementation
 * \author Richard Marks <ccpsceo@gmail.com>
 */

// include the common headers
#include <cstdio>
#include <cstdlib>
#include <cstring>

// include the complementing header
#include "FileUtilities.h"

// include the error reporting header
#include "DebugReport.h"

namespace UTILITY
{
	namespace FILESYSTEM
	{
		bool FileExists::Execute(const char* pathName, bool quiet)
		{
			bool result = false;

			FILE* fp = fopen(pathName, "rb");

			if (fp)
			{
				fseek(fp, 0, SEEK_END);

				// we only return true if the file is not 0 bytes
				unsigned long fileSize = ftell(fp);

				if (fileSize)
				{
					result = true;
				}
				else
				{
					if (!quiet)
					{
						// do we have the ability to use the ged101 warning logger?
						#ifdef LogWarning
						LogWarning("%s is %d bytes!", pathName, fileSize);
						#else
						fprintf(stderr, "Warning: %s is %d bytes!\n", pathName, fileSize);
						#endif
					}
				}

				fclose(fp);
			}
			else
			{
				if (!quiet)
				{
					// do we have the ability to use the ged101 error logger?
					#ifdef LogError
					LogError("Error: Cannot Open %s for Reading!", pathName);
					#else
					fprintf(stderr, "Error: Cannot Open %s for Reading!\n", pathName);
					#endif
				}
			}

			return result;
		}
	} // end namespace
} // end namespace
