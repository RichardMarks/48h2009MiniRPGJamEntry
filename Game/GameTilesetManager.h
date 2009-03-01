
// CODESTYLE: v2.0

// GameTilesetManager.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: manages a registry of named tilesets

#ifndef __GAMETILESETMANAGER_H__
#define __GAMETILESETMANAGER_H__

#include <vector>
#include <map>
#include <string>

namespace GAME
{
	class GameTileset;
	
	class GameTilesetManager
	{
	public:
		GameTilesetManager();
		~GameTilesetManager();
		
		// void Add(const char* tilesetName, GameTileset* tilesetInstance);
		void Add(const char* tilesetName, const char* tilesetFilePath, int tileWidth, int tileHeight, int cols, int rows, int spacing = 1);
		
		GameTileset* Get(const char* tilesetName);
		
	private:
		GameTilesetManager(const GameTilesetManager& rhs);
		const GameTilesetManager& operator=(const GameTilesetManager& rhs);
		
	private:
		typedef std::vector<GameTileset*> GameTilesetSTLVector;
		typedef std::vector<GameTileset*>::iterator GameTilesetSTLVectorIterator;
		typedef std::map<std::string, unsigned int> GameTilesetSTLMap;
		typedef std::map<std::string, unsigned int>::iterator GameTilesetSTLMapIterator;
		
		GameTilesetSTLVector tilesets_;
		GameTilesetSTLMap names_;
		
	};
} // end namespace
#endif


