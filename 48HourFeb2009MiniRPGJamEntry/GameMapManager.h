
// CODESTYLE: v2.0

// GameMapManager.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: manages a registry of named maps

#ifndef __GAMEMAPMANAGER_H__
#define __GAMEMAPMANAGER_H__

#include <vector>
#include <map>
#include <string>

namespace GAME
{
	class GameMap;
	class GameTilesetManager;
	
	class GameMapManager
	{
	public:
		GameMapManager(GameTilesetManager* tilesetManager);
		~GameMapManager();
		
		// void Add(const char* mapName, GameMap* mapInstance);
		void Add(const char* mapName, const char* mapFilePath, const char* tilesetName);
		
		GameMap* Get(const char* mapName);
		
		GameMap* Get(int index);
		
		void DebugList();
		
	private:
		GameMapManager(const GameMapManager& rhs);
		const GameMapManager& operator=(const GameMapManager& rhs);
		
	private:
		typedef std::vector<GameMap*> GameMapSTLVector;
		typedef std::vector<GameMap*>::iterator GameMapSTLVectorIterator;
		typedef std::map<std::string, unsigned int> GameMapSTLMap;
		typedef std::map<std::string, unsigned int>::iterator GameMapSTLMapIterator;
		
		GameMapSTLVector maps_;
		GameMapSTLMap names_;
		
		GameTilesetManager* tilesets_;
	};
	
} // end namespace
#endif


