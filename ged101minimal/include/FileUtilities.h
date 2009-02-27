
// CODESTYLE: v2.0

// FileUtilities.h
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: classes for file system utilities

/**
 * \file FileUtilities.h
 * \brief File System Utilities - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __FILEUTILITIES_H__
#define __FILEUTILITIES_H__


/// Contains Utility Classes
namespace UTILITY
{
	/// Contains classes for the file system
	namespace FILESYSTEM
	{
		/// A class for checking if a file exists
		class FileExists
		{
		public:
			/// perform the action that this class is designed for
			static bool Execute(const char* pathName, bool quiet = true);

		private:
			/// hidden constructor
			FileExists();

			/// hidden copy constructor
			FileExists(const FileExists& rhs);

			/// hidden assignment operator
			const FileExists& operator=(const FileExists& rhs);
		}; // end class
	} // end namespace
} // end namespace

#endif
