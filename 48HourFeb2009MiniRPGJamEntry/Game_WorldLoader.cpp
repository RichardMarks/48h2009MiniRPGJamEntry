
// CODESTYLE: v2.0

// Game_WorldLoader.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the .world file loader implementation for the game singleton class

#include "Game.h"

// so we don't need to prefix the ENGINE:: namespace scope to everything
#define GED101_USE_SIMPLE_NAMESPACES

// windows build bug patch
#if defined(WIN_32)
#define DWORD signed long long
#endif

// include the ged101 minimal header
#include "ged101minimal.h"

#include "GameMapLayer.h"
#include "GameMap.h"
#include "GameTileset.h"

#include "GameCamera.h"
#include "GameMapSprite.h"

#include "GameMapEvent.h"
#include "GameMapEventHandler.h"

#include "GameMapManager.h"
#include "GameTilesetManager.h"

#include "GameObject.h"

#include "GameBasicMapWarp.h"

#include "GameMapSpriteManager.h"
#include "GameNPCManager.h"
#include "GameNPC.h"

// few defines for changing available commands
#define _WORLD_LOADER_VERBOSE
#define _WORLD_FORMAT_VERSION_2

#if defined(_WORLD_FORMAT_VERSION_2)

#endif

namespace GAME
{
	#if 1
	
	void GameSingleton::LoadWorldFile(const char* worldPath)
	{
		FILE* fp = fopen(worldPath, "r");
		if (!fp) { LogFatal("Cannot load world: %s!", worldPath); return; }
		
		// delete the tileset and map managers if they exist
		if (gameTiles_) { delete gameTiles_; }
		if (gameMaps_) { delete gameMaps_; }
		if (gameSprites_) { delete gameSprites_; }
		if (gameNPCs_) { delete gameNPCs_; }
		
		// init the tileset and map managers 
		gameTiles_ = new GameTilesetManager();
		gameMaps_ = new GameMapManager(gameTiles_);
		gameSprites_ = new GameMapSpriteManager();
		gameNPCs_ = new GameNPCManager();
		
		std::string tilesetDirectory = "";
		std::string mapDirectory = "";
		std::string spritesDirectory = "";
		
		
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
						case ' ':
						{
						} break;
						case '\t':
						{
						} break;
						case '\n':
						{
						} break;
						case '\r':
						{
						} break;
						case '#':
						{
							p = length;
						} break;
						default:
						{
							lineEmpty = false;
							p = length;
						} break; 
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
		
#if defined(_WORLD_LOADER_VERBOSE)
		fprintf(stderr, "There are %d lines to parse in %s\n", lineCount, worldPath);
#endif

		
		for (unsigned int index = 0; index < lineCount; index++)
		{
			std::string& line = fileLines.at(index);
			std::vector<std::string> tokens = Tokenize(line, " \t");
			unsigned int tokenCount = tokens.size();
			
			if (0x2 == tokenCount)
			{
				std::string& commandToken  = tokens.at(0);
				std::string& paramsToken = tokens.at(1);
				
				// fprintf(stderr, "\tToken 1: \"%s\"\n\tToken 2: \"%s\"\n", commandToken.c_str(), paramsToken.c_str());
				
				if ("TILESDIR" == commandToken)
				{
#if defined(_WORLD_LOADER_VERBOSE)
					fprintf(stderr, "\tTiles are expected to be stored in %s\n\n", paramsToken.c_str());
#endif
					tilesetDirectory = paramsToken.c_str();
				}
				else if ("MAPSDIR" == commandToken)
				{
#if defined(_WORLD_LOADER_VERBOSE)
					fprintf(stderr, "\tMaps are expected to be stored in %s\n\n", paramsToken.c_str());
#endif
					mapDirectory = paramsToken.c_str();
				}
				else if ("START" == commandToken)
				{
#if defined(_WORLD_LOADER_VERBOSE)
					fprintf(stderr, "\tThe start-up map is named %s\n\n", paramsToken.c_str());
#endif
					currentMap_ = gameMaps_->Get(paramsToken.c_str());
				}
				
#if defined(_WORLD_FORMAT_VERSION_2)
				else if ("SPRITESDIR" == commandToken)
				{
#if defined(_WORLD_LOADER_VERBOSE)
					fprintf(stderr, "\tSprites are expected to be stored in %s\n\n", paramsToken.c_str());
#endif
					spritesDirectory = paramsToken.c_str();
				}

				else if ("PLAYER" == commandToken)
				{
					// PLAYER [sprite-num]:[x-tile]:[y-tile]
					std::vector<std::string> playerDefParams = Tokenize(paramsToken, ":");
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
						fprintf(stderr, 
							"\tPLAYER DEFINITION:\n"
							"\tThe Player will use Sprite #%d and will start at world coordinates %d, %d (%d, %d in pixels).\n\n",
							spriteNum, xTile, yTile, 8 * xTile, 8 * yTile);
#endif					
						// set the player sprite index
						playerSpriteIndex_ = ((spriteNum - 1) >= 0) ? spriteNum - 1 : 0;
						
						// position the player
						gameSprites_->Get(playerSpriteIndex_)->SetWorldPosition(xTile * 8, yTile * 8);
					}
				}
				
				else if ("NPCSPEED" == commandToken)
				{
					// NPCSPEED [npc-num]:[speed]
					std::vector<std::string> npcDefParams = Tokenize(paramsToken, ":");
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
						fprintf(stderr, 
							"\tNPCSPEED DEFINITION:\n"
							"\tNPC #%d will have a speed of %d.\n\n",
							npcNum, npcSpeed);
#endif					
						gameNPCs_->Get(npcNum - 1)->SetSpeed(npcSpeed);
					}
				}
				else if ("NPCMOTION" == commandToken)
				{
					// NPCMOTION [npc-num]:[motion-data]
					std::vector<std::string> npcDefParams = Tokenize(paramsToken, ":");
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
						fprintf(stderr, 
							"\tNPCMOTION DEFINITION:\n"
							"\tNPC #%d will have a motion-data-string of\n\t\"%s\".\n\n",
							npcNum, motionData.c_str());
#endif					
						gameNPCs_->Get(npcNum - 1)->SetMotionData(motionData.c_str());
					}
				}
				
				else if ("WARP" == commandToken)
				{
					// WARP [from-map-name]:[from-x]:[from-y]:[to-map-name]:[to-x]:[to-y]:[armed (true|false)]:[persists (true|false)]
					std::vector<std::string> warpDefParams = Tokenize(paramsToken, ":");
					
					std::string fromMapName = "";
					int fromX = 0;
					int fromY = 0;
					std::string toMapName = "";
					int toX = 0;
					int toY = 0;
					std::string warpArmed = "";
					std::string warpPersists = "";
					
					unsigned int warpDefParamCount = warpDefParams.size();
					if (0x8 != warpDefParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [from-map-name]:[from-x]:[from-y]:[to-map-name]:[to-x]:[to-y]:[armed (true|false)]:[persists (true|false)]\n\n", 
						worldPath, index + 1);
					}
					else
					{
						fromMapName 	= warpDefParams.at(0).c_str();
						fromX 			= atoi(warpDefParams.at(1).c_str());
						fromY 			= atoi(warpDefParams.at(2).c_str());
						toMapName 		= warpDefParams.at(3).c_str();
						toX 			= atoi(warpDefParams.at(4).c_str());
						toY 			= atoi(warpDefParams.at(5).c_str());
						warpArmed 		= warpDefParams.at(6).c_str();
						warpPersists 	= warpDefParams.at(7).c_str();
						
#if defined(_WORLD_LOADER_VERBOSE)
						fprintf(stderr, 
							"\tWARP DEFINITION:\n"
							"\tFrom map \"%s\" at %d, %d to map \"%s\" at %d, %d.\n"
							"\tThe warp %s armed by default and %s persist.\n\n",
							fromMapName.c_str(), fromX, fromY, toMapName.c_str(), toX, toY,
							(warpArmed=="true")?"is ":"is not ",
							(warpPersists=="true")?"will ":"will not ");
#endif					
						gameMaps_->Get(fromMapName.c_str())->AddMapEvent(
							new GameMapEvent(
								fromX, fromY, gameMaps_->Get(fromMapName.c_str())->GetID(),
								GameBasicMapWarp::GetInstance(), 
								(warpArmed=="true")?true:false,
								(warpPersists=="true")?true:false,"",
								toX, toY, gameMaps_->Get(toMapName.c_str())->GetID())
						);
					}
				}
				
				
				
				else if ("NPC" == commandToken)
				{
					// NPC [sprite-num]:[map-name]:[x-tile]:[y-tile]
					std::vector<std::string> npcDefParams = Tokenize(paramsToken, ":");
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
						fprintf(stderr, 
							"\tNPC DEFINITION:\n"
							"\tNPC #%d will use Sprite #%d and will start at world coordinates %d, %d (%d, %d in pixels) on map %s.\n\n",
							gameNPCs_->GetNumNPCs() + 1,
							spriteNum, xTile, yTile, 8 * xTile, 8 * yTile, mapName.c_str());
#endif					
					
						// position the sprite
						gameSprites_->Get(spriteNum - 1)->SetWorldPosition(xTile * 8, yTile * 8);
						
						// add the NPC
#if 1
						// class GameNPCManager;
						// GameNPCManager::Add(GameNPC* npc, const char* parentMapName);
						gameNPCs_->Add(
							// class GameNPC; 
							// GameNPC(GameMapSprite* sprite, int speed = 1, const char* motionData = "");
							new GameNPC(gameSprites_->Get(spriteNum - 1)), mapName.c_str()
						);
#endif
					}
				}

				else if ("SPRITE" == commandToken)
				{
					// SPRITE [file]:[width]:[height]:[frames]:[delay]
					std::vector<std::string> spriteDefParams = Tokenize(paramsToken, ":");
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
#endif						
						// add the sprite to the manager
						spriteFile = spritesDirectory + spriteFile;
						
						gameSprites_->Add(
							new GameMapSprite(spriteFile.c_str(), spriteWidth, spriteHeight, spriteFrames, spriteDelay, 0)
						);
						
					}
				}

#endif


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
						fprintf(stderr, 
							"\tTILESET DEFINITION: %s\n"
							"\tRequires an Image file called %s\n"
							"\tcontaining %d %s of %d %s of %d x %d "
							"pixel tiles with padding of %d %s surrounding them.\n\n", 
							tilesetName.c_str(), tilesetFile.c_str(), tileCols, 
							(1==tileCols)?"column":"columns", tileRows, 
							(1==tileRows)?"row":"rows",tileWidth, tileHeight, tilePadding,
							(1==tilePadding)?"pixel":"pixels");
							
#endif						
						// add the tileset to the manager
						tilesetFile = tilesetDirectory + tilesetFile;
						gameTiles_->Add(tilesetName.c_str(), tilesetFile.c_str(), tileWidth, tileHeight, tileCols, tileRows, tilePadding);
						
					}
				}
				else if ("MAP" == commandToken)
				{
					std::vector<std::string> mapParams = Tokenize(paramsToken, ":");
					std::string mapName = "";
					std::string mapFile = "";
					std::string tilesetName = "";
					
					unsigned int mapParamCount = mapParams.size();
					
					if (0x3 != mapParamCount)
					{
						fprintf(stderr,
						"*** Syntax Error in %s:line #%04d: Missing Parameters:\n    "
						"Expected [map-name]:[file]:[tileset-name]\n\n", 
						worldPath, index + 1);
					}
					else
					{
						mapName 	= mapParams.at(0);
						mapFile 	= mapParams.at(1);
						tilesetName = mapParams.at(2);
						
#if defined(_WORLD_LOADER_VERBOSE)
						fprintf(stderr, 
							"\tMAP DEFINITION: %s\n"
							"\tRequires map data file called %s\n"
							"\tDepends on a Tileset named %s.\n\n",
							mapName.c_str(), mapFile.c_str(), tilesetName.c_str());
#endif
						// add the map to the manager
						mapFile = mapDirectory + mapFile;
						gameMaps_->Add(mapName.c_str(), mapFile.c_str(), tilesetName.c_str());
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

