
// CODESTYLE: v2.0

// GameMapLayer.h
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a map layer

#include "GameLibrary.h"

namespace GAME
{
	GameMapLayer::GameMapLayer(GameTileset *tileSet,int cols,int rows) :
		tiles_(cols * rows), numCols_(cols), numRows_(rows), tileSet_(tileSet)
	{
	}

	void GameMapLayer::Resize(int cols,int rows)
	{
		numCols_ = cols;
		numRows_ = rows;
		tiles_.resize(0);
		tiles_.resize(cols * rows);
	}

	int GameMapLayer::GetNumColumns() const
	{
		return numCols_;
	}

	int GameMapLayer::GetNumRows() const
	{
		return numRows_;
	}

	int GameMapLayer::GetWidth() const
	{
		return numCols_ * tileSet_->GetTileWidth();
	}

	int GameMapLayer::GetHeight() const
	{
		return numRows_ * tileSet_->GetTileHeight();
	}

	void GameMapLayer::PixelToTileIndex(int x,int y,int *col,int *row) const
	{
		*col = (x / tileSet_->GetTileWidth());
		*row = (y / tileSet_->GetTileHeight());
	}

	int GameMapLayer::GetTileIndexAt(int col,int row) const
	{
		return tiles_[row * numCols_ + col].tileIndex;
	}

	void GameMapLayer::SetTileIndexAt(int col,int row,int index)
	{
		tiles_[row * numCols_ + col].tileIndex = index;
	}

	int GameMapLayer::GetEventAt(int col,int row) const
	{
		return tiles_[row * numCols_ + col].eventNo;
	}

	void GameMapLayer::SetEventAt(int col,int row,int event)
	{
		tiles_[row * numCols_ + col].eventNo = event;
	}

	std::vector<GameMapLayer::Tile>& GameMapLayer::GetTileData()
	{
		return tiles_;
	}

	void GameMapLayer::DrawLayer(ImageResource *destImage,int srcX,int srcY,int destX,int destY,int width,int height) const
	{
		// Perform basic clipping
		if(destX >= destImage->GetWidth()) return;
		if(destY >= destImage->GetHeight()) return;
		if(srcX >= GetWidth()) return;
		if(srcY >= GetHeight()) return;

		// use (destX2,destY2) to represent the bottom right corner destination
		int destX2 = destX + width;
		int destY2 = destY + height;

		if(destX2 < 0) return;
		if(destY2 < 0) return;

		if(destX < 0)
		{
			srcX -= destX;
			destX = 0;
		}
		if(destY < 0)
		{
			srcY -= destY;
			destY = 0;
		}
		if(destX2 >= destImage->GetWidth()) destX2 = destImage->GetWidth() - 1;
		if(destY2 >= destImage->GetHeight()) destY2 = destImage->GetHeight() - 1;

		int srcX2 = srcX + destX2 - destX;
		int srcY2 = srcY + destY2 - destY;

		if(srcX2 < 0) return;
		if(srcY2 < 0) return;

		if(srcX < 0)
		{
			destX -= srcX;
			srcX = 0;
		}

		if(srcY < 0)
		{
			destY -= srcY;
			srcY = 0;
		}

		if(srcX2 >= GetWidth()) srcX2 = GetWidth() - 1;
		if(srcY2 >= GetHeight()) srcY2 = GetHeight() - 1;

		width = srcX2 - srcX;
		height = srcY2 - srcY;

		const int tileWidth = tileSet_->GetTileWidth();
		const int tileHeight = tileSet_->GetTileHeight();

		int tileX1, tileY1, tileX2, tileY2;
		// compute the tile coordinate (tileX1,tileY1), given the top-left corner (srcX1,srcY1)
		PixelToTileIndex(srcX,srcY,&tileX1,&tileY1);
		// compute (tileX2,tileY2) from the bottom right corner
		PixelToTileIndex(srcX + width,srcY + height,&tileX2,&tileY2);
		// (tileX2,tileY2) are non-inclusive, so add one to them
		tileX2++;
		tileY2++;

		// render tiles from (tileX1,tileY1) -> (tileX2,tileY2)
		int row = tileY1 * numCols_ + tileX1; // get the array index to (tileX1,tileY1)
		for(int y = tileY1 * tileHeight; y < tileY2 * tileHeight; y += tileHeight)
		{
			for(int i = row, x = tileX1 * tileWidth; x < tileX2 * tileWidth; x += tileWidth, i++)
			{
				int sx = 0; // sourceX of current tile
				int sy = 0; // sourceY of current tile
				int dx = destX + x - srcX; // destX
				int dy = destY + y - srcY; // destY
				int w = tileWidth;
				int h = tileHeight;

				// compute clipping for egdes
				if(dx < destX)
				{
					sx += destX - dx;
					w -= destX - dx;
					dx = destX;
				}

				if(dy < destY)
				{
					sy += destY - dy;
					h -= destY - dy;
					dy = destY;
				}

				if(dx + w > destX + width) w = destX + width - dx;
				if(dy + h > destY + height) h = destY + height - dy;

				tileSet_->GetImageResourceAt(tiles_[i].tileIndex)->BlitMasked(destImage,sx,sy,dx,dy,w,h);
			}
			row += numCols_;
		}
	}

} // end namespace

