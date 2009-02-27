
// CODESTYLE: v2.0

// Settings.h
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for loading and saving program settings easily

/**
 * \file Settings.h
 * \brief Runtime Configuration - Header
 * \author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __UTILITYSETTINGS_H__
#define __UTILITYSETTINGS_H__

#include <string>
#include <map>

/// Contains Utility Classes
namespace UTILITY
{
	/// Contains classes for program configuration
	namespace CONFIGURATION
	{
		/// A class for loading and saving program settings easily
		class Settings
		{
		public:
			/// constructor
			Settings();

			/// destructor
			~Settings();

			/// loads a settings file
			void Load(const char* pathName, bool clearBeforeLoad = true, bool quiet = true);

			/// saves settings to a file
			void Save(const char* pathName, bool quiet = true);

			/// clears all loaded settings
			void Clear();

			/// lists all loaded settings to the specified file pointer
			void List(FILE* target);

			/// sets the value of a setting, adds if it doesn't exist, overrides if it does
			void Set(const char* setting, const char* value);

			/// gets the value of a setting, or a null-string if it was not found
			std::string Get(const char* setting);

		private:
			/// hidden copy constructor
			Settings(const Settings& rhs);

			/// hidden assignment operator
			const Settings& operator=(const Settings& rhs);

		private:

			/// a typedef for string to string mapping
			typedef std::map<std::string, std::string> SettingTable;

			/// a typedef for iterating on a string to string mapping
			typedef std::map<std::string, std::string>::iterator SettingTableIterator;

			/// the settings table
			SettingTable settings_;

		}; // end class

	} // end namespace

} // end namespace
#endif

