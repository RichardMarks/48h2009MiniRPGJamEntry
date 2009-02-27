// CODESTYLE: v2.0

// GameMap.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks, RedSlash
// Purpose: the class that defines a map

#include "GameLibrary.h"

namespace GAME
{
	int GameMap::totalMapCount = 0;

/*#**************************************************************************#*/

	// the new map data interface methods

	 // file paths are assumed to be in the format of "data/maps/"

	void GameMap::SaveMapData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.map
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".map";

		// open the file
		FILE* fp = fopen(mapFileName.c_str(), "wb");
		if (!fp)
		{
			LogFatal("Could not open %s for writing.\n", mapFileName.c_str());
		}

		// get a pointer to the base map layer
		GameMapLayer* layer = GetGameMapLayer(0);

		// write the dimensions of the map
		unsigned char mapWidth = static_cast<unsigned char>(layer->GetNumColumns());
		unsigned char mapHeight = static_cast<unsigned char>(layer->GetNumRows());

		fwrite(&mapWidth, sizeof(unsigned char), 1, fp);
		fwrite(&mapHeight, sizeof(unsigned char), 1, fp);

		// write the layer data
		std::vector<GameMapLayer::Tile>& tiles = layer->GetTileData();

		const int tileSize = mapWidth * mapHeight;

		for (int index = 0; index < tileSize; index++)
		{
			unsigned char tileIndex = static_cast<unsigned char>(tiles[index].tileIndex);
			fwrite(&tileIndex, sizeof(unsigned char), 1, fp);
		}

		// close the file
		fclose(fp);
	}

	/**************************************************************************/

	void GameMap::LoadMapData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.map
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".map";

		// clear any data
		Clear();

		// open the file
		FILE* fp = fopen(mapFileName.c_str(), "rb");
		if (!fp)
		{
			LogFatal("Could not open \"%s\" for reading.\n", mapFileName.c_str());
		}

		// read the dimensions of the map
		unsigned char mapWidth, mapHeight;
		fread(&mapWidth, sizeof(unsigned char), 1, fp);
		fread(&mapHeight, sizeof(unsigned char), 1, fp);

		// create the base map layer
		int layerIndex = CreateMapLayer(mapWidth, mapHeight);

		// get a pointer to the base map layer
		GameMapLayer* layer = GetGameMapLayer(layerIndex);

		// read in the layer data
		std::vector<GameMapLayer::Tile>& tiles = layer->GetTileData();

		const int tileSize = mapWidth * mapHeight;

		for (int index = 0; index < tileSize; index++)
		{
			unsigned char tileIndex;
			fread(&tileIndex, sizeof(unsigned char), 1, fp);

			tiles[index].tileIndex = tileIndex;
		}

		// close the file
		fclose(fp);
	}

	/**************************************************************************/

	void GameMap::SaveCollisionData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.collision
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".collision";

		// open the file
		FILE* fp = fopen(mapFileName.c_str(), "wb");
		if (!fp)
		{
			LogFatal("Could not open %s for writing.\n", mapFileName.c_str());
		}

		// get a pointer to the base map layer
		GameMapLayer* layer = GetGameMapLayer(0);

		// write the collision data

		std::vector<GameMapLayer::Tile>& tiles = layer->GetTileData();

		unsigned char mapWidth = static_cast<unsigned char>(layer->GetNumColumns());
		unsigned char mapHeight = static_cast<unsigned char>(layer->GetNumRows());
		const int tileSize = mapWidth * mapHeight;

		for (int index = 0; index < tileSize; index++)
		{
			unsigned char colDat = static_cast<unsigned char>(
				(tiles[index].eventNo == 0xFF) ? 0xFF : 0x0);
			fwrite(&colDat, sizeof(unsigned char), 1, fp);
		}

		// close the file
		fclose(fp);

	}

	/**************************************************************************/

	void GameMap::LoadCollisionData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.collision
		std::string mapFileName = static_cast<std::string>(
			filePath +
				mapName_ +
					"/" +
						mapName_ +
							".collision");

		// open the file
		FILE* fp = fopen(mapFileName.c_str(), "rb");
		if (!fp)
		{
			LogFatal("Could not open \"%s\" for reading.\n", mapFileName.c_str());
		}

		// get a pointer to the base map layer
		GameMapLayer* layer = GetGameMapLayer(0);

		// read in the layer data
		std::vector<GameMapLayer::Tile>& tiles = layer->GetTileData();

		unsigned char mapWidth = static_cast<unsigned char>(layer->GetNumColumns());
		unsigned char mapHeight = static_cast<unsigned char>(layer->GetNumRows());
		const int tileSize = mapWidth * mapHeight;

		for (int index = 0; index < tileSize; index++)
		{
			unsigned char colDat;
			fread(&colDat, sizeof(unsigned char), 1, fp);
			tiles[index].eventNo = colDat;
		}

		// close the file
		fclose(fp);

	}

	/**************************************************************************/

	void GameMap::SaveEventData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.event
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".event";

		LogMessage("TODO: implement event saving routine");
	}

	/**************************************************************************/

	void GameMap::LoadEventData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.event
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".event";

		// grab the setting from the game instance
		bool enableVerboseStartup = ("true" == GameSingleton::GetInstance()->GetSetting("enable_verbose_startup")) ? true : false;

		if (enableVerboseStartup)
		{
			LogMessage("TODO: implement event loading routine");
		}
	}

	/**************************************************************************/

	void GameMap::SaveMapWarpData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.warp
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".warp";

		// LogMessage("TODO: implement map warp saving routine");

		// open the file
		FILE* fp = fopen(mapFileName.c_str(), "wb");
		if (!fp)
		{
			LogFatal("Could not open %s for writing.\n", mapFileName.c_str());
		}

		// get a pointer to the base map layer
		GameMapLayer* layer = GetGameMapLayer(0);

		// write the warp data data

		std::vector<GameMapLayer::Tile>& tiles = layer->GetTileData();

		unsigned char mapWidth = static_cast<unsigned char>(layer->GetNumColumns());
		unsigned char mapHeight = static_cast<unsigned char>(layer->GetNumRows());
		const int tileSize = mapWidth * mapHeight;

		// write mapWidth*mapHeight of the warp index values as signed int values from the eventNo of layer 0 of the map
		// negative values mean there is no warp at the location
		for (int index = 0; index < tileSize; index++)
		{
			signed int e = static_cast<signed int>(tiles[index].eventNo);

			signed int warpIndex = static_cast<signed int>((e >= 0x0 && e < 0xFF) ? e : -1);
			fwrite(&warpIndex, sizeof(signed int), 1, fp);
		}

		// write the total number of warps (A) as unsigned int value
		unsigned int numWarps = warps_.size();
		fwrite(&numWarps, sizeof(unsigned int), 1, fp);

		// write (A) number of 6 unsigned int values (the real warp data)
		for (unsigned int index = 0; index < numWarps; index++)
		{
			WarpTargetPair* pair 	= warps_.at(index);

			unsigned int startX 	= pair->startX_;
			unsigned int startY 	= pair->startY_;
			unsigned int startMapID = pair->startMap_;
			unsigned int endX 		= pair->endX_;
			unsigned int endY 		= pair->endY_;
			unsigned int endMapID 	= pair->endMap_;

			fwrite(&startX, sizeof(unsigned int), 1, fp);
			fwrite(&startY, sizeof(unsigned int), 1, fp);
			fwrite(&startMapID, sizeof(unsigned int), 1, fp);
			fwrite(&endX, sizeof(unsigned int), 1, fp);
			fwrite(&endY, sizeof(unsigned int), 1, fp);
			fwrite(&endMapID, sizeof(unsigned int), 1, fp);
		}

		// close the file
		fclose(fp);


	}

	/**************************************************************************/

	void GameMap::LoadMapWarpData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.warp
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".warp";

		// LogMessage("TODO: implement map warp loading routine");



		// open the file
		FILE* fp = fopen(mapFileName.c_str(), "rb");
		if (!fp)
		{
			LogFatal("Could not open \"%s\" for reading.\n", mapFileName.c_str());
		}

		// get a pointer to the base map layer
		GameMapLayer* layer = GetGameMapLayer(0);

		// read in the layer data
		std::vector<GameMapLayer::Tile>& tiles = layer->GetTileData();

		unsigned char mapWidth = static_cast<unsigned char>(layer->GetNumColumns());
		unsigned char mapHeight = static_cast<unsigned char>(layer->GetNumRows());


		// clear the warps vector
		warps_.clear();

		// read mapWidth*mapHeight of the wap index values as signed int values into the eventNo of layer 0 of the map

		const int tileSize = mapWidth * mapHeight;

		for (int index = 0; index < tileSize; index++)
		{
			signed int warpIndex;
			fread(&warpIndex, sizeof(signed int), 1, fp);

			if (tiles[index].eventNo != 0xFF && tiles[index].eventNo != 0xFE)
			{
				tiles[index].eventNo = warpIndex;
			}
		}

		// read the total number of warps as unsigned int value as (A)
		unsigned int numWarps;
		fread(&numWarps, sizeof(unsigned int), 1, fp);

		// read (A) number of 6 unsigned ints values creating new warptargetpairs and storing each into the map warps_ vector
		for (unsigned int index = 0; index < numWarps; index++)
		{
			unsigned int startX;
			unsigned int startY;
			unsigned int startMapID;
			unsigned int endX;
			unsigned int endY;
			unsigned int endMapID;

			fread(&startX, sizeof(unsigned int), 1, fp);
			fread(&startY, sizeof(unsigned int), 1, fp);
			fread(&startMapID, sizeof(unsigned int), 1, fp);
			fread(&endX, sizeof(unsigned int), 1, fp);
			fread(&endY, sizeof(unsigned int), 1, fp);
			fread(&endMapID, sizeof(unsigned int), 1, fp);

			AddWarpTargetPair(startX, startY, startMapID, endX, endY, endMapID);

#if 0
			fprintf(stderr,
				"WARP From (%d, %d) on %d To (%d, %d) on %d\n",
				startX, startY,
				startMapID,
				endX, endY, endMapID
				);
#endif
		}

		// close the file
		fclose(fp);


	}

/*#**************************************************************************#*/

	/**************************************************************************/

	GameMap::GameMap(GameTileset* tileSet) :
		tileSet_(tileSet)
	{
		mapID_ = GameMap::totalMapCount;
		GameMap::totalMapCount++;
	}

	/**************************************************************************/

	const int GameMap::GetID() const
	{
		return mapID_;
	}

	/**************************************************************************/

	void GameMap::Clear()
	{
		std::vector<GameMapLayer>().swap(layers_);
	}

	/**************************************************************************/

	void GameMap::LoadMapFromFile(const char *pathname)
	{
		LogError("Apparently you forgot to update your code, and you are trying to use a function that no longer serves its purpose.");
		LogFatal("Terminating Execution to Prevent Damage to the Innocent.. XD");
#if 0
		// map format: http://ccpssolutions.com/nogdusforums/index.php?topic=365.msg1163;topicseen#msg1163
		// unsigned char mapWidth;
		// unsigned char mapHeight;
		// map data: (mapWidth*mapHeight) of unsigned char
		// collision/event data: (mapWidth*mapHeight) of unsigned char

		Clear();

		std::FILE* file = std::fopen(pathname,"rb");
		if (!file) LogFatal("%s: Failed to open file.\n");

		// there is no error checking for map files, make sure they are correct!
		unsigned char mapWidth, mapHeight;
		fread(&mapWidth, 1, 1, file);
		fread(&mapHeight, 1, 1, file);

		int layerIndex = CreateMapLayer(mapWidth, mapHeight);
		GameMapLayer* layer = GetGameMapLayer(layerIndex);

		std::vector<GameMapLayer::Tile>& tiles = layer->GetTileData();

		const int tileSize = mapWidth * mapHeight;
		for (int i = 0; i < tileSize; i++)
		{
			unsigned char index;
			std::fread(&index, 1, 1, file);
			tiles[i].tileIndex = index;
		}

		for (int i = 0; i < tileSize; i++)
		{
			unsigned char event;
			std::fread(&event, 1, 1, file);
			tiles[i].eventNo = event;
		}

		std::fclose(file);
#endif
	}

	/**************************************************************************/

	GameTileset* GameMap::SetGameTileset(GameTileset* tileSet)
	{
		std::swap(tileSet_, tileSet);
		return tileSet;
	}

	/**************************************************************************/

	GameTileset* GameMap::GetTileset() const
	{
		return tileSet_;
	}

	/**************************************************************************/

	 int GameMap::CreateMapLayer(int cols, int rows)
	 {
	 	layers_.push_back(GameMapLayer(tileSet_, cols, rows));
	 	return static_cast<int>(layers_.size() - 1);
	 }

	/**************************************************************************/

	 GameMapLayer* GameMap::GetGameMapLayer(int layerIndex)
	 {
		 return &layers_[layerIndex];
	 }

	/**************************************************************************/

	 void GameMap::DrawMap(ImageResource* destImage, int srcX, int srcY, int destX, int destY, int width, int height)
	 {
	 	for (int i = 0; i < static_cast<int>(layers_.size()); i++)
	 	{
	 		layers_[i].DrawLayer(destImage, srcX, srcY, destX, destY, width, height);
	 	}
	 }

	/**************************************************************************/

	void GameMap::SetTileSolid(int tileX, int tileY, bool isSolid)
	{
		layers_[0].SetEventAt(tileX, tileY, (isSolid) ? 0xFF : 0xFE);
	}

	/**************************************************************************/

	bool GameMap::IsSolid(int tileX, int tileY) const
	{
		return (layers_[0].GetEventAt(tileX, tileY) == 0xFF) ? true : false;
	}

	/**************************************************************************/

	bool GameMap::IsWarp(int tileX, int tileY) const
	{
		// return (layers_[0].GetEventAt(tileX, tileY) < 0xFE && layers_[0].GetEventAt(tileX, tileY) >= 0x0) ? true : false;
		signed int warpIndex = layers_[0].GetEventAt(tileX, tileY);

		return (warpIndex < 0x0) ? false :
			(warpIndex < 0xFE) ? true : false;
	}

	/**************************************************************************/

	void GameMap::SetWarp(int tileX, int tileY, int warpIndex)
	{
		layers_[0].SetEventAt(tileX, tileY, warpIndex);
	}

	/**************************************************************************/

	int GameMap::GetWarp(int tileX, int tileY) const
	{
		return layers_[0].GetEventAt(tileX, tileY);
	}

	/**************************************************************************/

	void GameMap::ClearMapEvents()
	{
		events_.clear();
	}

	/**************************************************************************/

	void GameMap::AddMapEvent(GameMapEvent* event)
	{
		events_.push_back(event);
	}

	/**************************************************************************/

	GameMapEvent* GameMap::GetMapEvent(int eventIndex)
	{
		return (static_cast<unsigned int>(eventIndex) < events_.size()) ? events_.at(eventIndex) : 0;
	}

	/**************************************************************************/

	void GameMap::SetName(const char* mapName)
	{
		mapName_ = mapName;
	}

	/**************************************************************************/

	std::string GameMap::GetName() const
	{
		return mapName_;
	}

	/**************************************************************************/

	void GameMap::DebugList()
	{
	}

	/**************************************************************************/

	void GameMap::AddWarpTargetPair(int startX, int startY, int startMap, int endX, int endY, int endMap)
	{
		// add the warp data
		warps_.push_back(new WarpTargetPair(startX, startY, startMap, endX, endY, endMap));
	}

	/**************************************************************************/

	WarpTargetPair* GameMap::GetWarpTargetPair(unsigned int index) const
	{
		return (index < warps_.size()) ? warps_.at(index) : 0;
	}

	/**************************************************************************/

	void GameMap::ClearWarpTargetPairs()
	{
		warps_.clear();
	}

	/**************************************************************************/

	void GameMap::ClearWarpTargetPair(unsigned int index)
	{
		if (index < warps_.size())
		{
			warps_.erase(warps_.begin() + index);
		}
	}

	/**************************************************************************/

	unsigned int GameMap::GetNumWarpTargetPairs() const
	{
		return warps_.size();
	}
}

