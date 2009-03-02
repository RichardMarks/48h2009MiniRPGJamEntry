
// CODESTYLE: v2.0

// Settings.cpp
// Project: Game Engine Design 101 (UTILITY)
// Author: Richard Marks
// Purpose: A class for loading and saving program settings easily

/**
 * \file Settings.cpp
 * \brief Runtime Configuration - Implementation
 * \author Richard Marks <ccpsceo@gmail.com>
 */

// include the common headers
#include <cstdio>
#include <cstdlib>
#include <cstring>

// include the complementing header
#include "Settings.h"

// include the error reporting header
#include "DebugReport.h"

#include "TimeStamp.h"
#include "StringTokenizer.h"
#include "TextFileParser.h"

namespace UTILITY
{
	namespace CONFIGURATION
	{
		Settings::Settings()
		{
		}

		/**********************************************************************/

		Settings::~Settings()
		{
			Clear();
		}

		/**********************************************************************/

		void Settings::Load(const char* pathName, bool clearBeforeLoad, bool quiet)
		{
			std::vector<std::string> fileLines = PARSING::TextFileParser::Execute(pathName, "#");
			unsigned int lineCount = fileLines.size();

			if (lineCount)
			{
				if (clearBeforeLoad)
				{
					Clear();
				}

				// for each line, parse it
				for (unsigned int index = 0; index < lineCount; index++)
				{
					// tokenize the line based on whitespace
					std::vector<std::string> lineTokens = PARSING::StringTokenizer::Execute(fileLines.at(index), " \t");

					unsigned int lineTokenCount = lineTokens.size();
					// if there are three or more tokens then the line is valid
					if (lineTokenCount >= 3)
					{
						std::string setting 	= lineTokens.at(0);
						std::string eqaulsop 	= lineTokens.at(1);
						std::string value 		= lineTokens.at(2);
						if (lineTokenCount > 3)
						{
							for (unsigned int valueIndex = 3; valueIndex < lineTokenCount; valueIndex++)
							{
								value = value + " " + lineTokens.at(valueIndex);
							}
						}

						// set the setting
						Set(setting.c_str(), value.c_str());
					}
					else
					{
						if (!quiet)
						{
							// do we have the ability to use the ged101 error logger?
							#ifdef LogError
							LogError("Parsing Error: %s on line 04%d: Expected [setting] \"=\" [value]\n", pathName, index + 1);
							#else
							fprintf(stderr, "Parsing Error: %s on line 04%d: Expected [setting] \"=\" [value]\n", pathName, index + 1);
							#endif
						}
					}
				}
			}
		}

		/**********************************************************************/

		void Settings::Save(const char* pathName, bool quiet)
		{
			FILE* fp = fopen(pathName, "w");
			if (!fp)
			{
				if (!quiet)
				{
					// do we have the ability to use the ged101 error logger?
					#ifdef LogError
					LogError("Error: Cannot Open %s for Writing!\n", pathName);
					#else
					fprintf(stderr, "Error: Cannot Open %s for Writing!\n", pathName);
					#endif
				}
			}
			else
			{
				fprintf(fp, "# %s\n", pathName);
				fprintf(fp, "# ged101 Settings Configuration Saved at %s\n", TIME::TimeStamp::Execute().c_str());
				SettingTableIterator iter;

				for (iter = settings_.begin(); iter != settings_.end(); iter++)
				{
					fprintf(fp, "%s\t= %s\n", iter->first.c_str(), iter->second.c_str());
				}

				fclose(fp);
			}
		}

		/**********************************************************************/

		void Settings::Clear()
		{
			settings_.clear();
		}

		/**********************************************************************/

		void Settings::List(FILE* target)
		{
			SettingTableIterator iter;
			for (iter = settings_.begin(); iter != settings_.end(); iter++)
			{
				fprintf(target, "%s\t= \"%s\"\n", iter->first.c_str(), iter->second.c_str());
			}
		}

		/**********************************************************************/

		void Settings::Set(const char* setting, const char* value)
		{
			settings_[setting] = value;
		}

		/**********************************************************************/

		std::string Settings::Get(const char* setting)
		{
			if (settings_.size())
			{
				SettingTableIterator iter;
				if ((iter = settings_.find(setting)) != settings_.end())
				{
					return iter->second.c_str();
				}
			}
			return "";
		}

	} // end namespace
} // end namespace
