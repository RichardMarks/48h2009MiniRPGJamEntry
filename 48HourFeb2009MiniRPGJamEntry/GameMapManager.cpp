
// CODESTYLE: v2.0

// GameMapManager.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: manages a registry of named maps

#include "GameLibrary.h"

namespace GAME
{
	GameMapManager::GameMapManager(GameTilesetManager* tilesetManager)
	{
		tilesets_ = tilesetManager;
	}

	GameMapManager::~GameMapManager()
	{
		maps_.clear();
		names_.clear();
	}

	/**************************************************************************/

	void GameMapManager::Add(const char* mapName, const char* mapFilePath, const char* tilesetName)
	{
		GameMapSTLMapIterator iter;

		if (!((iter = names_.find(mapName)) != names_.end()))
		{
			// register the map instance
			GameMap* mapInstance = new GameMap(tilesets_->Get(tilesetName));
			mapInstance->LoadMapFromFile(mapFilePath);
			mapInstance->SetName(mapName);
			maps_.push_back(mapInstance);

			// register the name
			names_[mapName] = static_cast<unsigned int>(maps_.size() - 1);
			//LogMessage("Registered %s\n", mapName);
		}
		else
		{
			LogError("The map [%s] is already registered!\nYou cannot register a map to the same map name!\n", mapName);
		}
	}

	/**************************************************************************/

	GameMap* GameMapManager::Get(const char* mapName)
	{
		GameMapSTLMapIterator iter;

		if ((iter = names_.find(mapName)) != names_.end())
		{
			// return the map instance
			return maps_.at(static_cast<unsigned int>(iter->second));
		}
		else
		{
			LogError("The map [%s] is not registered!\n", mapName);

			// return a null pointer
			return 0;
		}
	}

	/**************************************************************************/

	GameMap* GameMapManager::Get(int index)
	{
		return (static_cast<unsigned int>(index) < maps_.size()) ? maps_.at(index) : 0;
	}

	void GameMapManager::DebugList()
	{
#if 0
		GameMapSTLMapIterator iter;

		for (iter = names_.begin(); iter != names_.end(); iter++)
		{
			GameMap* map = maps_.at(static_cast<unsigned int>(iter->second));
			fprintf(stderr, "Map: %s\n", iter->first.c_str());
			map->DebugList();
		}
#endif
	}

} // end namespace


