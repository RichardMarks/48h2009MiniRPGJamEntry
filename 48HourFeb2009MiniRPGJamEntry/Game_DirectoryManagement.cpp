
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
		// tilesDirectory_ = path;
		LogWarning("DEPRECATED FUNCTION");
	}

	/**************************************************************************/

	void GameSingleton::SetMapsDirectory(const char* path)
	{
		//mapsDirectory_ = path;
		LogWarning("DEPRECATED FUNCTION");
	}

	/**************************************************************************/

	void GameSingleton::SetSpritesDirectory(const char* path)
	{
		//spritesDirectory_ = path;
		LogWarning("DEPRECATED FUNCTION");
	}

	/**************************************************************************/

	std::string GameSingleton::GetTilesDirectory() const
	{
		// return tilesDirectory_;
		return static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("graphics_path") +
						gameSettings_->Get("tiles_path"));
	}

	/**************************************************************************/

	std::string GameSingleton::GetMapsDirectory() const
	{
		//return mapsDirectory_;
		return static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("map_path"));
	}

	/**************************************************************************/

	std::string GameSingleton::GetSpritesDirectory() const
	{
		//return spritesDirectory_;
		return static_cast<std::string>(
				gameSettings_->Get("data_path") +
					gameSettings_->Get("graphics_path") +
						gameSettings_->Get("game_sprite_path"));
	}

} // end namespace
