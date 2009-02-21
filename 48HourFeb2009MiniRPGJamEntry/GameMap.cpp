// CODESTYLE: v2.0

// GameMap.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks, RedSlash
// Purpose: the class that defines a map

#include "GameLibrary.h"

namespace GAME
{
	int GameMap::totalMapCount = 0;

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
		layers_[0].SetEventAt(tileX, tileY, (isSolid) ? 0xFF : 0x0);
	}

	/**************************************************************************/

	bool GameMap::IsSolid(int tileX, int tileY) const
	{
		return (layers_[0].GetEventAt(tileX, tileY) == 0xFF) ? true : false;
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
			LogFatal("Could not open %s for reading.\n", mapFileName.c_str());
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
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".collision";

		// open the file
		FILE* fp = fopen(mapFileName.c_str(), "rb");
		if (!fp)
		{
			LogFatal("Could not open %s for reading.\n", mapFileName.c_str());
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

		LogMessage("TODO: implement event loading routine");
	}

	/**************************************************************************/

	void GameMap::SaveMapWarpData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.warp
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".warp";

		LogMessage("TODO: implement map warp saving routine");
	}

	/**************************************************************************/

	void GameMap::LoadMapWarpData(const char* filePath)
	{
		// build file name string
		// for a map named "Test1" the file path is
		// data/maps/Test1/Test1.warp
		std::string mapFileName = filePath + mapName_ + "/" + mapName_ + ".warp";

		LogMessage("TODO: implement map warp loading routine");
	}

/*#**************************************************************************#*/

}

