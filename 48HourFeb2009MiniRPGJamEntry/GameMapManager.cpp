
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

	/**************************************************************************/

	GameMapManager::~GameMapManager()
	{
		maps_.clear();
		names_.clear();
	}

	/**************************************************************************/

	void GameMapManager::Add(const char* mapName, const char* tilesetName)
	{
		GameMapSTLMapIterator iter;

		if (!((iter = names_.find(mapName)) != names_.end()))
		{
			// create the map instance
			GameMap* mapInstance = new GameMap(tilesets_->Get(tilesetName));

			// must name the map instance first, as all loader code needs the name
			mapInstance->SetName(mapName);

			// get the maps directory from the game singleton
			const char* mapsPath = GameSingleton::GetInstance()->GetMapsDirectory().c_str();

			// grab the setting from the game instance
			bool enableVerboseStartup = ("true" == GameSingleton::GetInstance()->GetSetting("enable_verbose_startup")) ? true : false;

			if (!enableVerboseStartup)
			{
				LogSimpleMessage("Loading Map \"%s\" ...", mapName);
			}

			if (enableVerboseStartup)
			{
				LogSimpleMessage("Loading Map Layout for \"%s\" ...", mapName);
			}
			// load the .map
			mapInstance->LoadMapData(mapsPath);

			if (enableVerboseStartup)
			{
				LogSimpleMessage("Loading Map Collision Data for \"%s\" ...", mapName);
			}
			// load the .collision
			mapInstance->LoadCollisionData(mapsPath);

			if (enableVerboseStartup)
			{
				LogSimpleMessage("Loading Map Warp Data for \"%s\" ...", mapName);
			}
			// load the .warp
			mapInstance->LoadMapWarpData(mapsPath);

			if (enableVerboseStartup)
			{
				LogSimpleMessage("Loading Map Event Data for \"%s\" ...", mapName);
			}
			// load the .event
			mapInstance->LoadEventData(mapsPath);

			// add the map to the maps_ vector
			maps_.push_back(mapInstance);

			// register the map name
			names_[mapName] = static_cast<unsigned int>(maps_.size() - 1);
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

	/**************************************************************************/

	void GameMapManager::DebugList()
	{
	}

	/**************************************************************************/

	unsigned int GameMapManager::GetNumMaps() const
	{
		return maps_.size();
	}

} // end namespace


