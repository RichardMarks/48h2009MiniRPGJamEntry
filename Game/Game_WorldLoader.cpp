
// CODESTYLE: v2.0

// Game_WorldLoader.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the .world file loader implementation for the game singleton class

#include "GameLibrary.h"

// few defines for changing available commands
#define _WORLD_LOADER_VERBOSE

namespace GAME
{
	#if 1

	void GameSingleton::LoadWorldFile(const char* worldPath)
	{
		// grab the setting from the game instance
		bool enableVerboseStartup = ("true" == GameSingleton::GetInstance()->GetSetting("enable_verbose_startup")) ? true : false;

		if (enableVerboseStartup)
		{
			LogSimpleMessage("Loading World Configuration File \"%s\" ...", worldPath);
		}
		else
		{
			LogSimpleMessage("Loading World...");
		}

		std::vector<std::string> fileLines = UTILITY::PARSING::TextFileParser::Execute(worldPath, "#");
		unsigned int lineCount = fileLines.size();

		if (!lineCount)
		{
			LogSimpleMessage("There was a problem loading the World file \"%s\" No valid commands found.");
			return;
		}

		// delete the managers if they exist
		if (gameTiles_) 	{ delete gameTiles_; }
		if (gameMaps_) 		{ delete gameMaps_; }
		if (gameSprites_) 	{ delete gameSprites_; }
		if (gameNPCs_) 		{ delete gameNPCs_; }

		// init the managers
		gameTiles_ 		= new GameTilesetManager();
		gameMaps_ 		= new GameMapManager(gameTiles_);
		gameSprites_ 	= new GameMapSpriteManager();
		gameNPCs_ 		= new GameNPCManager();

	/**************************************************************************/

		// for each line, parse it
#if defined(_WORLD_LOADER_VERBOSE)
		if (enableVerboseStartup)
		{
			fprintf(stderr, "\nThere are %d lines to parse in %s\n", lineCount, worldPath);
		}
#endif


		for (unsigned int index = 0; index < lineCount; index++)
		{
			std::string& line = fileLines.at(index);
			std::vector<std::string> tokens = UTILITY::PARSING::StringTokenizer::Execute(line, " \t");
			unsigned int tokenCount = tokens.size();

			if (0x2 == tokenCount)
			{
				std::string& commandToken  = tokens.at(0);
				std::string& paramsToken = tokens.at(1);

				if ("START" == commandToken)
				{
#if defined(_WORLD_LOADER_VERBOSE)
					if (enableVerboseStartup)
					{
						fprintf(stderr, "\tThe start-up map is named %s\n\n", paramsToken.c_str());
					}
#endif
					currentMap_ = gameMaps_->Get(paramsToken.c_str());
				}

	/**************************************************************************/

				else if ("PLAYER" == commandToken)
				{
					// PLAYER [sprite-num]:[x-tile]:[y-tile]
					std::vector<std::string> playerDefParams = UTILITY::PARSING::StringTokenizer::Execute(paramsToken, ":");
					int spriteNum = 0;
					int xTile = 0;
					int yTile = 0;

					unsigned int playerDefParamCount = playerDefParams.size();
					if (0x3 != playerDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [sprite-num]:[x-tile]:[y-tile]\n\n",
						worldPath, index + 1);
					}
					else
					{
						spriteNum 	= atoi(playerDefParams.at(0).c_str());
						xTile		= atoi(playerDefParams.at(1).c_str());
						yTile		= atoi(playerDefParams.at(2).c_str());
#if defined(_WORLD_LOADER_VERBOSE)
						if (enableVerboseStartup)
						{
							fprintf(stderr,
								"\tPLAYER DEFINITION:\n"
								"\tThe Player will use Sprite #%d and will start at world coordinates %d, %d (%d, %d in pixels).\n\n",
								spriteNum, xTile, yTile, 8 * xTile, 8 * yTile);
						}
#endif
						// set the player sprite index
						playerSpriteIndex_ = ((spriteNum - 1) >= 0) ? spriteNum - 1 : 0;

						// position the player
						gameSprites_->Get(playerSpriteIndex_)->SetWorldPosition(xTile * 8, yTile * 8);
					}
				}

	/**************************************************************************/

				else if ("NPCSPEED" == commandToken)
				{
					// NPCSPEED [npc-num]:[speed]
					std::vector<std::string> npcDefParams = UTILITY::PARSING::StringTokenizer::Execute(paramsToken, ":");
					int npcNum = 0;
					int npcSpeed = 0;

					unsigned int npcDefParamCount = npcDefParams.size();
					if (0x2 != npcDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [npc-num]:[speed]\n\n",
						worldPath, index + 1);
					}
					else
					{
						npcNum 		= atoi(npcDefParams.at(0).c_str());
						npcSpeed 	= atoi(npcDefParams.at(1).c_str());
#if defined(_WORLD_LOADER_VERBOSE)
						if (enableVerboseStartup)
						{
							fprintf(stderr,
								"\tNPCSPEED DEFINITION:\n"
								"\tNPC #%d will have a speed of %d.\n\n",
								npcNum, npcSpeed);
						}
#endif
						gameNPCs_->Get(npcNum - 1)->SetSpeed(npcSpeed);
					}
				}

	/**************************************************************************/

				else if ("NPCMOTION" == commandToken)
				{
					// NPCMOTION [npc-num]:[motion-data]
					std::vector<std::string> npcDefParams = UTILITY::PARSING::StringTokenizer::Execute(paramsToken, ":");
					int npcNum = 0;
					std::string motionData = "";

					unsigned int npcDefParamCount = npcDefParams.size();
					if (0x2 != npcDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [npc-num]:[motion-data]\n\n",
						worldPath, index + 1);
					}
					else
					{
						npcNum 		= atoi(npcDefParams.at(0).c_str());
						motionData 	= npcDefParams.at(1).c_str();
#if defined(_WORLD_LOADER_VERBOSE)
						if (enableVerboseStartup)
						{
							fprintf(stderr,
								"\tNPCMOTION DEFINITION:\n"
								"\tNPC #%d will have a motion-data-string of\n\t\"%s\".\n\n",
								npcNum, motionData.c_str());
						}
#endif
						gameNPCs_->Get(npcNum - 1)->SetMotionData(motionData.c_str());
					}
				}

	/**************************************************************************/

				else if ("NPCCHAT" == commandToken)
				{
					// NPCCHAT [npc-id]:[dialogue-file]
					std::vector<std::string> npcDefParams = UTILITY::PARSING::StringTokenizer::Execute(paramsToken, ":");
					int npcNum = 0;
					std::string dlgFile = "";

					unsigned int npcDefParamCount = npcDefParams.size();
					if (0x2 != npcDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [npc-id]:[dialogue-file]\n\n",
						worldPath, index + 1);
					}
					else
					{
						npcNum 		= atoi(npcDefParams.at(0).c_str());
						dlgFile 	= npcDefParams.at(1).c_str();
#if defined(_WORLD_LOADER_VERBOSE)
						if (enableVerboseStartup)
						{
							fprintf(stderr,
								"\tNPCCHAT DEFINITION:\n"
								"\tNPC #%d depends on a .dialogue file called \"%s\".\n\n",
								npcNum, dlgFile.c_str());
						}
#endif
						gameNPCs_->Get(npcNum - 1)->AddDialogueFile(dlgFile.c_str());
					}
				}

	/**************************************************************************/

				else if ("NPC" == commandToken)
				{
					// NPC [sprite-num]:[map-name]:[x-tile]:[y-tile]
					std::vector<std::string> npcDefParams = UTILITY::PARSING::StringTokenizer::Execute(paramsToken, ":");
					std::string mapName = "";
					int spriteNum = 0;
					int xTile = 0;
					int yTile = 0;

					unsigned int npcDefParamCount = npcDefParams.size();
					if (0x4 != npcDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [sprite-num]:[map-name]:[x-tile]:[y-tile]\n\n",
						worldPath, index + 1);
					}
					else
					{
						spriteNum 	= atoi(npcDefParams.at(0).c_str());
						mapName		= npcDefParams.at(1).c_str();
						xTile		= atoi(npcDefParams.at(2).c_str());
						yTile		= atoi(npcDefParams.at(3).c_str());
#if defined(_WORLD_LOADER_VERBOSE)
						if (enableVerboseStartup)
						{
							fprintf(stderr,
								"\tNPC DEFINITION:\n"
								"\tNPC #%d will use Sprite #%d and will start at world coordinates"
								" %d, %d (%d, %d in pixels) on map %s.\n\n",
								gameNPCs_->GetNumNPCs() + 1,
								spriteNum,
								xTile,
								yTile,
								8 * xTile,
								8 * yTile,
								mapName.c_str());
						}
#endif

						// position the sprite
						gameSprites_->Get(spriteNum - 1)->SetWorldPosition(xTile * 8, yTile * 8);

						// add the NPC
#if 1
						// class GameNPCManager;
						// GameNPCManager::Add(GameNPC* npc, const char* parentMapName);

						if (enableVerboseStartup)
						{
							fprintf(stderr, "Listing NPCs Before ADD\n");
							gameNPCs_->List(stderr);
						}
						gameNPCs_->Add(
							// class GameNPC;
							// GameNPC(GameMapSprite* sprite, int speed = 1, const char* motionData = "");
							new GameNPC(gameSprites_->Get(spriteNum - 1)), mapName.c_str()
						);
						if (enableVerboseStartup)
						{
							fprintf(stderr, "Listing NPCs After ADD\n");
							gameNPCs_->List(stderr);
						}
#endif
					}
				}

	/**************************************************************************/

				else if ("SPRITE" == commandToken)
				{
					// SPRITE [file]:[width]:[height]:[frames]:[delay]
					std::vector<std::string> spriteDefParams = UTILITY::PARSING::StringTokenizer::Execute(paramsToken, ":");
					std::string spriteFile = "";
					int spriteWidth = 0;
					int spriteHeight = 0;
					int spriteFrames = 0;
					int spriteDelay = 0;

					unsigned int spriteDefParamCount = spriteDefParams.size();
					if (0x5 != spriteDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [file]:[width]:[height]:[frames]:[delay]\n\n",
						worldPath, index + 1);
					}
					else
					{
						spriteFile		= spriteDefParams.at(0);
						spriteWidth 	= atoi(spriteDefParams.at(1).c_str());
						spriteHeight	= atoi(spriteDefParams.at(2).c_str());
						spriteFrames	= atoi(spriteDefParams.at(3).c_str());
						spriteDelay		= atoi(spriteDefParams.at(4).c_str());

#if defined(_WORLD_LOADER_VERBOSE)
						if (enableVerboseStartup)
						{
							fprintf(stderr,
								"\tSPRITE DEFINITION: #%04d\n"
								"\tSprite will have a frame-delay of %d\n"
								"\tRequires an Image file called %s\n"
								"\tcontaining %d %s of %d x %d "
								"pixel cells with padding of 1 pixel surrounding them.\n\n",
								gameSprites_->GetNumSprites() + 1, spriteDelay,
								spriteFile.c_str(), spriteFrames*4,
								(1==spriteFrames)?"frame":"frames",
								spriteWidth, spriteHeight);
						}
#endif
						// add the sprite to the manager
						spriteFile = GetSpritesDirectory() + spriteFile;

						gameSprites_->Add(
							new GameMapSprite(spriteFile.c_str(), spriteWidth, spriteHeight, spriteFrames, spriteDelay, 0)
						);

					}
				}

	/**************************************************************************/

				else if ("TILESET" == commandToken)
				{
					std::vector<std::string> tilesetParams = Tokenize(paramsToken, ":");
					std::string tilesetName = "";
					std::string tilesetFile = "";
					int tileCols = 0;
					int tileRows = 0;
					int tileWidth = 0;
					int tileHeight = 0;
					int tilePadding = 0;

					unsigned int tilesetParamCount = tilesetParams.size();

					if (0x7 != tilesetParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [name]:[file]:[width]:[height]:[columns]:[rows]:[padding]\n\n",
						worldPath, index + 1);
					}
					else
					{
						tilesetName 	= tilesetParams.at(0);
						tilesetFile 	= tilesetParams.at(1);
						tileCols 		= atoi(tilesetParams.at(4).c_str());
						tileRows 		= atoi(tilesetParams.at(5).c_str());
						tileWidth 		= atoi(tilesetParams.at(2).c_str());
						tileHeight 		= atoi(tilesetParams.at(3).c_str());
						tilePadding 	= atoi(tilesetParams.at(6).c_str());

#if defined(_WORLD_LOADER_VERBOSE)
						if (enableVerboseStartup)
						{
							fprintf(stderr,
								"\tTILESET DEFINITION: %s\n"
								"\tRequires an Image file called %s\n"
								"\tcontaining %d %s of %d %s of %d x %d "
								"pixel tiles with padding of %d %s surrounding them.\n\n",
								tilesetName.c_str(), tilesetFile.c_str(), tileCols,
								(1==tileCols)?"column":"columns", tileRows,
								(1==tileRows)?"row":"rows",tileWidth, tileHeight, tilePadding,
								(1==tilePadding)?"pixel":"pixels");
						}

#endif
						// add the tileset to the manager
						tilesetFile = GetTilesDirectory() + tilesetFile;
						gameTiles_->Add(tilesetName.c_str(), tilesetFile.c_str(), tileWidth, tileHeight, tileCols, tileRows, tilePadding);

					}
				}

	/**************************************************************************/

				else if ("MAP" == commandToken)
				{
					std::vector<std::string> mapParams = Tokenize(paramsToken, ":");
					std::string mapName = "";
					std::string tilesetName = "";

					unsigned int mapParamCount = mapParams.size();

					if (0x2 != mapParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [map-name]:[tileset-name]\n\n",
						worldPath, index + 1);
					}
					else
					{
						mapName 	= mapParams.at(0);
						tilesetName = mapParams.at(1);

#if defined(_WORLD_LOADER_VERBOSE)
						if (enableVerboseStartup)
						{
							const char* mapsPath = GetMapsDirectory().c_str();
							fprintf(stderr,
								"\tMAP DEFINITION: %s\n"
								"\tRequires the following map data files:\n"
								"\t\t%s%s/%s.map\n"
								"\t\t%s%s/%s.collision\n"
								"\t\t%s%s/%s.warp\n"
								"\t\t%s%s/%s.event\n"
								"\n"
								"\tDepends on a Tileset named %s.\n\n",
								mapName.c_str(),
								mapsPath, mapName.c_str(), mapName.c_str(),
								mapsPath, mapName.c_str(), mapName.c_str(),
								mapsPath, mapName.c_str(), mapName.c_str(),
								mapsPath, mapName.c_str(), mapName.c_str(),
								tilesetName.c_str());
						}
#endif
						// add the map to the manager
						gameMaps_->Add(mapName.c_str(), tilesetName.c_str());
					}
				}
				else
				{
					LogError("Unknown Command \"%s\" in World File %s", commandToken.c_str(), worldPath);
				}
			}
		}
	}
	#endif

	/**************************************************************************/

	std::vector<std::string> GameSingleton::Tokenize(const std::string& source, const std::string& delimiters)
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

