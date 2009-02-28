
// CODESTYLE: v2.0

// GameMap.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a game map

#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include <string>
#include <vector>
#include "GameMapLayer.h"

namespace GAME
{
	class GameTileset;
	class GameMapEvent;


/**
		a map consists of a vector of at least 3 layers
		a map has a name and an ID #
		a map holds a list of events
		a map holds a list of warp target pairs
*/

	struct WarpTargetPair
	{
		int startX_, startY_, startMap_;
		int endX_, endY_, endMap_;

		WarpTargetPair() :
			startX_(0),
			startY_(0),
			startMap_(0),
			endX_(0),
			endY_(0),
			endMap_(0)
		{
		}

		WarpTargetPair(int startX, int startY, int startMap, int endX, int endY, int endMap) :
			startX_(startX),
			startY_(startY),
			startMap_(startMap),
			endX_(endX),
			endY_(endY),
			endMap_(endMap)
		{
		}

		void Copy(WarpTargetPair& source)
		{
			startX_ 	= source.startX_;
			startY_ 	= source.startY_;
			startMap_ 	= source.startMap_;
			endX_ 		= source.endX_;
			endY_ 		= source.endY_;
			endMap_ 	= source.endMap_;

		}
	};


	class GameMap
	{
	public:
		/**
		 * Creates a new empty game map.
		 * @param tileSet GameTileset to use.
		 * @remarks tileSet must remain a valid pointer throughout the lifetime of GameMap.
		 *          User is responsible for deallocating tileSet.
		 */
		GameMap(GameTileset *tileSet);

		/**
		 * Erases the contents of the current map.
		 * Does not affect current tileset.
		 */
		void Clear();

		/**
		 * Erases the current map's contents and loads map tile and event data from file.
		 * The current map data must be compatible with the current tileset, or else,
		 * call SetGameTileset() prior to this call to set the appropriate tileset.
		 * @param pathname Pathname to the .map file to load.
		 * @deprecated -- do not use this function use the Load* functions instead
		 */
		void LoadMapFromFile(const char *pathname);

		/**
		 * Changes the current tile set assoicated with the map.
		 * This does not automatically switch all the tiles in the current map,
		 * rather all new layers will use the new GameTileset. THe passed tileset
		 * must be a valid pointer throughout the lifetime of GameMap, and the user
		 * is reponsible for delete it when its not used.
		 * @param tileSet Pointer to a GameTileset
		 * @return Returns the tile set previously assigned to the current map
		 */
		GameTileset *SetGameTileset(GameTileset *tileSet);

		/// gets a pointer to the tileset used by the map
		GameTileset* GetTileset() const;

		/**
		 * Creates a new map layer.
		 * @param cols Number of colums
		 * @param rows Number of rows
		 * @return Returns the layer index of the created map layer.
		 */
		 int CreateMapLayer(int cols,int rows);

		 /**
		  * Returns the MapLayer at index
		  * @param layerIndex Layer index
		  * @return Pointer to the map layer, or NULL if @a layerIndex is out of bound.
		  */
		 GameMapLayer* GetGameMapLayer(int layerIndex);

		 /**
		  * Draws the map.
		  * @param srcX Source X position given in pixels.
		  * @param srcY Source Y position given in pixels.
		  * @param destX Destination X position given in pixels.
		  * @param destY Destination Y position given in pixels.
		  * @param width Width of the map in pixels to copy. If 0, uses the dest image's width.
		  * @param height Height of the map in pixels to copy. If 0, uses the dest image's height.
		  */
		 void DrawMap(ImageResource *destImage,int srcX = 0,int srcY = 0,int destX = 0,int destY = 0,int width = 0,int height = 0);


/*#**************************************************************************#*/

		// the new map data interface

		/// saves .map
		void SaveMapData(const char* filePath);

		/// loads .map
		void LoadMapData(const char* filePath);

		/// saves .collision
		void SaveCollisionData(const char* filePath);

		/// loads .collision
		void LoadCollisionData(const char* filePath);

		/// saves .event
		void SaveEventData(const char* filePath);

		/// loads .event
		void LoadEventData(const char* filePath);

		/// saves .warp
		void SaveMapWarpData(const char* filePath);

		/// loads .warp
		void LoadMapWarpData(const char* filePath);

/*#**************************************************************************#*/

		/// toggles the solidness of the specified tile
		void SetTileSolid(int tileX, int tileY, bool isSolid = true);

		/// returns true if the specified tile is solid
		bool IsSolid(int tileX, int tileY) const;

		/// returns true if the specified tile is a warp tile
		bool IsWarp(int tileX, int tileY) const;

		void SetWarp(int tileX, int tileY, int warpIndex);
		int GetWarp(int tileX, int tileY) const;

		//

		void ClearMapEvents();
		void AddMapEvent(GameMapEvent* event);
		GameMapEvent* GetMapEvent(int eventIndex);
		const int GetID() const;

		void SetName(const char* mapName);
		std::string GetName() const;

		void DebugList();

		static int totalMapCount; // used for IDs

	public:

		void AddWarpTargetPair(int startX, int startY, int startMap, int endX, int endY, int endMap);

		WarpTargetPair* GetWarpTargetPair(unsigned int index) const;

		void ClearWarpTargetPairs();

		void ClearWarpTargetPair(unsigned int index);

		unsigned int GetNumWarpTargetPairs() const;

		/// destroys all warps on the map
		void NukeWarps();

	private:
		std::vector<WarpTargetPair*> warps_;

		std::vector<GameMapLayer> layers_;
		GameTileset *tileSet_;

		std::vector<GameMapEvent*> events_;

		int mapID_;
		std::string mapName_;
	}; // end class

} // end namespace
#endif


