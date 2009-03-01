
// CODESTYLE: v2.0

// GameTilesetManager.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: manages a registry of named tilesets

#include "GameLibrary.h"

namespace GAME
{
	GameTilesetManager::GameTilesetManager()
	{
	}

	GameTilesetManager::~GameTilesetManager()
	{
		tilesets_.clear();
		names_.clear();
	}

	/**************************************************************************/

#if 0
	void GameTilesetManager::Add(const char* tilesetName, GameTileset* tilesetInstance)
	{
		GameTilesetSTLMapIterator iter;

		if (!((iter = names_.find(tilesetName)) != names_.end()))
		{
			// register the map instance
			tilesets_.push_back(tilesetInstance);

			// register the name
			names_[tilesetName] = static_cast<unsigned int>(tilesets_.size() - 1);
		}
		else
		{
			LogError("The tileset [%s] is already registered!\nYou cannot register a map to the same tileset name!\n", tilesetName);
		}
	}
#endif

	void GameTilesetManager::Add(const char* tilesetName, const char* tilesetFilePath,
		int tileWidth, int tileHeight, int cols, int rows, int spacing)
	{
		GameTilesetSTLMapIterator iter;

		if (!((iter = names_.find(tilesetName)) != names_.end()))
		{
			// register the tileset instance
			ImageResource tempImg(tilesetFilePath);
			tilesets_.push_back(new GameTileset(&tempImg, tileWidth, tileHeight, cols, rows, spacing));

			// register the name
			names_[tilesetName] = static_cast<unsigned int>(tilesets_.size() - 1);
			//LogMessage("Registered %s\n", tilesetName);
		}
		else
		{
			LogError("The tileset [%s] is already registered!\nYou cannot register a tileset to the same tileset name!\n", tilesetName);
		}
	}

	/**************************************************************************/

	GameTileset* GameTilesetManager::Get(const char* tilesetName)
	{
		GameTilesetSTLMapIterator iter;

		if ((iter = names_.find(tilesetName)) != names_.end())
		{
			// return the map instance
			return tilesets_.at(static_cast<unsigned int>(iter->second));
		}
		else
		{
			LogError("The tileset [%s] is not registered!\n", tilesetName);

			// return a null pointer
			return 0;
		}
	}

} // end namespace


