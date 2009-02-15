// CODESTYLE: v2.0

// GameMap.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a map

#include "GameLibrary.h"

namespace GAME
{
	int GameMap::totalMapCount = 0;

	GameMap::GameMap(GameTileset *tileSet) : tileSet_(tileSet)
	{
		mapID_ = GameMap::totalMapCount;
		GameMap::totalMapCount++;
	}

	const int GameMap::GetID() const
	{
		return mapID_;
	}

	void GameMap::Clear()
	{
		std::vector<GameMapLayer>().swap(layers_);
	}

	void GameMap::LoadMapFromFile(const char *pathname)
	{
		// map format: http://ccpssolutions.com/nogdusforums/index.php?topic=365.msg1163;topicseen#msg1163
		// unsigned char mapWidth;
		// unsigned char mapHeight;
		// map data: (mapWidth*mapHeight) of unsigned char
		// collision/event data: (mapWidth*mapHeight) of unsigned char

		Clear();

		std::FILE *file = std::fopen(pathname,"rb");
		if(!file) LogFatal("%s: Failed to open file.\n");

		// there is no error checking for map files, make sure they are correct!
		unsigned char mapWidth, mapHeight;
		fread(&mapWidth,1,1,file);
		fread(&mapHeight,1,1,file);

		int layerIndex = CreateMapLayer(mapWidth,mapHeight);
		GameMapLayer *layer = GetGameMapLayer(layerIndex);

		std::vector<GameMapLayer::Tile>& tiles = layer->GetTileData();

		const int tileSize = mapWidth * mapHeight;
		for(int i = 0; i < tileSize; i++)
		{
			unsigned char index;
			std::fread(&index,1,1,file);
			tiles[i].tileIndex = index;
		}

		for(int i = 0; i < tileSize; i++)
		{
			unsigned char event;
			std::fread(&event,1,1,file);
			tiles[i].eventNo = event;
		}

		std::fclose(file);
	}

	GameTileset *GameMap::SetGameTileset(GameTileset *tileSet)
	{
		std::swap(tileSet_,tileSet);
		return tileSet;
	}

	 int GameMap::CreateMapLayer(int cols,int rows)
	 {
	 	layers_.push_back(GameMapLayer(tileSet_,cols,rows));
	 	return (int)layers_.size() - 1;
	 }

	 GameMapLayer* GameMap::GetGameMapLayer(int layerIndex)
	 {
		 return &layers_[layerIndex];
	 }

	 void GameMap::DrawMap(ImageResource *destImage,int srcX,int srcY,int destX,int destY,int width,int height)
	 {
	 	for(int i = 0; i < (int)layers_.size(); i++)
	 		layers_[i].DrawLayer(destImage,srcX,srcY,destX,destY,width,height);
	 }




	void GameMap::ClearMapEvents()
	{
		events_.clear();
	}

	void GameMap::AddMapEvent(GameMapEvent* event)
	{
		#if 0
		int wx, wy, par, tar, tx, ty;
		bool a, p;
		std::string x;
		GameMapEventHandler* h;
		event->GetLocation(par, wx, wy);
		event->GetWarpTargetLocation(tar, tx, ty);
		h = event->GetEventHandler();
		a = event->IsArmed();
		p = event->IsPersistent();
		x = event->GetExtra();

		fprintf(stderr,
		"Event Data:\n{\n"
		"\tworldX_ = %d;\n"
		"\tworldY_ = %d;\n"
		"\tarmed_ = %s;\n"
		"\tpersists_ = %s;\n"
		"\tparentMapIndex_ = %d;\n"
		"\ttargetMapIndex_ = %d;\n"
		"\ttargetX_ = %d;\n"
		"\ttargetY_ = %d;\n"
		"\textra_ = %s;\n"
		"}\n",

		wx, wy, (a)?"true":"false",
		(p)?"true":"false",
		par,tar,tx,ty,x.c_str());
		#endif

		events_.push_back(event);
	}

	GameMapEvent* GameMap::GetMapEvent(int eventIndex)
	{
		return (static_cast<unsigned int>(eventIndex) < events_.size()) ? events_.at(eventIndex) : 0;
	}

	void GameMap::SetName(const char* mapName)
	{
		mapName_ = mapName;
	}

	std::string GameMap::GetName() const
	{
		return mapName_;
	}

	void GameMap::DebugList()
	{
#if 0
		std::vector<GameMapLayer>::iterator layerIter;
		std::vector<GameMapEvent*>::iterator eventIter;

		// list the layer info
		int width = layers_.at(0).GetNumColumns();
		int height = layers_.at(0).GetNumRows();
		fprintf(stderr, "Map %d has %d layers that are %d x %d tiles in size.\n", mapID_, static_cast<int>(layers_.size()), width, height);

		// list the events
		for (eventIter = events_.begin(); eventIter != events_.end(); eventIter++)
		{
			GameMapEvent* event = *(eventIter);
			#if 1
			int wx, wy, par, tar, tx, ty;
			bool a, p;
			std::string x;
			GameMapEventHandler* h;
			event->GetLocation(par, wx, wy);
			event->GetWarpTargetLocation(tar, tx, ty);
			h = event->GetEventHandler();
			a = event->IsArmed();
			p = event->IsPersistent();
			x = event->GetExtra();

			fprintf(stderr,
			"Event Data:\n{\n"
			"\tworldX_ = %d;\n"
			"\tworldY_ = %d;\n"
			"\tarmed_ = %s;\n"
			"\tpersists_ = %s;\n"
			"\tparentMapIndex_ = %d;\n"
			"\ttargetMapIndex_ = %d;\n"
			"\ttargetX_ = %d;\n"
			"\ttargetY_ = %d;\n"
			"\textra_ = \"%s\";\n"
			"}\n",

			wx, wy, (a)?"true":"false",
			(p)?"true":"false",
			par,tar,tx,ty,x.c_str());
			#endif
		}
#endif
	}


}

