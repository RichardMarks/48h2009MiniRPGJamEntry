
// CODESTYLE: v2.0

// GameBattle_BattleLoader.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: Implementation of the Battle System Definition File Loader method for the BattleSingleton class

#include "GameLibrary.h"

#define _BATTLE_LOADER_VERBOSE

namespace GAME
{
	void BattleEngineSingleton::LoadBattleFile(const char* battlePath)
	{
		FILE* fp = fopen(battlePath, "r");
		if (!fp) { LogFatal("Cannot load battle configuration: %s!", battlePath); return; }

		if (0 != battleSprites_) { delete battleSprites_; }
		if (0 != battleScenes_) { delete battleScenes_; }

		battleSprites_ = new GameBattleSpriteManager();
		battleScenes_ = new GameBattleSceneManager();

		std::string spritesDirectory = "";
		std::string scenesDirectory = "";

		// read each line into a vector of std strings
		std::vector<std::string> fileLines;

		char buffer[0x1000];
		while(fgets(buffer, sizeof(buffer), fp))
		{
			// ignore comments, and empty lines
			if ('#' == buffer[0])
			{
				// ignored
			}
			else
			{
				unsigned int length = strlen(buffer);
				bool lineEmpty = true;
				for (unsigned int p = 0; p < length; p++)
				{
					switch(buffer[p])
					{
						case ' ': 	{ } break;
						case '\t': 	{ } break;
						case '\n': 	{ } break;
						case '\r': 	{ } break;
						case '#': 	{ p = length; } break;
						default: 	{ lineEmpty = false; p = length; } break;
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

		// for each line, parse it
		unsigned int lineCount = fileLines.size();

#if defined(_BATTLE_LOADER_VERBOSE)
		fprintf(stderr, "There are %d lines to parse in %s\n", lineCount, battlePath);
#endif


		for (unsigned int index = 0; index < lineCount; index++)
		{
			std::string& line = fileLines.at(index);
			std::vector<std::string> tokens = GameSingleton::GetInstance()->Tokenize(line, " \t");
			unsigned int tokenCount = tokens.size();

			if (0x2 == tokenCount)
			{
				std::string& commandToken = tokens.at(0);
				std::string& paramsToken = tokens.at(1);

				// # SCENESDIR [path]
				if ("SCENESDIR" == commandToken)
				{
#if defined(_BATTLE_LOADER_VERBOSE)
					fprintf(stderr, "\tScene Images are expected to be stored in %s\n\n", paramsToken.c_str());
#endif
				}

/*#**************************************************************************#*/

				// # SPRITESDIR [path]
				else if ("SPRITESDIR" == commandToken)
				{
#if defined(_BATTLE_LOADER_VERBOSE)
					fprintf(stderr, "\tSprite Images are expected to be stored in %s\n\n", paramsToken.c_str());
#endif
				}

/*#**************************************************************************#*/

				// # SPRITE [file]
				else if ("SPRITE" == commandToken)
				{
#if defined(_BATTLE_LOADER_VERBOSE)
					fprintf(stderr,
						"\tSprite %d\n"
						"\t\tRequires an image file called %s\n\n",
						0, "<undefined>");
#endif
					// add the sprite to the battle system
				}

/*#**************************************************************************#*/

				// # SCENE [image-file]:[area-name]:[list-of-tile-index-values]
				else if ("SCENE" == commandToken)
				{
					std::vector<std::string> cmdDefParams = GameSingleton::GetInstance()->Tokenize(paramsToken, ":");
					unsigned int cmdDefParamCount = cmdDefParams.size();
					if (0x3 != cmdDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [image-file]:[area-name]:[list-of-tile-index-values]\n\n",
						battlePath, index + 1);
					}
					else
					{
						// get image file name
						// get name of scene
						// get the list of index values

#if defined(_BATTLE_LOADER_VERBOSE)
					fprintf(stderr,
						"\tSCENE %d\n"
						"\t\tDepends on an image resource named %s\n"
						"\t\tThe name of the scene is \"%s\"\n"
						"\t\tThis scene will be used on tiles: %s\n\n",
						0, "<undefined>","<undefined>","<undefined>");
#endif

						// add the scene to the battle system
					}
				}

/*#**************************************************************************#*/

				// # COMBATANT [sprite-id]:[width]:[height]:[frames]:[delay]:[list-of-scenes]
				else if ("COMBATANT" == commandToken)
				{
					std::vector<std::string> cmdDefParams = GameSingleton::GetInstance()->Tokenize(paramsToken, ":");
					unsigned int cmdDefParamCount = cmdDefParams.size();
					if (0x6 != cmdDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [sprite-id]:[width]:[height]:[frames]:[delay]:[list-of-scenes]\n\n",
						battlePath, index + 1);
					}
					else
					{
					}
				}

/*#**************************************************************************#*/

				// # COMBATANTANIM [combatant-id]:[state]:[first-frame]:[last-frame]
				else if ("COMBATANTANIM" == commandToken)
				{
					std::vector<std::string> cmdDefParams = GameSingleton::GetInstance()->Tokenize(paramsToken, ":");
					unsigned int cmdDefParamCount = cmdDefParams.size();
					if (0x4 != cmdDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [combatant-id]:[state]:[first-frame]:[last-frame]\n\n",
						battlePath, index + 1);
					}
					else
					{
					}
				}

/*#**************************************************************************#*/

				// # COMBATANTLOGIC [combatant-id]:[battle-logic-string]
				else if ("COMBATANTLOGIC" == commandToken)
				{
					std::vector<std::string> cmdDefParams = GameSingleton::GetInstance()->Tokenize(paramsToken, ":");
					unsigned int cmdDefParamCount = cmdDefParams.size();
					if (0x2 != cmdDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [combatant-id]:[battle-logic-string]\n\n",
						battlePath, index + 1);
					}
					else
					{
					}
				}

/*#**************************************************************************#*/
#if 0
				// a "template" for how to parse a new command
				else if ("_" == commandToken)
				{
					std::vector<std::string> cmdDefParams = GameSingleton::GetInstance()->Tokenize(paramsToken, ":");
					unsigned int cmdDefParamCount = cmdDefParams.size();
					if (0x0 != cmdDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [parameters]\n\n",
						battlePath, index + 1);
					}
					else
					{
					}
				}
#endif
/*#**************************************************************************#*/

				// else the command is unknown
				else
				{
					LogError("Unknown Command \"%s\" in Battle Definition File %s", commandToken.c_str(), battlePath);
				}

			} // end if two tokens in line
		} // end for each line
	}
} // end namespace
