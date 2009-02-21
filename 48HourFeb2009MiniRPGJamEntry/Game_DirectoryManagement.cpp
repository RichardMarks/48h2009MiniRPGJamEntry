
// CODESTYLE: v2.0

// Game_WorldLoader.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the implementation for the directory management methods of the game singleton class

#include "GameLibrary.h"

namespace GAME
{
	void GameSingleton::SetTilesDirectory(const char* path)
	{
		tilesDirectory_ = path;
	}

	/**************************************************************************/

	void GameSingleton::SetMapsDirectory(const char* path)
	{
		mapsDirectory_ = path;
	}

	/**************************************************************************/

	void GameSingleton::SetSpritesDirectory(const char* path)
	{
		spritesDirectory_ = path;
	}

	/**************************************************************************/

	std::string GameSingleton::GetTilesDirectory() const
	{
		return tilesDirectory_;
	}

	/**************************************************************************/

	std::string GameSingleton::GetMapsDirectory() const
	{
		return mapsDirectory_;
	}

	/**************************************************************************/

	std::string GameSingleton::GetSpritesDirectory() const
	{
		return spritesDirectory_;
	}

} // end namespace
