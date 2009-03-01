
// CODESTYLE: v2.0

// GameTileset.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Redslash
// Purpose: the class that manages an array of tile images

#include "GameLibrary.h"

namespace GAME
{
	GameTileset::GameTileset(ImageResource *image,int tileWidth,int tileHeight,int cols,int rows,int spacing) :
		numCols_(cols), numRows_(rows), tileWidth_(tileWidth), tileHeight_(tileHeight), tiles_(cols * rows)
	{
		const int width = cols * (tileWidth + spacing) + spacing; // calculate width/height of tileset based on col/row
		const int height = rows * (tileHeight + spacing) + spacing;
		for(int i = 0, y = spacing; y < height; y += tileHeight + spacing)
		{
			for(int x = spacing; x < width; x += tileWidth + spacing, i++)
			{
				tiles_[i] = new ImageResource(tileWidth,tileHeight);
				image->Blit(tiles_[i],x,y,0,0,tileWidth,tileHeight);
			}
		}
	}

	/**************************************************************************/

	GameTileset::GameTileset(const GameTileset& rhs) :
		numCols_(rhs.numCols_), numRows_(rhs.numRows_), tileWidth_(rhs.tileWidth_), tileHeight_(rhs.tileHeight_), tiles_(numCols_ * numRows_)
	{
		for(int i = 0; i < (int)tiles_.size(); i++)
		{
			tiles_[i] = new ImageResource(rhs.tiles_[i]->GetBitmap());
		}
	}

	/**************************************************************************/

	GameTileset::~GameTileset()
	{
		for(int i = 0; i < (int)tiles_.size(); i++)
		{
			delete tiles_[i];
		}
	}

	/**************************************************************************/

	GameTileset& GameTileset::operator =(const GameTileset& rhs)
	{
		for(int i = 0; i < (int)tiles_.size(); i++)
		{
			delete tiles_[i];
		}
		tiles_.resize(rhs.tiles_.size());

		numCols_ = rhs.numCols_;
		numRows_ = rhs.numRows_;
		tileWidth_ = rhs.tileWidth_;
		tileHeight_ = rhs.tileHeight_;

		for(int i = 0; i < (int)tiles_.size(); i++)
		{
			tiles_[i] = new ImageResource(rhs.tiles_[i]->GetBitmap());
		}

		return *this;
	}

	/**************************************************************************/

	int GameTileset::GetNumTiles() const
	{
		return tiles_.size();
	}

	/**************************************************************************/

	int GameTileset::GetTileWidth() const
	{
		return tileWidth_;
	}

	/**************************************************************************/

	int GameTileset::GetTileHeight() const
	{
		return tileHeight_;
	}

	/**************************************************************************/

	ImageResource* GameTileset::operator[](int index)
	{
		return tiles_[index];
	}

	/**************************************************************************/

	ImageResource* GameTileset::GetImageResourceAt(int index)
	{
		return tiles_[index];
	}

} // end namespace

