
// CODESTYLE: v2.0

// GameMapCell.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a map layer

#ifndef __GAMEMAPLAYER_H__
#define __GAMEMAPLAYER_H__

#include <vector>

namespace ENGINE
{
	class ImageResource;
}

namespace GAME
{
	class GameTileset;
	using ENGINE::ImageResource;

	/// A layer in the game map.
	class GameMapLayer
	{
	public:
		struct Tile
		{
			unsigned int tileIndex;
			signed int eventNo;
			unsigned char solid;
			unsigned int warp;

			Tile() :
				tileIndex(0),
				eventNo(0),
				solid(0),
				warp(0) {}
		};

		/**
		 * Creates a new map layer of cols x rows.
		 * @param tileSet GameTileset to use for this layer.
		 * @param cols Number of columns
		 * @param rows Number of rows
		 */
		GameMapLayer(GameTileset *tileSet,int cols,int rows);

		/**
		 * Clears the current layer and resizes the dimension.
		 * @param cols Number of columns
		 * @param rows Number of rows.
		 */
		void Resize(int cols,int rows);

		/// Gets the number of colums
		int GetNumColumns() const;

		/// Gets the number of rows
		int GetNumRows() const;

		/// Returns the width of the layer in pixels
		int GetWidth() const;

		/// Returns the height of the layer in pixels
		int GetHeight() const;

		/**
		 * Obtain col and row for the tile at pixel position (x,y)
		 * @param x X
		 * @param y Y
		 * @param col Output variable to store the column index
		 * @param row Output variable to store the row index
		 */
		void PixelToTileIndex(int x,int y,int *col,int *row) const;

		/**
		 * Returns the tile index at (col,row).
		 * @param col Column number
		 * @oaram row Row number
		 * @return Returns the index to the tile at the given coordinates.
		 */
		int GetTileIndexAt(int col,int row) const;

		/**
		 * Sets the tile index at (col, row).
		 * @param col Column number
		 * @param row Row number
		 * @param index Index value to set (0-65535).
		 */
		void SetTileIndexAt(int col,int row,int index);

		/**
		 * Returns the tile event at (col,row)
		 * @param col Column number
		 * @oaram row Row number
		 */
		int GetEventAt(int col,int row) const;

		/**
		 * Set's the event at (col,row)
		 * @param col Column number
		 * @oaram row Row number
		 * @param event Event number (0-255)
		 */
		void SetEventAt(int col,int row,int event);






		/**
		 * Gets the tile data array. Used internally.
		 */
		std::vector<GameMapLayer::Tile>& GetTileData();

		/**
		 * Composites the layer to the provided image resource.
		 * Uses the provided color key, or uses alpha composition if color key is -1.
		 * @param destImage Destination image.
		 * @param srcX Source X position given in pixels.
		 * @param srcY Source Y position given in pixels.
		 * @param destX Destination X position given in pixels.
		 * @param destY Destination Y position given in pixels.
		 * @param width Width of the map in pixels to copy. If 0, uses the dest image's width.
		 * @Param height Height of the map in pixels to copy. If 0, uses the dest image's height.
		 */
		void DrawLayer(ImageResource *destImage,int srcX = 0,int srcY = 0,int destX = 0,int destY = 0,int width = 0,int height = 0) const;



		int GetSolidAt(int col,int row) const;
		void SetSolidAt(int col,int row, int event);
		int GetWarpAt(int col,int row) const;
		void SetWarpAt(int col,int row, int event);




		void FillEvents(int value);
		void FillWarps(int value);
		void FillSolid(int value);
		void FillTiles(int value);

	private:
		std::vector<Tile> tiles_; // tile indices of size numCols_ x numRows_
		int numCols_;
		int numRows_;
		GameTileset *tileSet_;
	}; // end class

} // end namespace
#endif


