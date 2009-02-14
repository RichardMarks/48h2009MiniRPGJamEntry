
// CODESTYLE: v2.0

// GameTileset.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Redslash
// Purpose: the class that manages an array of tile images 

#ifndef __GAMETILESET_H__
#define __GAMETILESET_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	using ENGINE::ImageResource;
	
	/**
	 * An array of ImageResources, each representing a tile.
	 */
	class GameTileset
	{
	public:
		/**
		 * Create a tileset from a given image resource, with the tiles
		 * arranged in a grid. Tile 0 starts from the top left of the images, for
		 * which each subsequent tile progresses to the right of the previous tile
		 * for @a cols count, then continues on the next row.
		 * Example tile layout (4x3; 12 tiles total):
		 * +--+--+--+--+
		 * | 0| 1| 2| 3|
		 * +--+--+--+--+
		 * | 4| 5| 6| 7|
		 * +--+--+--+--+
		 * | 8| 9|10|11|
		 * +--+--+--+--+
		 * @param image Pointer to an image resource which must be atleast (@a tileWidth x @a cols,@a tileHeight x @a rows) in size.
		 * @param tileSize Tile size in pixels, should be >= 1
		 * @param tileWidth Width of tile
		 * @param tileHeight Height of tile
		 * @param cols Number of columns
		 * @param rows Number of rows
		 * @param spacing Number of pixels between each tile
		 */
		GameTileset(ImageResource *image,int tileWidth,int tileHeight,int cols,int rows,int spacing = 1);
		
		/**
		 * Copy constructs the tile set.
		 */
		GameTileset(const GameTileset& rhs);
		
		~GameTileset();
		
		/// Assignment operator
		GameTileset& operator =(const GameTileset& rhs);
		
		/**
		 * Return the number of tiles.
		 */
		int GetNumTiles() const;
		
		/**
		 * Return the tile width in pixels.
		 */
		int GetTileWidth() const;
		
		/**
		 * Return the tile height in pixels.
		 */
		int GetTileHeight() const;
		
		/**
		 * Use to get the ImageResource for a tile at index.
		 * @param index Index of tile
		 */
		ImageResource* operator[](int index);

		/**
		 * Returns the image resource at @a index.
		 * @param index Index of tile.
		 */
		ImageResource* GetImageResourceAt(int index);
	private:		
		int numCols_;
		int numRows_;
		int tileWidth_;
		int tileHeight_;
		std::vector<ImageResource*> tiles_; 
	}; // end class

} // end namespace
#endif

