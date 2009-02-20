
// CODESTYLE: v2.0

// GameBattleScene.cpp
// Project: 48h Contest LO-Fi Mini-RPG Game Project (GAME)
// Author: Richard Marks
// Purpose: the class that defines a battle background scene

#include "GameLibrary.h"

namespace GAME
{
	GameBattleScene::GameBattleScene() :
		name_("<undefined>"),
		image_(0)
	{
	}

	/**************************************************************************/

	GameBattleScene::~GameBattleScene()
	{
		name_ = "";
		terrains_.clear();
	}

	/**************************************************************************/

	void GameBattleScene::Setup(const char* name, const char* terrainString, ImageResource* image)
	{
		name_ 	= name;
		image_ 	= image;
		SetTerrains(terrainString);
	}

	/**************************************************************************/

	void GameBattleScene::SetTerrains(const char* terrainString)
	{
		terrains_.clear();
		std::string terrainStr = terrainString;
		if ("" == terrainStr)
		{
			// we just wanted to clear the list
			return;
		}

		std::vector<std::string> terrainList = GameSingleton::GetInstance()->Tokenize(terrainStr, ",");
		int terrainCount = terrainList.size();
		for (int index = 0; index < terrainCount; index++)
		{
			terrains_.push_back(static_cast<unsigned int>(atoi(terrainList.at(index).c_str())));
		}
	}

	/**************************************************************************/

	void GameBattleScene::AddTerrain(unsigned int terrain)
	{
		terrains_.push_back(terrain);
	}

	/**************************************************************************/

	void GameBattleScene::SetName(const char* name)
	{
		name_ = name;
	}

	/**************************************************************************/

	void GameBattleScene::SetImage(ImageResource* image)
	{
		image_ = image;
	}

	/**************************************************************************/

	ImageResource* GameBattleScene::GetImage() const
	{
		return image_;
	}

	/**************************************************************************/

	std::string GameBattleScene::GetName() const
	{
		return static_cast<std::string>(name_);
	}

	/**************************************************************************/

	bool GameBattleScene::IsFoundOnTerrain(unsigned int terrain)
	{
		unsigned int terrainCount = terrains_.size();

		for (unsigned int index = 0; index < terrainCount; index++)
		{
			if (terrain == terrains_.at(index))
			{
				return true;
			}
		}

		return false;
	}

} // end namespace
